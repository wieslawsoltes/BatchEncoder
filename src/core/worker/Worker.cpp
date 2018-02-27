// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "utilities\TimeCount.h"
#include "utilities\Utilities.h"
#include "InputPath.h"
#include "OutputPath.h"
#include "FileToPipeWriter.h"
#include "PipeToFileWriter.h"
#include "PipeToStringWriter.h"
#include "LuaOutputParser.h"
#include "DebugOutputParser.h"
#include "ToolUtilities.h"
#include "Worker.h"

namespace worker
{
    bool CWorker::ConvertFileUsingConsole(IWorkerContext* ctx, CCommandLine &cl, util::CSynchronize &syncDown)
    {
        util::CProcess process;
        util::CPipe Stderr(true);
        util::CTimeCount timer;
        CLuaOutputParser parser;
        CPipeToStringWriter writer;

        if ((cl.bUseReadPipes == true) || (cl.bUseWritePipes == true))
        {
            ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00120001));
            ctx->Callback(cl.nItemId, -1, true, true);
            return false;
        }

        // create pipes for stderr
        if (Stderr.Create() == false)
        {
            ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00120002));
            ctx->Callback(cl.nItemId, -1, true, true);
            return false;
        }

        // duplicate stderr read pipe handle to prevent child process from closing the pipe
        if (Stderr.DuplicateRead() == false)
        {
            ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00120003));
            ctx->Callback(cl.nItemId, -1, true, true);
            return false;
        }

        // connect pipes to process
        process.ConnectStdInput(nullptr);
        process.ConnectStdOutput(Stderr.hWrite);
        process.ConnectStdError(Stderr.hWrite);

        syncDown.Wait();
        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

        timer.Start();
        if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;

                int nTool = -1;
                nTool = ctx->pConfig->m_Tools.GetToolByPath(cl.pFormat->szPath);
                if (nTool < 0)
                {
                    nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, cl.pFormat->szId);
                }

                if (nTool >= 0)
                {
                    config::CTool& tool = ctx->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                        [this, ctx, &cl](int nIndex, std::wstring szStatus)
                    {
                        ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

                        if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
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
                szStatus.Format(ctx->pConfig->GetString(0x00120004).c_str(), ::GetLastError());

                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }
        }

        syncDown.Release();

        // close unused pipe handle
        Stderr.CloseWrite();

        // console progress loop
        if (writer.ReadLoop(ctx, cl, Stderr, parser, syncDown) == false)
        {
            timer.Stop();
            Stderr.CloseRead();
            process.Stop(false, cl.pFormat->nExitCodeSuccess);
            return false;
        }

        timer.Stop();
        Stderr.CloseRead();
        if (process.Stop(parser.nProgress == 100, cl.pFormat->nExitCodeSuccess) == false)
            parser.nProgress = -1;

        if (parser.nProgress != 100)
        {
            ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00120005));
            ctx->Callback(cl.nItemId, -1, true, true);
            return false;
        }
        else
        {
            ctx->Status(cl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->pConfig->GetString(0x00120006));
            ctx->Callback(cl.nItemId, 100, true, false);
            return true;
        }
    }

    bool CWorker::ConvertFileUsingPipes(IWorkerContext* ctx, CCommandLine &cl, util::CSynchronize &syncDown)
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

        if ((cl.bUseReadPipes == false) && (cl.bUseWritePipes == false))
        {
            ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130001));
            ctx->Callback(cl.nItemId, -1, true, true);
            return false;
        }
 
        if (cl.bUseReadPipes == true)
        {
            // create pipes for stdin
            if (Stdin.Create() == false)
            {
                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130002));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }

            // set stdin write pipe inherit flag
            if (Stdin.InheritWrite() == false)
            {
                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130003));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }
        }

        if (cl.bUseWritePipes == true)
        {
            // create pipes for stdout
            if (Stdout.Create() == false)
            {
                if (cl.bUseReadPipes == true)
                {
                    Stdin.CloseRead();
                    Stdin.CloseWrite();
                }

                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130004));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }

            // set stdout read pipe inherit flag
            if (Stdout.InheritRead() == false)
            {
                if (cl.bUseReadPipes == true)
                {
                    Stdin.CloseRead();
                    Stdin.CloseWrite();
                }

                Stdout.CloseRead();
                Stdout.CloseWrite();

                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130005));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }
        }

        // connect pipes to process
        if ((cl.bUseReadPipes == true) && (cl.bUseWritePipes == false))
        {
            process.ConnectStdInput(Stdin.hRead);
            process.ConnectStdOutput(GetStdHandle(STD_OUTPUT_HANDLE));
            process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
        }
        else if ((cl.bUseReadPipes == false) && (cl.bUseWritePipes == true))
        {
            process.ConnectStdInput(GetStdHandle(STD_INPUT_HANDLE));
            process.ConnectStdOutput(Stdout.hWrite);
            process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
        }
        else if ((cl.bUseReadPipes == true) && (cl.bUseWritePipes == true))
        {
            process.ConnectStdInput(Stdin.hRead);
            process.ConnectStdOutput(Stdout.hWrite);
            process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
        }

        syncDown.Wait();
        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

        timer.Start();
        if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;

                int nTool = -1;
                nTool = ctx->pConfig->m_Tools.GetToolByPath(cl.pFormat->szPath);
                if (nTool < 0)
                {
                    nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, cl.pFormat->szId);
                }

                if (nTool >= 0)
                {
                    config::CTool& tool = ctx->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                        [this, ctx, &cl](int nIndex, std::wstring szStatus)
                    {
                        ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

                        if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
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

                if (cl.bUseReadPipes == true)
                {
                    Stdin.CloseRead();
                    Stdin.CloseWrite();
                }

                if (cl.bUseWritePipes == true)
                {
                    Stdout.CloseRead();
                    Stdout.CloseWrite();
                }

                CString szStatus;
                szStatus.Format(ctx->pConfig->GetString(0x00130006).c_str(), ::GetLastError());

                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }
        }

        syncDown.Release();

        // close unused pipe handles
        if (cl.bUseReadPipes == true)
            Stdin.CloseRead();

        if (cl.bUseWritePipes == true)
            Stdout.CloseWrite();

        // create read thread
        if (cl.bUseReadPipes == true)
        {
            readContext.bError = false;
            readContext.bFinished = false;
            readContext.szFileName = cl.szInputFile;
            readContext.nIndex = cl.nItemId;

            if (readThread.Start([this, ctx, &readContext, &Stdin]() { readContext.ReadLoop(ctx, Stdin); }) == false)
            {
                timer.Stop();

                process.Stop(false, cl.pFormat->nExitCodeSuccess);

                Stdin.CloseWrite();

                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130007));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }

            // wait for read thread to finish
            if (cl.bUseWritePipes == false)
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
        if (cl.bUseWritePipes == true)
        {
            writeContext.bError = false;
            writeContext.bFinished = false;
            writeContext.szFileName = cl.szOutputFile;
            writeContext.nIndex = cl.nItemId;

            if (writeThread.Start([this, ctx, &writeContext, &Stdout]() { writeContext.WriteLoop(ctx, Stdout); }) == false)
            {
                timer.Stop();

                process.Stop(false, cl.pFormat->nExitCodeSuccess);

                Stdout.CloseRead();

                ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130008));
                ctx->Callback(cl.nItemId, -1, true, true);
                return false;
            }

            if (cl.bUseReadPipes == true)
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

        if (process.Stop(nProgress == 100, cl.pFormat->nExitCodeSuccess) == false)
            nProgress = -1;

        if (nProgress != 100)
        {
            ctx->Status(cl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130009));
            ctx->Callback(cl.nItemId, -1, true, true);
            return false;
        }
        else
        {
            ctx->Status(cl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->pConfig->GetString(0x0013000B));
            ctx->Callback(cl.nItemId, 100, true, false);
            return true;
        }
    }

    bool CWorker::ConvertFileUsingOnlyPipes(IWorkerContext* ctx, CCommandLine &dcl, CCommandLine &ecl, util::CSynchronize &syncDown)
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
            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130002));
            ctx->Callback(dcl.nItemId, -1, true, true);
            return false;
        }

        // set stdin write pipe inherit flag
        if (Stdin.InheritWrite() == false)
        {
            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130003));
            ctx->Callback(dcl.nItemId, -1, true, true);
            return false;
        }

        // create pipes for stdout
        if (Stdout.Create() == false)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();

            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130004));
            ctx->Callback(dcl.nItemId, -1, true, true);
            return false;
        }

        // set stdout read pipe inherit flag
        if (Stdout.InheritRead() == false)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();

            Stdout.CloseRead();
            Stdout.CloseWrite();

            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130005));
            ctx->Callback(dcl.nItemId, -1, true, true);
            return false;
        }

        // create pipes for processes bridge
        if (Bridge.Create() == false)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();

            Stdout.CloseRead();
            Stdout.CloseWrite();

            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0013000A));
            ctx->Callback(dcl.nItemId, -1, true, true);
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
        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

        // create decoder process
        if (decoderProcess.Start(dcl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;

                int nTool = -1;
                nTool = ctx->pConfig->m_Tools.GetToolByPath(dcl.pFormat->szPath);
                if (nTool < 0)
                {
                    nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, dcl.pFormat->szId);
                }

                if (nTool >= 0)
                {
                    config::CTool& tool = ctx->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                        [this, ctx, &dcl](int nIndex, std::wstring szStatus)
                    {
                        ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

                        if (decoderProcess.Start(dcl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
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
                szStatus.Format(ctx->pConfig->GetString(0x00130006).c_str(), ::GetLastError());

                ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                ctx->Callback(dcl.nItemId, -1, true, true);
                return false;
            }
        }

        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

        // create encoder process
        if (encoderProcess.Start(ecl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
        {
            bool bFailed = true;

            if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
            {
                CToolUtilities m_Utilities;

                int nTool = -1;
                nTool = ctx->pConfig->m_Tools.GetToolByPath(ecl.pFormat->szPath);
                if (nTool < 0)
                {
                    nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, ecl.pFormat->szId);
                }

                if (nTool >= 0)
                {
                    config::CTool& tool = ctx->pConfig->m_Tools.Get(nTool);
                    bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                        [this, ctx, &ecl](int nIndex, std::wstring szStatus)
                    {
                        ctx->Status(ecl.nItemId, ctx->pConfig->GetString(0x00150001), szStatus);
                    });

                    if (bResult == true)
                    {
                        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

                        if (encoderProcess.Start(ecl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
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

                decoderProcess.Stop(false, dcl.pFormat->nExitCodeSuccess);

                Stdin.CloseRead();
                Stdin.CloseWrite();

                Stdout.CloseRead();
                Stdout.CloseWrite();

                Bridge.CloseRead();
                Bridge.CloseWrite();

                CString szStatus;
                szStatus.Format(ctx->pConfig->GetString(0x00130006).c_str(), ::GetLastError());

                ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                ctx->Callback(dcl.nItemId, -1, true, true);
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
        readContext.szFileName = dcl.szInputFile;
        readContext.nIndex = dcl.nItemId;

        if (readThread.Start([this, ctx, &readContext, &Stdin]() { readContext.ReadLoop(ctx, Stdin); }) == false)
        {
            timer.Stop();

            Stdin.CloseWrite();

            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130007));
            ctx->Callback(dcl.nItemId, -1, true, true);
            return false;
        }

        // create write thread
        writeContext.bError = false;
        writeContext.bFinished = false;
        writeContext.szFileName = ecl.szOutputFile;
        writeContext.nIndex = ecl.nItemId;

        if (writeThread.Start([this, ctx, &writeContext, &Stdout]() { writeContext.WriteLoop(ctx, Stdout); }) == false)
        {
            timer.Stop();

            Stdout.CloseRead();

            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130008));
            ctx->Callback(dcl.nItemId, -1, true, true);
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

        if (decoderProcess.Stop(nProgress == 100, dcl.pFormat->nExitCodeSuccess) == false)
            nProgress = -1;

        if (encoderProcess.Stop(nProgress == 100, ecl.pFormat->nExitCodeSuccess) == false)
            nProgress = -1;

        if (nProgress != 100)
        {
            ctx->Status(dcl.nItemId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00130009));
            ctx->Callback(dcl.nItemId, -1, true, true);
            return false;
        }
        else
        {
            ctx->Status(dcl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->pConfig->GetString(0x0013000B));
            ctx->Callback(dcl.nItemId, 100, true, false);
            return true;
        }
    }

    bool CWorker::ConvertItem(IWorkerContext* ctx, int nId, util::CSynchronize &syncDir, util::CSynchronize &syncDown)
    {
        config::CFormat *pEncFormat = nullptr;
        config::CFormat *pDecFormat = nullptr;
        std::wstring szEncInputFile;
        std::wstring szEncOutputFile;
        std::wstring szDecInputFile;
        std::wstring szDecOutputFile;
        COutputPath m_Output;
        CCommandLine decoderCommandLine;
        CCommandLine encoderCommandLine;

        // prepare encoder
        config::CItem& item = ctx->pConfig->m_Items.Get(nId);
        config::CPath& path = item.m_Paths.Get(0);

        szEncInputFile = path.szPath;
        if (util::Utilities::FileExists(szEncInputFile) == false)
        {
            ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140001));
            return false;
        }

        int nEncoder = ctx->pConfig->m_Formats.GetFormatById(item.szFormatId);
        if (nEncoder == -1)
        {
            ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140002));
            return false;
        }

        pEncFormat = &ctx->pConfig->m_Formats.Get(nEncoder);

        if (item.nPreset >= pEncFormat->m_Presets.Count())
        {
            ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140003));
            return false;
        }

        bool bIsValidEncoderInput = pEncFormat->IsValidInputExtension(util::Utilities::GetFileExtension(szEncInputFile));

        szEncOutputFile = m_Output.CreateFilePath(
            ctx->pConfig->m_Options.szOutputPath,
            szEncInputFile,
            item.szName,
            pEncFormat->szOutputExtension);

        if (ctx->pConfig->m_Options.bOverwriteExistingFiles == false)
        {
            if (util::Utilities::FileExists(szEncOutputFile) == true)
            {
                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140010));
                return false;
            }
        }

        // create output path
        if (syncDir.Wait() == true)
        {
            if (m_Output.CreateOutputPath(szEncOutputFile) == false)
            {
                syncDir.Release();
                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000F));
                return false;
            }
        }
        else
        {
            ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000F));
            return false;
        }

        if (syncDir.Release() == false)
        {
            ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000F));
            return false;
        }

        ::SetCurrentDirectory(config::m_Settings.szSettingsPath.c_str());

        // prepare decoder
        if (bIsValidEncoderInput == false)
        {
            int nDecoder = ctx->pConfig->m_Formats.GetDecoderByExtensionAndFormat(item.szExtension, pEncFormat);
            if (nDecoder == -1)
            {
                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140004));
                return false;
            }

            pDecFormat = &ctx->pConfig->m_Formats.Get(nDecoder);

            if (pDecFormat->nDefaultPreset >= pDecFormat->m_Presets.Count())
            {
                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140005));
                return false;
            }

            bool bIsValidDecoderOutput = pEncFormat->IsValidInputExtension(pDecFormat->szOutputExtension);
            if (bIsValidDecoderOutput == false)
            {
                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140006));
                return false;
            }

            szDecInputFile = szEncInputFile;
            szDecOutputFile = 
                util::Utilities::GetFilePath(szEncOutputFile) + 
                util::Utilities::GenerateUuidString() +
                L"." + util::StringHelper::TowLower(pDecFormat->szOutputExtension);
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
                L"");
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
                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000C));

                item.ResetProgress();

                bool bResult = ConvertFileUsingOnlyPipes(ctx, decoderCommandLine, encoderCommandLine, syncDown);
                if (bResult == true)
                {
                    if (ctx->pConfig->m_Options.bDeleteSourceFiles == true)
                        ::DeleteFile(szEncInputFile.c_str());

                    return true;
                }
                else
                {
                    if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szEncOutputFile.c_str());

                    return false;
                }
            }
            catch (...)
            {
                if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile.c_str());

                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000E));
                ctx->Callback(item.nId, -1, true, true);
            }
        }
        else
        {
            // decode
            if (bIsValidEncoderInput == false)
            {
                try
                {
                    ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140007));

                    item.ResetProgress();

                    bool bResult = false;
                    if ((decoderCommandLine.bUseReadPipes == false) && (decoderCommandLine.bUseWritePipes == false))
                        bResult = ConvertFileUsingConsole(ctx, decoderCommandLine, syncDown);
                    else
                        bResult = ConvertFileUsingPipes(ctx, decoderCommandLine, syncDown);
                    if (bResult == false)
                    {
                        if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szDecOutputFile.c_str());

                        return false;
                    }

                    if (util::Utilities::FileExists(szDecOutputFile) == false)
                    {
                        ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140008));
                        return false;
                    }
                }
                catch (...)
                {
                    if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szEncOutputFile.c_str());

                    ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x00140009));
                    ctx->Callback(item.nId, -1, true, true);
                }
            }

            if (ctx->bRunning == false)
                return false;

            // encode
            try
            {
                if (pEncFormat->nType == config::FormatType::Encoder)
                    ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000A));
                else if (pEncFormat->nType == config::FormatType::Decoder)
                    ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000B));

                item.ResetProgress();

                bool bResult = false;
                if ((encoderCommandLine.bUseReadPipes == false) && (encoderCommandLine.bUseWritePipes == false))
                    bResult = ConvertFileUsingConsole(ctx, encoderCommandLine, syncDown);
                else
                    bResult = ConvertFileUsingPipes(ctx, encoderCommandLine, syncDown);
                if (bResult == true)
                {
                    if (bIsValidEncoderInput == false)
                        ::DeleteFile(szDecOutputFile.c_str());

                    if (ctx->pConfig->m_Options.bDeleteSourceFiles == true)
                        ::DeleteFile(szEncInputFile.c_str());

                    return true;
                }
                else
                {
                    if (bIsValidEncoderInput == false)
                        ::DeleteFile(szDecOutputFile.c_str());

                    if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szEncOutputFile.c_str());

                    return false;
                }
            }
            catch (...)
            {
                if (bIsValidEncoderInput == false)
                    ::DeleteFile(szDecOutputFile.c_str());

                if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile.c_str());

                ctx->Status(item.nId, ctx->pConfig->GetString(0x00150001), ctx->pConfig->GetString(0x0014000E));
                ctx->Callback(item.nId, -1, true, true);
            }
        }

        return false;
    }

    bool CWorker::ConvertLoop(IWorkerContext* ctx, std::queue<int> &queue, util::CSynchronize &sync, util::CSynchronize &syncDir, util::CSynchronize &syncDown)
    {
        while (TRUE)
        {
            try
            {
                if (sync.Wait() == true)
                {
                    if (!queue.empty())
                    {
                        int nId = queue.front();
                        queue.pop();

                        if (sync.Release() == false)
                            return false;

                        if (ctx->bRunning == false)
                            return false;

                        ctx->Next(nId);
                        if (ConvertItem(ctx, nId, syncDir, syncDown) == true)
                        {
                            ctx->nProcessedFiles++;
                        }
                        else
                        {
                            ctx->nProcessedFiles++;
                            ctx->nErrors++;
                            if (ctx->pConfig->m_Options.bStopOnErrors == true)
                                return false;
                        }

                        if (ctx->bRunning == false)
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

    void CWorker::Convert(IWorkerContext* ctx)
    {
        std::queue<int> queue;
        util::CSynchronize sync;
        util::CSynchronize syncDir;
        util::CSynchronize syncDown;

        ctx->nTotalFiles = 0;
        ctx->nProcessedFiles = 0;
        ctx->nErrors = 0;
        ctx->nLastItemId = -1;

        int nItems = ctx->pConfig->m_Items.Count();
        for (int i = 0; i < nItems; i++)
        {
            config::CItem& item = ctx->pConfig->m_Items.Get(i);
            if (item.bChecked == true)
            {
                item.ResetProgress();
                queue.push(item.nId);
                ctx->nTotalFiles++;
            }
            else
            {
                item.nProgress = 100;
                item.nPreviousProgress = 100;
            }
        }

        ctx->nThreadCount = ctx->pConfig->m_Options.nThreadCount;
        if (ctx->nThreadCount < 1)
        {
            util::Utilities::LogicalProcessorInformation info;
            if (util::Utilities::GetLogicalProcessorInformation(&info) == 0)
                ctx->nThreadCount = info.processorCoreCount;
            else
                ctx->nThreadCount = 1;
        }

        ctx->Init();

        // single-threaded
        if (ctx->nThreadCount == 1)
        {
            ConvertLoop(ctx, queue, sync, syncDir, syncDown);
        }

        // multi-threaded
        if (ctx->nThreadCount > 1)
        {
            auto threads = std::make_unique<util::CThread[]>(ctx->nThreadCount);
            for (int i = 0; i < ctx->nThreadCount; i++)
            {
                auto entry = [this, ctx, &queue, &sync, &syncDir, &syncDown]()
                { 
                    this->ConvertLoop(ctx, queue, sync, syncDir, syncDown);
                };
                if (threads[i].Start(entry, true) == false)
                    break;

                if (threads[i].Resume() == false)
                    break;
            }

            for (int i = 0; i < ctx->nThreadCount; i++)
                threads[i].Wait();

            for (int i = 0; i < ctx->nThreadCount; i++)
                threads[i].Close();
        }

        ctx->Done();
        ctx->bDone = true;
    }
}
