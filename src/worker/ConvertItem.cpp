// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

enum Mode { None = -1, Encode = 0, Transcode = 1 };

bool ConvertItem(CItemContext* pContext)
{
    bool bSuccess = false;
    TCHAR szCommandLine[(64 * 1024)];
    ZeroMemory(szCommandLine, sizeof(szCommandLine));

    // input path
    CString szInputFile = pContext->item->szPath;

    // output path
    CString szOutPath;
    if (pContext->pWorkerContext->pConfig->m_Options.bOutputPathChecked == false)
    {
        szOutPath = szInputFile;
        CString szToRemove = ::GetFileName(szInputFile);
        int nNewLenght = szOutPath.GetLength() - szToRemove.GetLength();
        szOutPath.Truncate(nNewLenght);
    }
    else
    {
        szOutPath = pContext->pWorkerContext->pConfig->m_Options.szOutputPath;
    }

    // output format
    int nEncoder = pContext->pWorkerContext->pConfig->m_Formats.GetFormatById(pContext->item->szFormatId);
    if (nEncoder == -1)
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find valid encoder by id."));
        return bSuccess;
    }

    CFormat& encoderFormat = pContext->pWorkerContext->pConfig->m_Formats.GetData(nEncoder);
    CPreset& encoderPreset = encoderFormat.m_Presets.GetData(pContext->item->nPreset);
    CString szEncoderExePath = encoderFormat.szPath;
    CString szEncoderOptions = encoderPreset.szOptions;

    // check input extensions
    CString szInputFileExt = ::GetFileExtension(szInputFile).MakeUpper();
    bool bIsValidEncoderInput = encoderFormat.IsValidInputExtension(szInputFileExt);

    // output path

    CString szEncoderExtension = encoderFormat.szOutputExtension;
    CString szName = pContext->item->szName + _T(".") + szEncoderExtension.MakeLower();
    CString szOutputFile;
    if (szOutPath.GetLength() >= 1)
    {
        if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
            szOutputFile = szOutPath + szName;
        else
            szOutputFile = szOutPath + _T("\\") + szName;
    }
    else
    {
        szOutputFile = szName;
    }

    // processing mode
    Mode nProcessingMode = Mode::None;
    if (bIsValidEncoderInput)
        nProcessingMode = Mode::Encode;
    else
        nProcessingMode = Mode::Transcode;

    bool bDecode = false;
    int nTool = -1;
    bool bUseInPipesEnc = false;
    bool bUseOutPipesEnc = false;
    bool bUseInPipesDec = false;
    bool bUseOutPipesDec = false;
    CString szOrgInputFile = szInputFile;
    CString szOrgOutputFile = szOutputFile;

    // decode before encoding
    if (nProcessingMode == Mode::Transcode)
    {
        int nDecoder = pContext->pWorkerContext->pConfig->m_Formats.GetDecoderByExtension(pContext->item->szExtension);
        if (nDecoder == -1)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find valid decoder by extension."));
            return bSuccess;
        }

        CFormat& decoderFormat = pContext->pWorkerContext->pConfig->m_Formats.GetData(nDecoder);
        CPreset& decoderPreset = decoderFormat.m_Presets.GetData(decoderFormat.nDefaultPreset);
        CString szDecoderExePath = decoderFormat.szPath;
        CString szDecoderOptions = decoderPreset.szOptions;

        bIsValidEncoderInput = encoderFormat.IsValidInputExtension(decoderFormat.szOutputExtension);
        if (bIsValidEncoderInput == false)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: decoder output not supported by encoder."));
            return bSuccess;
        }

        if (pContext->pWorkerContext->pConfig->m_Options.bForceConsoleWindow == false)
        {
            // configure decoder input and output pipes
            bUseInPipesDec = decoderFormat.bInput;
            bUseOutPipesDec = decoderFormat.bOutput;
        }

        // input file is stdin
        if (bUseInPipesDec == true)
            szInputFile = _T("-");

        // output file is stdout
        if (bUseOutPipesDec == true)
            szOutputFile = _T("-");

        // TODO: bUseOutPipes == true than handle szOutputFile same as input file
        if (nProcessingMode == Mode::Transcode)
        {
            CString szDecoderExtension = decoderFormat.szOutputExtension;
            szOutputFile = szOutputFile + +_T(".") + szDecoderExtension.MakeLower();
        }

        // build full command line for decoder (DECODER-EXE + OPTIONS + INFILE + OUTFILE) 
        // this is basic model, some of encoder may have different command-line structure
        CString szExecute;
        szExecute = decoderFormat.szTemplate;
        szExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
        szExecute.Replace(_T("$EXE"), szDecoderExePath);
        szExecute.Replace(_T("$OPTIONS"), szDecoderOptions);
        szExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
        szExecute.Replace(_T("$INFILE"), szInputFile);
        szExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
        szExecute.Replace(_T("$OUTFILE"), szOutputFile);

        bDecode = true;
        nTool = pContext->pWorkerContext->pConfig->m_Formats.GetFormatById(decoderFormat.szId);
        if (nTool == -1)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: failed to get decoder by format id."));
            return bSuccess;
        }

        lstrcpy(szCommandLine, szExecute.GetBuffer(szExecute.GetLength()));

        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Decoding..."));

        CFileContext context;
        context.pWorkerContext = pContext->pWorkerContext;
        context.szInputFile = szOrgInputFile;
        context.szOutputFile = szOrgOutputFile;
        context.szCommandLine = szCommandLine;
        context.nIndex = pContext->item->nId;
        context.bDecode = bDecode;
        context.nTool = nTool;
        context.bUseReadPipes = bUseInPipesDec;
        context.bUseWritePipes = bUseOutPipesDec;

        // TODO: when transcoding don't show time stats
        bool bResult = false;
        try
        {
            bResult = ::ConvertFile(&context);
        }
        catch (...)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: exception thrown while decoding."));
            pContext->pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }

        if (bResult == true)
        {
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                ::DeleteFile(szOrgInputFile);
        }
        else
        {
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnError == true)
                ::DeleteFile(szOutputFile);

            if (pContext->pWorkerContext->bRunning == false)
                return bSuccess;

            if (pContext->pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                return bSuccess;

            // when transcoding we are skipping to next file no encoding can be done when decoding failed
            return bSuccess;
        }
    }

    if (pContext->pWorkerContext->bRunning == false)
        return bSuccess;

    if (pContext->pWorkerContext->pConfig->m_Options.bForceConsoleWindow == false)
    {
        // configure encoder input and output pipes
        bUseInPipesEnc = encoderFormat.bInput;
        bUseOutPipesEnc = encoderFormat.bOutput;
    }

    if (nProcessingMode == Mode::Encode)
    {
        // input file is stdin
        if (bUseInPipesEnc == true)
            szInputFile = _T("-");

        // output file is stdout
        if (bUseOutPipesEnc == true)
            szOutputFile = _T("-");
    }

    if (nProcessingMode == Mode::Transcode)
    {
        // input filename is decoded output filename
        szInputFile = szOutputFile;
        szOrgInputFile = szOutputFile;

        // restore output filename
        szOutputFile = szOrgOutputFile;

        // input file is stdin
        if (bUseInPipesEnc == true)
            szInputFile = _T("-");

        // output file is stdout
        if (bUseOutPipesEnc == true)
            szOutputFile = _T("-");
    }

    // encode
    if ((nProcessingMode == Mode::Encode) || (nProcessingMode == Mode::Transcode))
    {
        // build full command line for encoder (ENCODER-EXE + OPTIONS + INFILE + OUTFILE)
        // this is basic model, some of encoder may have different command-line structure
        CString szExecute;
        szExecute = encoderFormat.szTemplate;
        szExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
        szExecute.Replace(_T("$EXE"), szEncoderExePath);
        szExecute.Replace(_T("$OPTIONS"), szEncoderOptions);
        szExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
        szExecute.Replace(_T("$INFILE"), szInputFile);
        szExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
        szExecute.Replace(_T("$OUTFILE"), szOutputFile);

        bDecode = false;
        nTool = pContext->pWorkerContext->pConfig->m_Formats.GetFormatById(encoderFormat.szId);
        if (nTool == -1)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: failed to get encoder by format id."));
            return bSuccess;
        }

        lstrcpy(szCommandLine, szExecute.GetBuffer(szExecute.GetLength()));

        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Encoding..."));

        CFileContext context;
        context.pWorkerContext = pContext->pWorkerContext;
        context.szInputFile = szOrgInputFile;
        context.szOutputFile = szOrgOutputFile;
        context.szCommandLine = szCommandLine;
        context.nIndex = pContext->item->nId;
        context.bDecode = bDecode;
        context.nTool = nTool;
        context.bUseReadPipes = bUseInPipesEnc;
        context.bUseWritePipes = bUseOutPipesEnc;

        bool bResult = false;
        try
        {
            bResult = ::ConvertFile(&context);
        }
        catch (...)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: exception thrown while encoding."));
            pContext->pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }

        if (bResult == true)
        {
            bSuccess = true;
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                ::DeleteFile(szOrgInputFile);
        }
        else
        {
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnError == true)
                ::DeleteFile(szOutputFile);
        }

        if (nProcessingMode == Mode::Transcode)
            ::DeleteFile(szOrgInputFile);
    }

    return bSuccess;
}
