﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "Strings.h"
#include "utilities\OutputPath.h"
#include "utilities\TimeCount.h"
#include "utilities\Utilities.h"
#include "FileToPipeWriter.h"
#include "PipeToFileWriter.h"
#include "PipeToStringWriter.h"
#include "LuaOutputParser.h"
#include "DebugOutputParser.h"
#include "ToolUtilities.h"
#include "Worker.h"

namespace worker
{
    bool CWorker::ConvertFileUsingConsole(IWorkerContext* pWorkerContext, CCommandLine &commandLine, util::CSynchronize &syncDown)
    {
        util::CProcess process;
        util::CPipe Stderr(true);
        util::CTimeCount timer;
        CLuaOutputParser parser;
        CPipeToStringWriter writer;

        if ((commandLine.bUseReadPipes == true) || (commandLine.bUseWritePipes == true))
        {
            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00120001, app::pszConvertConsole[0]));
            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
            return false;
        }

        // create pipes for stderr
        if (Stderr.Create() == false)
        {
            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00120002, app::pszConvertConsole[1]));
            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
            return false;
        }

        // duplicate stderr read pipe handle to prevent child process from closing the pipe
        if (Stderr.DuplicateRead() == false)
        {
            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00120003, app::pszConvertConsole[2]));
            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
            return false;
        }

        // connect pipes to process
        process.ConnectStdInput(nullptr);
        process.ConnectStdOutput(Stderr.hWrite);
        process.ConnectStdError(Stderr.hWrite);

        syncDown.Wait();
        ::SetCurrentDirectory(app::m_App.szSettingsPath);

        timer.Start();
        if (process.Start(commandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (pWorkerContext->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;
                int nTool = m_Utilities.FindTool(pWorkerContext->pConfig->m_Tools, commandLine.pFormat->szId);
                if (nTool >= 0)
                {
                    config::CTool& tool = pWorkerContext->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, pWorkerContext->pConfig,
                        [this, pWorkerContext, &commandLine](int nIndex, CString szStatus)
                    {
                        pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(app::m_App.szSettingsPath);

                        if (process.Start(commandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }
            }

            if (bFailed == true)
            {
                syncDown.Release();

                timer.Stop();

                Stderr.CloseRead();
                Stderr.CloseWrite();

                CString szStatus;
                szStatus.Format(pWorkerContext->pConfig->GetString(0x00120004, app::pszConvertConsole[3]), ::GetLastError());

                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, szStatus);
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }
        }

        syncDown.Release();

        // close unused pipe handle
        Stderr.CloseWrite();

        // console progress loop
        if (writer.ReadLoop(pWorkerContext, commandLine, Stderr, parser, syncDown) == false)
        {
            timer.Stop();
            Stderr.CloseRead();
            process.Stop(false, commandLine.pFormat->nExitCodeSuccess);
            return false;
        }

        timer.Stop();
        Stderr.CloseRead();
        if (process.Stop(parser.nProgress == 100, commandLine.pFormat->nExitCodeSuccess) == false)
            parser.nProgress = -1;

        if (parser.nProgress != 100)
        {
            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00120005, app::pszConvertConsole[4]));
            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
            return false;
        }
        else
        {
            pWorkerContext->Status(commandLine.nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->pConfig->GetString(0x00120006, app::pszConvertConsole[5]));
            pWorkerContext->Callback(commandLine.nItemId, 100, true, false);
            return true;
        }
    }

    bool CWorker::ConvertFileUsingPipes(IWorkerContext* pWorkerContext, CCommandLine &commandLine, util::CSynchronize &syncDown)
    {
        util::CProcess process;
        util::CPipe Stdin(true);
        util::CPipe Stdout(true);
        CFileToPipeWriter readContext;
        CPipeToFileWriter writeContext;
        util::CThread readThread;
        util::CThread writeThread;
        int nProgress = 0;
        util::CTimeCount timer;

        if ((commandLine.bUseReadPipes == false) && (commandLine.bUseWritePipes == false))
        {
            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130001, app::pszConvertPipes[0]));
            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
            return false;
        }
 
        if (commandLine.bUseReadPipes == true)
        {
            // create pipes for stdin
            if (Stdin.Create() == false)
            {
                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130002, app::pszConvertPipes[1]));
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }

            // set stdin write pipe inherit flag
            if (Stdin.InheritWrite() == false)
            {
                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130003, app::pszConvertPipes[2]));
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }
        }

        if (commandLine.bUseWritePipes == true)
        {
            // create pipes for stdout
            if (Stdout.Create() == false)
            {
                if (commandLine.bUseReadPipes == true)
                {
                    Stdin.CloseRead();
                    Stdin.CloseWrite();
                }

                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130004, app::pszConvertPipes[3]));
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }

            // set stdout read pipe inherit flag
            if (Stdout.InheritRead() == false)
            {
                if (commandLine.bUseReadPipes == true)
                {
                    Stdin.CloseRead();
                    Stdin.CloseWrite();
                }

                Stdout.CloseRead();
                Stdout.CloseWrite();

                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130005, app::pszConvertPipes[4]));
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }
        }

        // connect pipes to process
        if ((commandLine.bUseReadPipes == true) && (commandLine.bUseWritePipes == false))
        {
            process.ConnectStdInput(Stdin.hRead);
            process.ConnectStdOutput(GetStdHandle(STD_OUTPUT_HANDLE));
            process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
        }
        else if ((commandLine.bUseReadPipes == false) && (commandLine.bUseWritePipes == true))
        {
            process.ConnectStdInput(GetStdHandle(STD_INPUT_HANDLE));
            process.ConnectStdOutput(Stdout.hWrite);
            process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
        }
        else if ((commandLine.bUseReadPipes == true) && (commandLine.bUseWritePipes == true))
        {
            process.ConnectStdInput(Stdin.hRead);
            process.ConnectStdOutput(Stdout.hWrite);
            process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
        }

        syncDown.Wait();
        ::SetCurrentDirectory(app::m_App.szSettingsPath);

        timer.Start();
        if (process.Start(commandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (pWorkerContext->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;
                int nTool = m_Utilities.FindTool(pWorkerContext->pConfig->m_Tools, commandLine.pFormat->szId);
                if (nTool >= 0)
                {
                    config::CTool& tool = pWorkerContext->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, pWorkerContext->pConfig,
                        [this, pWorkerContext, &commandLine](int nIndex, CString szStatus)
                    {
                        pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(app::m_App.szSettingsPath);

                        if (process.Start(commandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }
            }

            if (bFailed == true)
            {
                syncDown.Release();

                timer.Stop();

                if (commandLine.bUseReadPipes == true)
                {
                    Stdin.CloseRead();
                    Stdin.CloseWrite();
                }

                if (commandLine.bUseWritePipes == true)
                {
                    Stdout.CloseRead();
                    Stdout.CloseWrite();
                }

                CString szStatus;
                szStatus.Format(pWorkerContext->pConfig->GetString(0x00130006, app::pszConvertPipes[5]), ::GetLastError());

                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, szStatus);
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }
        }

        syncDown.Release();

        // close unused pipe handles
        if (commandLine.bUseReadPipes == true)
            Stdin.CloseRead();

        if (commandLine.bUseWritePipes == true)
            Stdout.CloseWrite();

        // create read thread
        if (commandLine.bUseReadPipes == true)
        {
            readContext.bError = false;
            readContext.bFinished = false;
            readContext.szFileName = commandLine.szInputFile;
            readContext.nIndex = commandLine.nItemId;

            if (readThread.Start([this, pWorkerContext, &readContext, &Stdin]() { readContext.ReadLoop(pWorkerContext, Stdin); }) == false)
            {
                timer.Stop();

                process.Stop(false, commandLine.pFormat->nExitCodeSuccess);

                Stdin.CloseWrite();

                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130007, app::pszConvertPipes[6]));
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }

            // wait for read thread to finish
            if (commandLine.bUseWritePipes == false)
            {
                readThread.Wait();

                // NOTE: Handle is closed in ReadThread.
                //Stdin.CloseWrite();

                // close read thread handle
                readThread.Close();

                // check for result from read thread
                if ((readContext.bError == false) && (readContext.bFinished == true))
                    nProgress = 100;
                else
                    nProgress = -1;
            }
        }

        // create write thread
        if (commandLine.bUseWritePipes == true)
        {
            writeContext.bError = false;
            writeContext.bFinished = false;
            writeContext.szFileName = commandLine.szOutputFile;
            writeContext.nIndex = commandLine.nItemId;

            if (writeThread.Start([this, pWorkerContext, &writeContext, &Stdout]() { writeContext.WriteLoop(pWorkerContext, Stdout); }) == false)
            {
                timer.Stop();

                process.Stop(false, commandLine.pFormat->nExitCodeSuccess);

                Stdout.CloseRead();

                pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130008, app::pszConvertPipes[7]));
                pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                return false;
            }

            if (commandLine.bUseReadPipes == true)
            {
                // wait for read thread to finish
                readThread.Wait();

                // NOTE: Handle is closed in ReadThread.
                //Stdin.CloseWrite();

                // close read thread handle
                readThread.Close();

                if ((readContext.bError == true) || (readContext.bFinished == false))
                {
                    // close write thread handle
                    Stdout.CloseRead();

                    // read thread failed so terminate write thread
                    writeThread.Terminate();
                    writeThread.Close();
                }
                else
                {
                    // wait for write thread to finish
                    writeThread.Wait();

                    // close write thread handle
                    Stdout.CloseRead();

                    // close write thread handle
                    writeThread.Close();
                }

                // check for result from read thread
                if ((readContext.bError == false) && (readContext.bFinished == true)
                    && (writeContext.bError == false) && (writeContext.bFinished == true))
                    nProgress = 100;
                else
                    nProgress = -1;
            }
            else
            {
                // wait for write thread to finish
                writeThread.Wait();

                // close write thread handle
                Stdout.CloseRead();

                // close write thread handle
                writeThread.Close();

                // check for result from write thread
                if ((writeContext.bError == false) && (writeContext.bFinished == true))
                    nProgress = 100;
                else
                    nProgress = -1;
            }
        }

        timer.Stop();

        if (process.Stop(nProgress == 100, commandLine.pFormat->nExitCodeSuccess) == false)
            nProgress = -1;

        if (nProgress != 100)
        {
            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130009, app::pszConvertPipes[8]));
            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
            return false;
        }
        else
        {
            pWorkerContext->Status(commandLine.nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->pConfig->GetString(0x0013000B, app::pszConvertPipes[10]));
            pWorkerContext->Callback(commandLine.nItemId, 100, true, false);
            return true;
        }
    }

    bool CWorker::ConvertFileUsingOnlyPipes(IWorkerContext* pWorkerContext, CCommandLine &decoderCommandLine, CCommandLine &encoderCommandLine, util::CSynchronize &syncDown)
    {
        util::CProcess decoderProcess;
        util::CProcess encoderProcess;
        util::CPipe Stdin(true);
        util::CPipe Stdout(true);
        util::CPipe Bridge(true);
        CFileToPipeWriter readContext;
        CPipeToFileWriter writeContext;
        util::CThread readThread;
        util::CThread writeThread;
        int nProgress = 0;
        util::CTimeCount timer;

        // create pipes for stdin
        if (Stdin.Create() == false)
        {
            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130002, app::pszConvertPipes[1]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // set stdin write pipe inherit flag
        if (Stdin.InheritWrite() == false)
        {
            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130003, app::pszConvertPipes[2]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // create pipes for stdout
        if (Stdout.Create() == false)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();

            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130004, app::pszConvertPipes[3]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // set stdout read pipe inherit flag
        if (Stdout.InheritRead() == false)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();

            Stdout.CloseRead();
            Stdout.CloseWrite();

            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130005, app::pszConvertPipes[4]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // create pipes for processes bridge
        if (Bridge.Create() == false)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();

            Stdout.CloseRead();
            Stdout.CloseWrite();

            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0013000A, app::pszConvertPipes[9]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // connect pipes to decoder process
        decoderProcess.ConnectStdInput(Stdin.hRead);
        decoderProcess.ConnectStdOutput(Bridge.hWrite);
        decoderProcess.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));

        // connect pipes to encoder process
        encoderProcess.ConnectStdInput(Bridge.hRead);
        encoderProcess.ConnectStdOutput(Stdout.hWrite);
        encoderProcess.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));

        timer.Start();

        syncDown.Wait();
        ::SetCurrentDirectory(app::m_App.szSettingsPath);

        // create decoder process
        if (decoderProcess.Start(decoderCommandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (pWorkerContext->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;
                int nTool = m_Utilities.FindTool(pWorkerContext->pConfig->m_Tools, decoderCommandLine.pFormat->szId);
                if (nTool >= 0)
                {
                    config::CTool& tool = pWorkerContext->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, pWorkerContext->pConfig,
                        [this, pWorkerContext, &decoderCommandLine](int nIndex, CString szStatus)
                    {
                        pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(app::m_App.szSettingsPath);

                        if (decoderProcess.Start(decoderCommandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }
            }

            if (bFailed == true)
            {
                syncDown.Release();

                timer.Stop();

                Stdin.CloseRead();
                Stdin.CloseWrite();

                Stdout.CloseRead();
                Stdout.CloseWrite();

                Bridge.CloseRead();
                Bridge.CloseWrite();

                CString szStatus;
                szStatus.Format(pWorkerContext->pConfig->GetString(0x00130006, app::pszConvertPipes[5]), ::GetLastError());

                pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, szStatus);
                pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
                return false;
            }
        }

        ::SetCurrentDirectory(app::m_App.szSettingsPath);

        // create encoder process
        if (encoderProcess.Start(encoderCommandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (pWorkerContext->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;
                int nTool = m_Utilities.FindTool(pWorkerContext->pConfig->m_Tools, encoderCommandLine.pFormat->szId);
                if (nTool >= 0)
                {
                    config::CTool& tool = pWorkerContext->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, pWorkerContext->pConfig,
                        [this, pWorkerContext, &encoderCommandLine](int nIndex, CString szStatus)
                    {
                        pWorkerContext->Status(encoderCommandLine.nItemId, app::pszDefaulTime, szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(app::m_App.szSettingsPath);

                        if (encoderProcess.Start(encoderCommandLine.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }
            }

            if (bFailed == true)
            {
                syncDown.Release();

                timer.Stop();

                decoderProcess.Stop(false, decoderCommandLine.pFormat->nExitCodeSuccess);

                Stdin.CloseRead();
                Stdin.CloseWrite();

                Stdout.CloseRead();
                Stdout.CloseWrite();

                Bridge.CloseRead();
                Bridge.CloseWrite();

                CString szStatus;
                szStatus.Format(pWorkerContext->pConfig->GetString(0x00130006, app::pszConvertPipes[5]), ::GetLastError());

                pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, szStatus);
                pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
                return false;
            }
        }

        syncDown.Release();

        // close unused pipe handles
        Stdin.CloseRead();
        Stdout.CloseWrite();
        Bridge.CloseWrite();
        Bridge.CloseRead();

        // create read thread
        readContext.bError = false;
        readContext.bFinished = false;
        readContext.szFileName = decoderCommandLine.szInputFile;
        readContext.nIndex = decoderCommandLine.nItemId;

        if (readThread.Start([this, pWorkerContext, &readContext, &Stdin]() { readContext.ReadLoop(pWorkerContext, Stdin); }) == false)
        {
            timer.Stop();

            Stdin.CloseWrite();

            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130007, app::pszConvertPipes[6]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // create write thread
        writeContext.bError = false;
        writeContext.bFinished = false;
        writeContext.szFileName = encoderCommandLine.szOutputFile;
        writeContext.nIndex = encoderCommandLine.nItemId;

        if (writeThread.Start([this, pWorkerContext, &writeContext, &Stdout]() { writeContext.WriteLoop(pWorkerContext, Stdout); }) == false)
        {
            timer.Stop();

            Stdout.CloseRead();

            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130008, app::pszConvertPipes[7]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }

        // wait for read thread to finish after write thread finished
        readThread.Wait();

        // NOTE: Handle is closed in ReadThread.
        //Stdin.CloseWrite();

        // close read thread handle
        readThread.Close();

        if ((readContext.bError == true) || (readContext.bFinished == false))
        {
            Stdout.CloseRead();

            // read thread failed so terminate write thread
            writeThread.Terminate();
            writeThread.Close();
        }
        else
        {
            Stdout.CloseRead();

            // wait for write thread to finish
            writeThread.Wait();

            // close write thread handle
            writeThread.Close();
        }

        // check for result from read and write thread
        if ((readContext.bError == false) && (readContext.bFinished == true)
            && (writeContext.bError == false) && (writeContext.bFinished == true))
            nProgress = 100;
        else
            nProgress = -1;

        timer.Stop();

        if (decoderProcess.Stop(nProgress == 100, decoderCommandLine.pFormat->nExitCodeSuccess) == false)
            nProgress = -1;

        if (encoderProcess.Stop(nProgress == 100, encoderCommandLine.pFormat->nExitCodeSuccess) == false)
            nProgress = -1;

        if (nProgress != 100)
        {
            pWorkerContext->Status(decoderCommandLine.nItemId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00130009, app::pszConvertPipes[8]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, -1, true, true);
            return false;
        }
        else
        {
            pWorkerContext->Status(decoderCommandLine.nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->pConfig->GetString(0x0013000B, app::pszConvertPipes[10]));
            pWorkerContext->Callback(decoderCommandLine.nItemId, 100, true, false);
            return true;
        }
    }

    bool CWorker::ConvertItem(IWorkerContext* pWorkerContext, config::CItem& item, util::CSynchronize &syncDir, util::CSynchronize &syncDown)
    {
        config::CFormat *pEncFormat = nullptr;
        config::CFormat *pDecFormat = nullptr;
        CString szEncInputFile;
        CString szEncOutputFile;
        CString szDecInputFile;
        CString szDecOutputFile;
        util::COutputPath m_Output;
        CCommandLine decoderCommandLine;
        CCommandLine encoderCommandLine;

        // prepare encoder
        config::CPath& path = item.m_Paths.Get(0);

        szEncInputFile = path.szPath;
        if (util::FileExists(szEncInputFile) == false)
        {
            pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140001, app::pszConvertItem[0]));
            return false;
        }

        int nEncoder = pWorkerContext->pConfig->m_Formats.GetFormatById(item.szFormatId);
        if (nEncoder == -1)
        {
            pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140002, app::pszConvertItem[1]));
            return false;
        }

        pEncFormat = &pWorkerContext->pConfig->m_Formats.Get(nEncoder);

        if (item.nPreset >= pEncFormat->m_Presets.Count())
        {
            pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140003, app::pszConvertItem[2]));
            return false;
        }

        bool bIsValidEncoderInput = pEncFormat->IsValidInputExtension(util::GetFileExtension(szEncInputFile));

        m_Output.Validate(pWorkerContext->pConfig->m_Options.szOutputPath);
        szEncOutputFile = m_Output.CreateFilePath(
            pWorkerContext->pConfig->m_Options.szOutputPath,
            szEncInputFile,
            item.szName,
            pEncFormat->szOutputExtension);

        if (pWorkerContext->pConfig->m_Options.bOverwriteExistingFiles == false)
        {
            if (util::FileExists(szEncOutputFile) == true)
            {
                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140010, app::pszConvertItem[15]));
                return false;
            }
        }

        // create output path
        if (syncDir.Wait() == true)
        {
            if (m_Output.CreateOutputPath(szEncOutputFile) == false)
            {
                syncDir.Release();
                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000F, app::pszConvertItem[14]));
                return false;
            }
        }
        else
        {
            pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000F, app::pszConvertItem[14]));
            return false;
        }

        if (syncDir.Release() == false)
        {
            pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000F, app::pszConvertItem[14]));
            return false;
        }

        ::SetCurrentDirectory(app::m_App.szSettingsPath);

        // prepare decoder
        if (bIsValidEncoderInput == false)
        {
            int nDecoder = pWorkerContext->pConfig->m_Formats.GetDecoderByExtensionAndFormat(item.szExtension, pEncFormat);
            if (nDecoder == -1)
            {
                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140004, app::pszConvertItem[3]));
                return false;
            }

            pDecFormat = &pWorkerContext->pConfig->m_Formats.Get(nDecoder);

            if (pDecFormat->nDefaultPreset >= pDecFormat->m_Presets.Count())
            {
                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140005, app::pszConvertItem[4]));
                return false;
            }

            bool bIsValidDecoderOutput = pEncFormat->IsValidInputExtension(pDecFormat->szOutputExtension);
            if (bIsValidDecoderOutput == false)
            {
                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140006, app::pszConvertItem[5]));
                return false;
            }

            szDecInputFile = szEncInputFile;
            szDecOutputFile.Format(_T("%s%s.%s"),
                util::GetFilePath(szEncOutputFile),
                util::GenerateUuidString(),
                CString(pDecFormat->szOutputExtension).MakeLower());
        }

        if (bIsValidEncoderInput == false)
        {
            decoderCommandLine.Build(
                pDecFormat,
                pDecFormat->nDefaultPreset,
                item.nId,
                szDecInputFile,
                szDecOutputFile,
                pDecFormat->bPipeInput,
                pDecFormat->bPipeOutput,
                _T(""));
        }

        encoderCommandLine.Build(
            pEncFormat,
            item.nPreset,
            item.nId,
            bIsValidEncoderInput == true ? szEncInputFile : szDecOutputFile,
            szEncOutputFile,
            pEncFormat->bPipeInput,
            pEncFormat->bPipeOutput,
            item.szOptions);

        if (bIsValidEncoderInput == false
            && decoderCommandLine.bUseReadPipes == true
            && decoderCommandLine.bUseWritePipes == true
            && encoderCommandLine.bUseReadPipes == true
            && encoderCommandLine.bUseWritePipes == true)
        {
            // trans-code
            try
            {
                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000C, app::pszConvertItem[11]));

                item.ResetProgress();

                bool bResult = ConvertFileUsingOnlyPipes(pWorkerContext, decoderCommandLine, encoderCommandLine, syncDown);
                if (bResult == true)
                {
                    if (pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                        ::DeleteFile(szEncInputFile);

                    return true;
                }
                else
                {
                    if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szEncOutputFile);

                    return false;
                }
            }
            catch (...)
            {
                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile);

                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000E, app::pszConvertItem[13]));
                pWorkerContext->Callback(item.nId, -1, true, true);
            }
        }
        else
        {
            // decode
            if (bIsValidEncoderInput == false)
            {
                try
                {
                    pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140007, app::pszConvertItem[6]));

                    item.ResetProgress();

                    bool bResult = false;
                    if ((decoderCommandLine.bUseReadPipes == false) && (decoderCommandLine.bUseWritePipes == false))
                        bResult = ConvertFileUsingConsole(pWorkerContext, decoderCommandLine, syncDown);
                    else
                        bResult = ConvertFileUsingPipes(pWorkerContext, decoderCommandLine, syncDown);
                    if (bResult == false)
                    {
                        if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szDecOutputFile);

                        return false;
                    }

                    if (util::FileExists(szDecOutputFile) == false)
                    {
                        pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140008, app::pszConvertItem[7]));
                        return false;
                    }
                }
                catch (...)
                {
                    if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szEncOutputFile);

                    pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x00140009, app::pszConvertItem[8]));
                    pWorkerContext->Callback(item.nId, -1, true, true);
                }
            }

            if (pWorkerContext->bRunning == false)
                return false;

            // encode
            try
            {
                if (pEncFormat->nType == 0)
                    pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000A, app::pszConvertItem[9]));
                else if (pEncFormat->nType == 1)
                    pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000B, app::pszConvertItem[10]));

                item.ResetProgress();

                bool bResult = false;
                if ((encoderCommandLine.bUseReadPipes == false) && (encoderCommandLine.bUseWritePipes == false))
                    bResult = ConvertFileUsingConsole(pWorkerContext, encoderCommandLine, syncDown);
                else
                    bResult = ConvertFileUsingPipes(pWorkerContext, encoderCommandLine, syncDown);
                if (bResult == true)
                {
                    if (bIsValidEncoderInput == false)
                        ::DeleteFile(szDecOutputFile);

                    if (pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                        ::DeleteFile(szEncInputFile);

                    return true;
                }
                else
                {
                    if (bIsValidEncoderInput == false)
                        ::DeleteFile(szDecOutputFile);

                    if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szEncOutputFile);

                    return false;
                }
            }
            catch (...)
            {
                if (bIsValidEncoderInput == false)
                    ::DeleteFile(szDecOutputFile);

                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile);

                pWorkerContext->Status(item.nId, app::pszDefaulTime, pWorkerContext->pConfig->GetString(0x0014000E, app::pszConvertItem[13]));
                pWorkerContext->Callback(item.nId, -1, true, true);
            }
        }

        return false;
    }

    bool CWorker::ConvertLoop(IWorkerContext* pWorkerContext, std::queue<config::CItem> &queue, util::CSynchronize &sync, util::CSynchronize &syncDir, util::CSynchronize &syncDown)
    {
        while (TRUE)
        {
            try
            {
                if (sync.Wait() == true)
                {
                    if (!queue.empty())
                    {
                        config::CItem item = queue.front();
                        queue.pop();

                        if (sync.Release() == false)
                            return false;

                        if (pWorkerContext->bRunning == false)
                            return false;

                        pWorkerContext->Next(item.nId);
                        if (ConvertItem(pWorkerContext, item, syncDir, syncDown) == true)
                        {
                            pWorkerContext->nDoneWithoutError++;
                        }
                        else
                        {
                            if (pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                                return false;
                        }

                        if (pWorkerContext->bRunning == false)
                            return false; 
                    }
                    else
                    {
                        if (sync.Release() == false)
                            return false;

                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
            catch (...)
            {
                return false;
            }
        }

        return true;
    }

    void CWorker::Convert(IWorkerContext* pWorkerContext)
    {
        std::queue<config::CItem> queue;
        util::CSynchronize sync;
        util::CSynchronize syncDir;
        util::CSynchronize syncDown;

        pWorkerContext->nTotalFiles = 0;
        pWorkerContext->nProcessedFiles = 0;
        pWorkerContext->nDoneWithoutError = 0;
        pWorkerContext->nErrors = 0;
        pWorkerContext->nLastItemId = -1;

        int nItems = pWorkerContext->pConfig->m_Items.Count();
        for (int i = 0; i < nItems; i++)
        {
            config::CItem& item = pWorkerContext->pConfig->m_Items.Get(i);
            if (item.bChecked == true)
            {
                item.ResetProgress();
                queue.push(item);
                pWorkerContext->nTotalFiles++;
            }
            else
            {
                item.nProgress = 100;
                item.nPreviousProgress = 100;
            }
        }

        pWorkerContext->nThreadCount = pWorkerContext->pConfig->m_Options.nThreadCount;
        if (pWorkerContext->nThreadCount < 1)
        {
            util::LogicalProcessorInformation info;
            if (util::GetLogicalProcessorInformation(&info) == 0)
                pWorkerContext->nThreadCount = info.processorCoreCount;
            else
                pWorkerContext->nThreadCount = 1;
        }

        pWorkerContext->Init();

        // single-threaded
        if (pWorkerContext->nThreadCount == 1)
        {
            ConvertLoop(pWorkerContext, queue, sync, syncDir, syncDown);
        }

        // multi-threaded
        if (pWorkerContext->nThreadCount > 1)
        {
            auto threads = std::make_unique<util::CThread[]>(pWorkerContext->nThreadCount);
            for (int i = 0; i < pWorkerContext->nThreadCount; i++)
            {
                auto entry = [this, pWorkerContext, &queue, &sync, &syncDir, &syncDown]()
                { 
                    this->ConvertLoop(pWorkerContext, queue, sync, syncDir, syncDown);
                };
                if (threads[i].Start(entry, true) == false)
                    break;

                if (threads[i].Resume() == false)
                    break;
            }

            for (int i = 0; i < pWorkerContext->nThreadCount; i++)
                threads[i].Wait();

            for (int i = 0; i < pWorkerContext->nThreadCount; i++)
                threads[i].Close();
        }

        pWorkerContext->Done();
        pWorkerContext->bDone = true;
    }
}
