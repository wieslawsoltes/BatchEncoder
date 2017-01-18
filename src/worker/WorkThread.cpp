// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "WorkThread.h"

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    // NOTE:
    // input and output filenames are UNICODE but
    // we can convert it to OS current code-page using CharToOem
    // this should work if UNICODE chars are in code-page of OS

    ::UpdatePath();

    // get handle of main dialog
    CBatchEncoderDlg *pDlg = (CBatchEncoderDlg *)lpParam;
    if (pDlg == NULL)
        return (DWORD)(-1);

    TCHAR szCommandLine[(64 * 1024)];
    ZeroMemory(szCommandLine, sizeof(szCommandLine));

    // get number of files in ListView
    int nItems = pDlg->m_Config.m_Items.GetSize();

    // get number of checked files in ListView
    int nTotalFiles = 0;
    int nProcessedFiles = 0;
    int nDoneWithoutError = 0;
    int nErrors = 0;
    for (int i = 0; i < nItems; i++)
    {
        if (pDlg->m_Config.m_Items.GetData(i).bChecked == TRUE)
            nTotalFiles++;
    }

    CTimeCount timeCount;
    timeCount.InitCounter();
    timeCount.StartCounter();

    // process all checked files
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = pDlg->m_Config.m_Items.GetData(i);

        // get next file name and check if we need to encode/decode/trans-code
        if (item.bChecked == true)
        {
            // update status-bar conversion status
            nProcessedFiles++;

            nErrors = (nProcessedFiles - 1) - nDoneWithoutError;
            CString szText;
            szText.Format(_T("Processing item %d of %d (%d Done, %d %s)"),
                nProcessedFiles,
                nTotalFiles,
                nDoneWithoutError,
                nErrors,
                ((nErrors == 0) || (nErrors > 1)) ? _T("Errors") : _T("Error"));

            VERIFY(pDlg->m_StatusBar.SetText(szText, 1, 0));

            // reset progress bar on start of next file
            pDlg->m_FileProgress.SetPos(0);

            // scroll list to ensure the item is visible
            pDlg->m_LstInputItems.EnsureVisible(i, FALSE);

            // TODO: 
            // when trans-coding on decode pass sum the decode+encode passes as one
            // in the total progress-bar calculations

            // processing modes:
            // 0. encoding - input is WAV file and we only have to encode
            // 1. decoding - we only have to decode input file to WAV
            // 2. trans-coding - we need to decode input file to encoder stdin stream
            int nProcessingMode = -1;

            int nIntputFormat = pDlg->m_Config.m_Formats.GetInFormatByExt(item.szExtension);
            CFormat& intputFormat = pDlg->m_Config.m_Formats.GetData(nIntputFormat);
            CPreset& inputPreset = intputFormat.m_Presets.GetData(intputFormat.nDefaultPreset);

            int nOutputFormat = pDlg->m_Config.m_Formats.GetOutFormatById(item.szFormatId);
            CFormat& outputFormat = pDlg->m_Config.m_Formats.GetData(nOutputFormat);
            CPreset& outputPreset = outputFormat.m_Presets.GetData(item.nPreset);

            // get full file path
            CString szInputFile = item.szPath;

            CString szOutPath;

            // output path is same as input file path
            if (pDlg->m_Config.m_Options.bOutputPathChecked == false)
            {
                szOutPath = szInputFile;
                CString szToRemove = pDlg->m_Config.m_Items.GetFileName(szInputFile);
                int nNewLenght = szOutPath.GetLength() - szToRemove.GetLength();
                szOutPath.Truncate(nNewLenght);
            }
            else
            {
                szOutPath = pDlg->m_Config.m_Options.szOutputPath;
            }

            // setup decoder steps:
            // 1. set encoder exe path
            // 2. set encoder options string
            CString szDecoderExePath;
            CString szDecoderOptions;

            szDecoderExePath = intputFormat.szPath;
            szDecoderOptions = inputPreset.szOptions;

            // get only output filename
            CString szName = item.szName;

            // setup encoder steps:
            // 1. add extension to output filename
            // 2. set encoder exe path
            // 3. set encoder options string
            CString szEncoderExePath;
            CString szEncoderOptions;

            szEncoderExePath = outputFormat.szPath;
            szEncoderOptions = outputPreset.szOptions;
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

            // set proper processing mode
            nProcessingMode = 1;
            if (isInputWav)
                nProcessingMode = 0;

            if (nProcessingMode == 1)
                nProcessingMode = 2;

            // 1. Input is WAV, [Output is WAV], No Resampling              = Copy Input File (using SSRC without options)
            // 2. Input is WAV, [Output is WAV], Resampling                 = Encode Input File (using SSRC)
            // 3. Input requires decoding, [Output is WAV], No Resampling   = Decode Input File (using Decoder)
            // 4. Input requires decoding, [Output is WAV], Resampling      = Decode and Encode (using Decoder and SSRC)
            if (isOutputWav)
            {
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
                if (pDlg->m_Config.m_Options.bForceConsoleWindow == false)
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
                nTool = pDlg->m_Config.m_Formats.GetFormatById(intputFormat.szId);

                lstrcpy(szCommandLine, csExecute.GetBuffer(csExecute.GetLength()));

                pDlg->m_LstInputItems.SetItemText(i, 5, _T("--:--"));
                pDlg->m_LstInputItems.SetItemText(i, 6, _T("Decoding..."));

                ConvertContext context;

                context.pDlg = pDlg;
                context.szInputFile = szOrgInputFile;
                context.szOutputFile = szOrgOutputFile;
                context.szCommandLine = szCommandLine;
                context.nIndex = i;
                context.bDecode = bDecode;
                context.nTool = nTool;
                context.bUseReadPipes = bUseInPipesDec;
                context.bUseWritePipes = bUseOutPipesDec;

                // TODO: when decoding in nProcessingMode == 2 don't show time stats
                if (::ConvertFile(&context) == true)
                {
                    if (nProcessingMode == 1)
                        nDoneWithoutError++;

                    if (pDlg->m_Config.m_Options.bDeleteSourceFiles == true)
                        ::DeleteFile(szOrgInputFile);

                    if (nProcessingMode == 1)
                        pDlg->m_LstInputItems.SetCheck(i, FALSE);
                }
                else
                {
                    // delete output file on error
                    if (pDlg->m_Config.m_Options.bDeleteOnError == true)
                        ::DeleteFile(szOutputFile);

                    if (pDlg->bRunning == false)
                        break;

                    // stop conversion process on error
                    if (pDlg->m_Config.m_Options.bStopOnErrors == true)
                        break;

                    // in processing mode 2 we are skipping to next file
                    // no encoding is done when decoding failed
                    continue;
                }
            }

            if (pDlg->bRunning == false)
                break;

            if (pDlg->m_Config.m_Options.bForceConsoleWindow == false)
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
                nTool = pDlg->m_Config.m_Formats.GetFormatById(outputFormat.szId);

                lstrcpy(szCommandLine, csExecute.GetBuffer(csExecute.GetLength()));

                pDlg->m_LstInputItems.SetItemText(i, 5, _T("--:--"));
                pDlg->m_LstInputItems.SetItemText(i, 6, _T("Encoding..."));

                ConvertContext context;

                context.pDlg = pDlg;
                context.szInputFile = szOrgInputFile;
                context.szOutputFile = szOrgOutputFile;
                context.szCommandLine = szCommandLine;
                context.nIndex = i;
                context.bDecode = bDecode;
                context.nTool = nTool;
                context.bUseReadPipes = bUseInPipesEnc;
                context.bUseWritePipes = bUseOutPipesEnc;

                if (::ConvertFile(&context) == true)
                {
                    nDoneWithoutError++;

                    if (pDlg->m_Config.m_Options.bDeleteSourceFiles == true)
                        ::DeleteFile(szOrgInputFile);

                    pDlg->m_LstInputItems.SetCheck(i, FALSE);
                }
                else
                {
                    // delete output file on error
                    if (pDlg->m_Config.m_Options.bDeleteOnError == true)
                        ::DeleteFile(szOutputFile);

                    // stop conversion process on error
                    if (pDlg->m_Config.m_Options.bStopOnErrors == true)
                        break;
                }

                // delete temporary file
                if (nProcessingMode == 2)
                    ::DeleteFile(szOrgInputFile);
            }

            if (pDlg->bRunning == false)
                break;
        }
    }

    timeCount.StopCounter();

    // show total time
    if (nProcessedFiles > 0)
    {
        CString szText;
        szText.Format(_T("Done in %s"), ::FormatTime(timeCount.GetTime(), 3));
        pDlg->m_StatusBar.SetText(szText, 1, 0);
    }
    else
    {
        pDlg->m_StatusBar.SetText(_T(""), 1, 0);
    }

    pDlg->FinishConvert();

    // close this worker thread handle
    return ::CloseHandle(pDlg->hThread);
}
