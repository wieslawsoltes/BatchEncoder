// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "WorkThread.h"

bool ConvertItem(ItemContext* pContext)
{
    bool bSuccess = false;

    TCHAR szCommandLine[(64 * 1024)];
    ZeroMemory(szCommandLine, sizeof(szCommandLine));

    // TODO: 
    // when trans-coding on decode pass sum the decode+encode passes as one
    // in the total progress-bar calculations

    int nIntputFormat = pContext->pDlg->m_Config.m_Formats.GetInFormatByExt(pContext->item->szExtension);
    CFormat& intputFormat = pContext->pDlg->m_Config.m_Formats.GetData(nIntputFormat);
    CPreset& inputPreset = intputFormat.m_Presets.GetData(intputFormat.nDefaultPreset);

    int nOutputFormat = pContext->pDlg->m_Config.m_Formats.GetOutFormatById(pContext->item->szFormatId);
    CFormat& outputFormat = pContext->pDlg->m_Config.m_Formats.GetData(nOutputFormat);
    CPreset& outputPreset = outputFormat.m_Presets.GetData(pContext->item->nPreset);

    // get full file path
    CString szInputFile = pContext->item->szPath;

    CString szOutPath;

    // output path is same as input file path
    if (pContext->pDlg->m_Config.m_Options.bOutputPathChecked == false)
    {
        szOutPath = szInputFile;
        CString szToRemove = pContext->pDlg->m_Config.m_Items.GetFileName(szInputFile);
        int nNewLenght = szOutPath.GetLength() - szToRemove.GetLength();
        szOutPath.Truncate(nNewLenght);
    }
    else
    {
        szOutPath = pContext->pDlg->m_Config.m_Options.szOutputPath;
    }

    // setup decoder steps:
    // 1. set encoder exe path
    // 2. set encoder options string
    CString szDecoderExePath = intputFormat.szPath;
    CString szDecoderOptions = inputPreset.szOptions;

    // get only output filename
    CString szName = pContext->item->szName;

    // setup encoder steps:
    // 1. add extension to output filename
    // 2. set encoder exe path
    // 3. set encoder options string
    CString szEncoderExePath = outputFormat.szPath;
    CString szEncoderOptions = outputPreset.szOptions;

    szName = szName + _T(".") + outputFormat.szExtension.MakeLower();

    // set full path for output file
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

    bool isInputWav = intputFormat.szExtension.MakeUpper().Compare(_T("WAV")) == 0;
    bool isOutputWav = outputFormat.szExtension.MakeUpper().Compare(_T("WAV")) == 0;

    // processing modes:
    // 0. encoding - input is WAV file and we only have to encode
    // 1. decoding - we only have to decode input file to WAV
    // 2. trans-coding - we need to decode input file to encoder stdin stream

    int nProcessingMode = -1;

    if (isInputWav)
        nProcessingMode = 0;
    else
        nProcessingMode = 2;

    if (isOutputWav)
    {
        // TODO: special case for WAV and SSRC output is based only on options check

        // setting proper processing mode:
        // 1. Input is WAV, [Output is WAV], No Resampling              = Copy Input File (using SSRC without options)
        // 2. Input is WAV, [Output is WAV], Resampling                 = Encode Input File (using SSRC)
        // 3. Input requires decoding, [Output is WAV], No Resampling   = Decode Input File (using Decoder)
        // 4. Input requires decoding, [Output is WAV], Resampling      = Decode and Encode (using Decoder and SSRC)

        bool bNeedResampling = (szEncoderOptions.GetLength() > 0) ? true : false;

        // case 1
        if (isInputWav && (bNeedResampling == false))
            nProcessingMode = 0;

        // case 2
        if (isInputWav && (bNeedResampling == true))
            nProcessingMode = 0;

        // case 3
        if (!isInputWav && (bNeedResampling == false))
            nProcessingMode = 1;

        // case 4
        if (!isInputWav && (bNeedResampling == true))
            nProcessingMode = 2;
    }

    // build proper command-line depending on processing mode:
    CString csExecute;
    bool bDecode = false;
    int nTool = -1;
    bool bUseInPipesEnc = false;
    bool bUseOutPipesEnc = false;
    bool bUseInPipesDec = false;
    bool bUseOutPipesDec = false;

    CString szOrgInputFile = szInputFile;
    CString szOrgOutputFile = szOutputFile;

    // decode
    if ((nProcessingMode == 1) || (nProcessingMode == 2))
    {
        if (pContext->pDlg->m_Config.m_Options.bForceConsoleWindow == false)
        {
            // configure decoder input and output pipes
            bUseInPipesDec = intputFormat.bInput;
            bUseOutPipesDec = intputFormat.bOutput;
        }

        // input file is stdin
        if (bUseInPipesDec == true)
            szInputFile = _T("-");

        // output file is stdout
        if (bUseOutPipesDec == true)
            szOutputFile = _T("-");

        // TODO:
        // bUseOutPipes == true than handle szOutputFile same as input file
        if (nProcessingMode == 2)
            szOutputFile = szOutputFile + _T(".wav");

        // TODO: validate the command-line template

        // build full command line for decoder (DECODER-EXE + OPTIONS + INFILE + OUTFILE) 
        // this is basic model, some of encoder may have different command-line structure
        csExecute = intputFormat.szTemplate;
        csExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
        csExecute.Replace(_T("$EXE"), szDecoderExePath);
        csExecute.Replace(_T("$OPTIONS"), szDecoderOptions);
        csExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
        csExecute.Replace(_T("$INFILE"), szInputFile);
        csExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
        csExecute.Replace(_T("$OUTFILE"), szOutputFile);

        bDecode = true;
        nTool = pContext->pDlg->m_Config.m_Formats.GetFormatById(intputFormat.szId);

        lstrcpy(szCommandLine, csExecute.GetBuffer(csExecute.GetLength()));

        pContext->pDlg->m_LstInputItems.SetItemText(pContext->item->nId, 5, _T("--:--"));
        pContext->pDlg->m_LstInputItems.SetItemText(pContext->item->nId, 6, _T("Decoding..."));

        FileContext context;

        context.pDlg = pContext->pDlg;
        context.szInputFile = szOrgInputFile;
        context.szOutputFile = szOrgOutputFile;
        context.szCommandLine = szCommandLine;
        context.nIndex = pContext->item->nId;
        context.bDecode = bDecode;
        context.nTool = nTool;
        context.bUseReadPipes = bUseInPipesDec;
        context.bUseWritePipes = bUseOutPipesDec;

        // TODO: when decoding in nProcessingMode == 2 don't show time stats
        if (::ConvertFile(&context) == true)
        {
            if (nProcessingMode == 1)
                bSuccess = true;

            if (pContext->pDlg->m_Config.m_Options.bDeleteSourceFiles == true)
                ::DeleteFile(szOrgInputFile);

            if (nProcessingMode == 1)
            {
                pContext->pDlg->m_LstInputItems.SetCheck(pContext->item->nId, FALSE);
                pContext->item->bChecked = false;
            }
        }
        else
        {
            // delete output file on error
            if (pContext->pDlg->m_Config.m_Options.bDeleteOnError == true)
                ::DeleteFile(szOutputFile);

            if (pContext->pDlg->bRunning == false)
                return bSuccess;

            // stop conversion process on error
            if (pContext->pDlg->m_Config.m_Options.bStopOnErrors == true)
                return bSuccess;

            // in processing mode 2 we are skipping to next file
            // no encoding is done when decoding failed
            return bSuccess;
        }
    }

    if (pContext->pDlg->bRunning == false)
        return bSuccess;

    if (pContext->pDlg->m_Config.m_Options.bForceConsoleWindow == false)
    {
        // configure encoder input and output pipes
        bUseInPipesEnc = outputFormat.bInput;
        bUseOutPipesEnc = outputFormat.bOutput;
    }

    if (nProcessingMode == 0)
    {
        // input file is stdin
        if (bUseInPipesEnc == true)
            szInputFile = _T("-");

        // output file is stdout
        if (bUseOutPipesEnc == true)
            szOutputFile = _T("-");
    }

    if (nProcessingMode == 2)
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
    if ((nProcessingMode == 0) || (nProcessingMode == 2))
    {
        // TODO: validate the command-line template

        // build full command line for encoder (ENCODER-EXE + OPTIONS + INFILE + OUTFILE)
        // this is basic model, some of encoder may have different command-line structure
        csExecute = outputFormat.szTemplate;
        csExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
        csExecute.Replace(_T("$EXE"), szEncoderExePath);
        csExecute.Replace(_T("$OPTIONS"), szEncoderOptions);
        csExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
        csExecute.Replace(_T("$INFILE"), szInputFile);
        csExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
        csExecute.Replace(_T("$OUTFILE"), szOutputFile);

        bDecode = false;
        nTool = pContext->pDlg->m_Config.m_Formats.GetFormatById(outputFormat.szId);

        lstrcpy(szCommandLine, csExecute.GetBuffer(csExecute.GetLength()));

        pContext->pDlg->m_LstInputItems.SetItemText(pContext->item->nId, 5, _T("--:--"));
        pContext->pDlg->m_LstInputItems.SetItemText(pContext->item->nId, 6, _T("Encoding..."));

        FileContext context;

        context.pDlg = pContext->pDlg;
        context.szInputFile = szOrgInputFile;
        context.szOutputFile = szOrgOutputFile;
        context.szCommandLine = szCommandLine;
        context.nIndex = pContext->item->nId;
        context.bDecode = bDecode;
        context.nTool = nTool;
        context.bUseReadPipes = bUseInPipesEnc;
        context.bUseWritePipes = bUseOutPipesEnc;

        if (::ConvertFile(&context) == true)
        {
            bSuccess = true;

            if (pContext->pDlg->m_Config.m_Options.bDeleteSourceFiles == true)
                ::DeleteFile(szOrgInputFile);

            pContext->pDlg->m_LstInputItems.SetCheck(pContext->item->nId, FALSE);
            pContext->item->bChecked = false;
        }
        else
        {
            // delete output file on error
            if (pContext->pDlg->m_Config.m_Options.bDeleteOnError == true)
                ::DeleteFile(szOutputFile);
        }

        // delete temporary file
        if (nProcessingMode == 2)
            ::DeleteFile(szOrgInputFile);
    }

    return bSuccess;
}
