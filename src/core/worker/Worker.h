// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include <utility>
#include <string>
#include <mutex>
#include <thread>
#include <atlstr.h>
#include "utilities\TimeCount.h"
#include "utilities\Utilities.h"
#include "utilities\Pipe.h"
#include "utilities\Process.h"
#include "utilities\StringHelper.h"
#include "WorkerContext.h"
#include "CommandLine.h"
#include "InputPath.h"
#include "OutputPath.h"
#include "FileToPipeWriter.h"
#include "PipeToFileWriter.h"
#include "PipeToStringWriter.h"
#include "OutputParser.h"
#include "LuaOutputParser.h"
#include "DebugOutputParser.h"
#include "ToolUtilities.h"

namespace worker
{
    class CWorker
    {
    public:
        bool CWorker::ConvertFileUsingConsole(IWorkerContext* ctx, CCommandLine &cl, std::mutex &syncDown)
        {
            util::CProcess process;
            util::CPipe Stderr(true);
            util::CTimeCount timer;
            CLuaOutputParser parser;
            CPipeToStringWriter writer;

            if ((cl.bUseReadPipes == true) || (cl.bUseWritePipes == true))
            {
                ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120001));
                ctx->Progress(cl.nItemId, -1, true, true);
                return false;
            }

            // create pipes for stderr
            if (Stderr.Create() == false)
            {
                ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120002));
                ctx->Progress(cl.nItemId, -1, true, true);
                return false;
            }

            // duplicate stderr read pipe handle to prevent child process from closing the pipe
            if (Stderr.DuplicateRead() == false)
            {
                ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120003));
                ctx->Progress(cl.nItemId, -1, true, true);
                return false;
            }

            // connect pipes to process
            process.ConnectStdInput(nullptr);
            process.ConnectStdOutput(Stderr.hWrite);
            process.ConnectStdError(Stderr.hWrite);

            syncDown.lock();
            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

            timer.Start();
            if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;

                if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
                {
                    CToolUtilities m_Utilities;

                    int nTool = config::CTool::GetToolByPath(ctx->pConfig->m_Tools, cl.pFormat->szPath);
                    if (nTool < 0)
                    {
                        nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, cl.pFormat->szId);
                    }

                    if (nTool >= 0)
                    {
                        config::CTool& tool = ctx->pConfig->m_Tools[nTool];
                        bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                            [this, ctx, &cl](int nIndex, std::wstring szStatus) -> bool
                        {
                            ctx->Status(cl.nItemId, ctx->GetString(0x00150001), szStatus);
                            if (ctx->bRunning == false)
                                return true;
                            return false;
                        });

                        if (bResult == true)
                        {
                            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

                            if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
                            {
                                bFailed = false;
                            }
                        }
                    }
                }

                if (bFailed == true)
                {
                    syncDown.unlock();

                    timer.Stop();

                    Stderr.CloseRead();
                    Stderr.CloseWrite();

                    CString szStatus;
                    szStatus.Format(ctx->GetString(0x00120004).c_str(), ::GetLastError());

                    ctx->Status(cl.nItemId, ctx->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                    ctx->Progress(cl.nItemId, -1, true, true);
                    return false;
                }
            }

            syncDown.unlock();

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
                ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120005));
                ctx->Progress(cl.nItemId, -1, true, true);
                return false;
            }
            else
            {
                ctx->Status(cl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->GetString(0x00120006));
                ctx->Progress(cl.nItemId, 100, true, false);
                return true;
            }
        }
        bool CWorker::ConvertFileUsingPipes(IWorkerContext* ctx, CCommandLine &cl, std::mutex &syncDown)
        {
            util::CProcess process;
            util::CPipe Stdin(true);
            util::CPipe Stdout(true);
            CFileToPipeWriter readContext;
            CPipeToFileWriter writeContext;
            std::thread readThread;
            std::thread writeThread;
            int nProgress = 0;
            util::CTimeCount timer;

            if ((cl.bUseReadPipes == false) && (cl.bUseWritePipes == false))
            {
                ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130001));
                ctx->Progress(cl.nItemId, -1, true, true);
                return false;
            }

            if (cl.bUseReadPipes == true)
            {
                // create pipes for stdin
                if (Stdin.Create() == false)
                {
                    ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130002));
                    ctx->Progress(cl.nItemId, -1, true, true);
                    return false;
                }

                // set stdin write pipe inherit flag
                if (Stdin.InheritWrite() == false)
                {
                    ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130003));
                    ctx->Progress(cl.nItemId, -1, true, true);
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

                    ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130004));
                    ctx->Progress(cl.nItemId, -1, true, true);
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

                    ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130005));
                    ctx->Progress(cl.nItemId, -1, true, true);
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

            syncDown.lock();
            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

            timer.Start();
            if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;

                if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
                {
                    CToolUtilities m_Utilities;

                    int nTool = config::CTool::GetToolByPath(ctx->pConfig->m_Tools, cl.pFormat->szPath);
                    if (nTool < 0)
                    {
                        nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, cl.pFormat->szId);
                    }

                    if (nTool >= 0)
                    {
                        config::CTool& tool = ctx->pConfig->m_Tools[nTool];
                        bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                            [this, ctx, &cl](int nIndex, std::wstring szStatus) -> bool
                        {
                            ctx->Status(cl.nItemId, ctx->GetString(0x00150001), szStatus);
                            if (ctx->bRunning == false)
                                return true;
                            return false;
                        });

                        if (bResult == true)
                        {
                            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

                            if (process.Start(cl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
                            {
                                bFailed = false;
                            }
                        }
                    }
                }

                if (bFailed == true)
                {
                    syncDown.unlock();

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
                    szStatus.Format(ctx->GetString(0x00130006).c_str(), ::GetLastError());

                    ctx->Status(cl.nItemId, ctx->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                    ctx->Progress(cl.nItemId, -1, true, true);
                    return false;
                }
            }

            syncDown.unlock();

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

                readThread = std::thread([this, ctx, &readContext, &Stdin]() { readContext.ReadLoop(ctx, Stdin); });

                // wait for read thread to finish
                if (cl.bUseWritePipes == false)
                {
                    readThread.join();

                    // NOTE: Handle is closed in ReadThread.
                    //Stdin.CloseWrite();

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

                writeThread = std::thread([this, ctx, &writeContext, &Stdout]() { writeContext.WriteLoop(ctx, Stdout); });

                if (cl.bUseReadPipes == true)
                {
                    // wait for read thread to finish
                    readThread.join();

                    // NOTE: Handle is closed in ReadThread.
                    //Stdin.CloseWrite();

                    if ((readContext.bError == true) || (readContext.bFinished == false))
                    {
                        // close write thread handle
                        Stdout.CloseRead();

                        // read thread failed so terminate write thread
                        writeThread.join();
                    }
                    else
                    {
                        // wait for write thread to finish
                        writeThread.join();

                        // close write thread handle
                        Stdout.CloseRead();
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
                    writeThread.join();

                    // close write thread handle
                    Stdout.CloseRead();

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
                ctx->Status(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130009));
                ctx->Progress(cl.nItemId, -1, true, true);
                return false;
            }
            else
            {
                ctx->Status(cl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->GetString(0x0013000B));
                ctx->Progress(cl.nItemId, 100, true, false);
                return true;
            }
        }
        bool CWorker::ConvertFileUsingOnlyPipes(IWorkerContext* ctx, CCommandLine &dcl, CCommandLine &ecl, std::mutex &syncDown)
        {
            util::CProcess decoderProcess;
            util::CProcess encoderProcess;
            util::CPipe Stdin(true);
            util::CPipe Stdout(true);
            util::CPipe Bridge(true);
            CFileToPipeWriter readContext;
            CPipeToFileWriter writeContext;
            std::thread readThread;
            std::thread writeThread;
            int nProgress = 0;
            util::CTimeCount timer;

            // create pipes for stdin
            if (Stdin.Create() == false)
            {
                ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130002));
                ctx->Progress(dcl.nItemId, -1, true, true);
                return false;
            }

            // set stdin write pipe inherit flag
            if (Stdin.InheritWrite() == false)
            {
                ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130003));
                ctx->Progress(dcl.nItemId, -1, true, true);
                return false;
            }

            // create pipes for stdout
            if (Stdout.Create() == false)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();

                ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130004));
                ctx->Progress(dcl.nItemId, -1, true, true);
                return false;
            }

            // set stdout read pipe inherit flag
            if (Stdout.InheritRead() == false)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();

                Stdout.CloseRead();
                Stdout.CloseWrite();

                ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130005));
                ctx->Progress(dcl.nItemId, -1, true, true);
                return false;
            }

            // create pipes for processes bridge
            if (Bridge.Create() == false)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();

                Stdout.CloseRead();
                Stdout.CloseWrite();

                ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x0013000A));
                ctx->Progress(dcl.nItemId, -1, true, true);
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

            syncDown.lock();
            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

            // create decoder process
            if (decoderProcess.Start(dcl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;

                if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
                {
                    CToolUtilities m_Utilities;

                    int nTool = config::CTool::GetToolByPath(ctx->pConfig->m_Tools, dcl.pFormat->szPath);
                    if (nTool < 0)
                    {
                        nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, dcl.pFormat->szId);
                    }

                    if (nTool >= 0)
                    {
                        config::CTool& tool = ctx->pConfig->m_Tools[nTool];
                        bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                            [this, ctx, &dcl](int nIndex, std::wstring szStatus) -> bool
                        {
                            ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), szStatus);
                            if (ctx->bRunning == false)
                                return true;
                            return false;
                        });

                        if (bResult == true)
                        {
                            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

                            if (decoderProcess.Start(dcl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
                            {
                                bFailed = false;
                            }
                        }
                    }
                }

                if (bFailed == true)
                {
                    syncDown.unlock();

                    timer.Stop();

                    Stdin.CloseRead();
                    Stdin.CloseWrite();

                    Stdout.CloseRead();
                    Stdout.CloseWrite();

                    Bridge.CloseRead();
                    Bridge.CloseWrite();

                    CString szStatus;
                    szStatus.Format(ctx->GetString(0x00130006).c_str(), ::GetLastError());

                    ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                    ctx->Progress(dcl.nItemId, -1, true, true);
                    return false;
                }
            }

            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

            // create encoder process
            if (encoderProcess.Start(ecl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;

                if (ctx->pConfig->m_Options.bTryToDownloadTools == true)
                {
                    CToolUtilities m_Utilities;

                    int nTool = -1;
                    nTool = config::CTool::GetToolByPath(ctx->pConfig->m_Tools, ecl.pFormat->szPath);
                    if (nTool < 0)
                    {
                        nTool = m_Utilities.FindTool(ctx->pConfig->m_Tools, ecl.pFormat->szId);
                    }

                    if (nTool >= 0)
                    {
                        config::CTool& tool = ctx->pConfig->m_Tools[nTool];
                        bool bResult = m_Utilities.Download(tool, true, true, nTool, ctx->pConfig,
                            [this, ctx, &ecl](int nIndex, std::wstring szStatus) -> bool
                        {
                            ctx->Status(ecl.nItemId, ctx->GetString(0x00150001), szStatus);
                            if (ctx->bRunning == false)
                                return true;
                            return false;
                        });

                        if (bResult == true)
                        {
                            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

                            if (encoderProcess.Start(ecl.szCommandLine, ctx->pConfig->m_Options.bHideConsoleWindow) == true)
                            {
                                bFailed = false;
                            }
                        }
                    }
                }

                if (bFailed == true)
                {
                    syncDown.unlock();

                    timer.Stop();

                    decoderProcess.Stop(false, dcl.pFormat->nExitCodeSuccess);

                    Stdin.CloseRead();
                    Stdin.CloseWrite();

                    Stdout.CloseRead();
                    Stdout.CloseWrite();

                    Bridge.CloseRead();
                    Bridge.CloseWrite();

                    CString szStatus;
                    szStatus.Format(ctx->GetString(0x00130006).c_str(), ::GetLastError());

                    ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), std::wstring(CT2CW(szStatus)));
                    ctx->Progress(dcl.nItemId, -1, true, true);
                    return false;
                }
            }

            syncDown.unlock();

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

            readThread = std::thread([this, ctx, &readContext, &Stdin]() { readContext.ReadLoop(ctx, Stdin); });

            // create write thread
            writeContext.bError = false;
            writeContext.bFinished = false;
            writeContext.szFileName = ecl.szOutputFile;
            writeContext.nIndex = ecl.nItemId;

            writeThread = std::thread([this, ctx, &writeContext, &Stdout]() { writeContext.WriteLoop(ctx, Stdout); });

            // wait for read thread to finish after write thread finished
            readThread.join();

            // NOTE: Handle is closed in ReadThread.
            //Stdin.CloseWrite();

            if ((readContext.bError == true) || (readContext.bFinished == false))
            {
                Stdout.CloseRead();

                // read thread failed so terminate write thread
                writeThread.join();
            }
            else
            {
                Stdout.CloseRead();

                // wait for write thread to finish
                writeThread.join();
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
                ctx->Status(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130009));
                ctx->Progress(dcl.nItemId, -1, true, true);
                return false;
            }
            else
            {
                ctx->Status(dcl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->GetString(0x0013000B));
                ctx->Progress(dcl.nItemId, 100, true, false);
                return true;
            }
        }
        bool CWorker::ConvertItem(IWorkerContext* ctx, int nId, std::mutex &syncDir, std::mutex &syncDown)
        {
            config::CFormat *pEncFormat = nullptr;
            config::CFormat *pDecFormat = nullptr;
            std::wstring szEncInputFile;
            std::wstring szEncOutputFile;
            std::wstring szDecInputFile;
            std::wstring szDecOutputFile;
            COutputPath m_Output;
            CCommandLine dcl;
            CCommandLine ecl;

            // prepare encoder
            config::CItem& item = ctx->pConfig->m_Items[nId];
            config::CPath& path = item.m_Paths[0];

            szEncInputFile = path.szPath;
            if (util::Utilities::FileExists(szEncInputFile) == false)
            {
                ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140001));
                return false;
            }

            int nEncoder = config::CFormat::GetFormatById(ctx->pConfig->m_Formats, item.szFormatId);
            if (nEncoder == -1)
            {
                ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140002));
                return false;
            }

            pEncFormat = &ctx->pConfig->m_Formats[nEncoder];

            if (item.nPreset >= pEncFormat->m_Presets.size())
            {
                ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140003));
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
                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140010));
                    return false;
                }
            }

            // create output path
            syncDir.lock();

            if (m_Output.CreateOutputPath(szEncOutputFile) == false)
            {
                syncDir.unlock();
                ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000F));
                return false;
            }

            syncDir.unlock();

            ::SetCurrentDirectory(ctx->pConfig->m_Settings.szSettingsPath.c_str());

            // prepare decoder
            if (bIsValidEncoderInput == false)
            {
                int nDecoder = config::CFormat::GetDecoderByExtensionAndFormat(ctx->pConfig->m_Formats, item.szExtension, pEncFormat);
                if (nDecoder == -1)
                {
                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140004));
                    return false;
                }

                pDecFormat = &ctx->pConfig->m_Formats[nDecoder];

                if (pDecFormat->nDefaultPreset >= pDecFormat->m_Presets.size())
                {
                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140005));
                    return false;
                }

                bool bIsValidDecoderOutput = pEncFormat->IsValidInputExtension(pDecFormat->szOutputExtension);
                if (bIsValidDecoderOutput == false)
                {
                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140006));
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
                dcl.Build(
                    pDecFormat,
                    pDecFormat->nDefaultPreset,
                    item.nId,
                    szDecInputFile,
                    szDecOutputFile,
                    pDecFormat->bPipeInput,
                    pDecFormat->bPipeOutput,
                    L"");
            }

            ecl.Build(
                pEncFormat,
                item.nPreset,
                item.nId,
                bIsValidEncoderInput == true ? szEncInputFile : szDecOutputFile,
                szEncOutputFile,
                pEncFormat->bPipeInput,
                pEncFormat->bPipeOutput,
                item.szOptions);

            if (bIsValidEncoderInput == false
                && dcl.bUseReadPipes == true && dcl.bUseWritePipes == true
                && ecl.bUseReadPipes == true && ecl.bUseWritePipes == true)
            {
                // trans-code
                try
                {
                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000C));

                    item.ResetProgress();

                    bool bResult = ConvertFileUsingOnlyPipes(ctx, dcl, ecl, syncDown);
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

                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000E));
                    ctx->Progress(item.nId, -1, true, true);
                }
            }
            else
            {
                // decode
                if (bIsValidEncoderInput == false)
                {
                    try
                    {
                        ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140007));

                        item.ResetProgress();

                        bool bResult = false;
                        if ((dcl.bUseReadPipes == false) && (dcl.bUseWritePipes == false))
                            bResult = ConvertFileUsingConsole(ctx, dcl, syncDown);
                        else
                            bResult = ConvertFileUsingPipes(ctx, dcl, syncDown);
                        if (bResult == false)
                        {
                            if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                                ::DeleteFile(szDecOutputFile.c_str());

                            return false;
                        }

                        if (util::Utilities::FileExists(szDecOutputFile) == false)
                        {
                            ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140008));
                            return false;
                        }
                    }
                    catch (...)
                    {
                        if (ctx->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szEncOutputFile.c_str());

                        ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140009));
                        ctx->Progress(item.nId, -1, true, true);
                    }
                }

                if (ctx->bRunning == false)
                    return false;

                // encode
                try
                {
                    if (pEncFormat->nType == config::FormatType::Encoder)
                        ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000A));
                    else if (pEncFormat->nType == config::FormatType::Decoder)
                        ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000B));

                    item.ResetProgress();

                    bool bResult = false;
                    if ((ecl.bUseReadPipes == false) && (ecl.bUseWritePipes == false))
                        bResult = ConvertFileUsingConsole(ctx, ecl, syncDown);
                    else
                        bResult = ConvertFileUsingPipes(ctx, ecl, syncDown);
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

                    ctx->Status(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000E));
                    ctx->Progress(item.nId, -1, true, true);
                }
            }

            return false;
        }
        bool CWorker::ConvertLoop(IWorkerContext* ctx, std::queue<int> &queue, std::mutex &sync, std::mutex &syncDir, std::mutex &syncDown)
        {
            while (true)
            {
                try
                {
                    sync.lock();

                    if (!queue.empty())
                    {
                        int nId = queue.front();
                        queue.pop();

                        sync.unlock();

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
                        sync.unlock();
                        return true;
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
            std::mutex sync;
            std::mutex syncDir;
            std::mutex syncDown;

            size_t nItems = ctx->pConfig->m_Items.size();
            for (size_t i = 0; i < nItems; i++)
            {
                config::CItem& item = ctx->pConfig->m_Items[i];
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

            ctx->Start();

            if (ctx->nThreadCount <= 1)
            {
                ConvertLoop(ctx, queue, sync, syncDir, syncDown);
            }
            else
            {
                auto convert = [this, ctx, &queue, &sync, &syncDir, &syncDown]()
                {
                    this->ConvertLoop(ctx, queue, sync, syncDir, syncDown);
                };

                auto threads = std::make_unique<std::thread[]>(ctx->nThreadCount);
                for (int i = 0; i < ctx->nThreadCount; i++)
                    threads[i] = std::thread(convert);

                for (int i = 0; i < ctx->nThreadCount; i++)
                    threads[i].join();
            }

            ctx->Stop();
            ctx->bDone = true;
        }
    };
}
