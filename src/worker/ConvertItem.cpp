// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

enum Mode { None = -1, Encode = 0, Transcode = 1 };

void ToFileContext(CFileContext &context, CWorkerContext *pWorkerContext, CFormat &format, int nPreset, int nItemId, CString szInputFile, CString szOutputFile)
{
    CPreset& preset = format.m_Presets.GetData(nPreset);

    context.pWorkerContext = pWorkerContext;
    context.szInputFile = szInputFile;
    context.szOutputFile = szOutputFile;

    CString szExecute;
    szExecute = format.szTemplate;
    szExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
    szExecute.Replace(_T("$EXE"), format.szPath);
    szExecute.Replace(_T("$OPTIONS"), preset.szOptions);

    szExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
    if (format.bPipeInput == true)
        szExecute.Replace(_T("$INFILE"), _T("-"));
    else
        szExecute.Replace(_T("$INFILE"), szInputFile);

    szExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
    if (format.bPipeOutput == true)
        szExecute.Replace(_T("$OUTFILE"), _T("-"));
    else
        szExecute.Replace(_T("$OUTFILE"), szOutputFile);

    TCHAR szCommandLine[(64 * 1024)];
    ZeroMemory(szCommandLine, sizeof(szCommandLine));
    lstrcpy(szCommandLine, szExecute.GetBuffer(szExecute.GetLength()));
    context.szCommandLine = szCommandLine;

    context.nItemId = nItemId;
    context.szFunction = format.szFunction;
    context.bUseReadPipes = format.bPipeInput;
    context.bUseWritePipes = format.bPipeOutput;
}

bool ConvertItem(CItemContext* pContext)
{
    // input path
    CString szInputFile = pContext->item->szPath;

    // output path
    CString szOutPath;
    if (pContext->pWorkerContext->pConfig->m_Options.bOutputPathChecked == true)
    {
        szOutPath = pContext->pWorkerContext->pConfig->m_Options.szOutputPath;
    }
    else
    {
        CString szInputFileName = ::GetFileName(szInputFile);
        szOutPath = szInputFile;
        szOutPath.Truncate(szOutPath.GetLength() - szInputFileName.GetLength());
    }

    // output format
    int nEncoder = pContext->pWorkerContext->pConfig->m_Formats.GetFormatById(pContext->item->szFormatId);
    if (nEncoder == -1)
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find valid encoder by id."));
        return false;
    }

    CFormat& encoderFormat = pContext->pWorkerContext->pConfig->m_Formats.GetData(nEncoder);

    // check input extensions
    CString szInputFileExt = ::GetFileExtension(szInputFile).MakeUpper();
    bool bIsValidEncoderInput = encoderFormat.IsValidInputExtension(szInputFileExt);

    // processing mode
    Mode nProcessingMode = Mode::None;
    if (bIsValidEncoderInput)
        nProcessingMode = Mode::Encode;
    else
        nProcessingMode = Mode::Transcode;

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

    CString szOrgInputFile = szInputFile;
    CString szOrgOutputFile = szOutputFile;

    if (nProcessingMode == Mode::Transcode)
    {
        int nDecoder = pContext->pWorkerContext->pConfig->m_Formats.GetDecoderByExtension(pContext->item->szExtension);
        if (nDecoder == -1)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find valid decoder by extension."));
            return false;
        }

        CFormat& decoderFormat = pContext->pWorkerContext->pConfig->m_Formats.GetData(nDecoder);

        bIsValidEncoderInput = encoderFormat.IsValidInputExtension(decoderFormat.szOutputExtension);
        if (bIsValidEncoderInput == false)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: decoder output not supported by encoder."));
            return false;
        }

        CString szDecoderExtension = decoderFormat.szOutputExtension;
        szOutputFile = szOutputFile + +_T(".") + szDecoderExtension.MakeLower();

        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Decoding..."));

        if (decoderFormat.nDefaultPreset >= decoderFormat.m_Presets.GetSize())
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find format preset."));
            return false;
        }

        try
        {
            CFileContext context;
            ToFileContext(context, pContext->pWorkerContext, decoderFormat, decoderFormat.nDefaultPreset, pContext->item->nId, szInputFile, szOutputFile);

            bool bResult = ::ConvertFile(&context);
            if (bResult == false)
            {
                if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szOutputFile);

                return false;
            }
        }
        catch (...)
        {
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szOutputFile);
            
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: exception thrown while converting file."));
            pContext->pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    if (pContext->pWorkerContext->bRunning == false)
        return false;

    if (nProcessingMode == Mode::Transcode)
    {
        szInputFile = szOutputFile;
        szOutputFile = szOrgOutputFile;
    }

    if ((nProcessingMode == Mode::Encode) || (nProcessingMode == Mode::Transcode))
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Encoding..."));

        if (pContext->item->nPreset >= encoderFormat.m_Presets.GetSize())
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find format preset."));
            return false;
        }

        try
        {
            CFileContext context;
            ToFileContext(context, pContext->pWorkerContext, encoderFormat, pContext->item->nPreset, pContext->item->nId, szInputFile, szOutputFile);

            bool bResult = ::ConvertFile(&context);

            if (nProcessingMode == Mode::Transcode)
                ::DeleteFile(szInputFile);

            if (bResult == true)
            {
                if (pContext->pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                    ::DeleteFile(szOrgInputFile);

                return true;
            }
            else
            {
                if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szOutputFile);

                return false;
            }
        }
        catch (...)
        {
            if (nProcessingMode == Mode::Transcode)
                ::DeleteFile(szInputFile);
            
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szOutputFile);
            
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: exception thrown while converting file."));
            pContext->pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    return false;
}
