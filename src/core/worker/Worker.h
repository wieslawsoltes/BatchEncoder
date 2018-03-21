// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include <utility>
#include <string>
#include <mutex>
#include <thread>
#include "utilities\FileSystem.h"
#include "utilities\Log.h"
#include "utilities\MemoryLog.h"
#include "utilities\String.h"
#include "utilities\TimeCount.h"
#include "WorkerContext.h"
#include "CommandLine.h"
#include "OutputPath.h"

namespace worker
{
    class IConverter
    {
    public:
        virtual ~IConverter() { };
        virtual bool Run(IWorkerContext* ctx, CCommandLine& cl, std::mutex& m_down) = 0;
    };

    class ITranscoder
    {
    public:
        virtual ~ITranscoder() { };
        virtual bool Run(IWorkerContext* ctx, CCommandLine &dcl, CCommandLine& ecl, std::mutex& m_down) = 0;
    };

    class IWorker
    {
    public:
        virtual ~IWorker() { };
        virtual bool Transcode(IWorkerContext* ctx, config::CItem& item, CCommandLine& dcl, CCommandLine& ecl, std::mutex& m_down) = 0;
        virtual bool Decode(IWorkerContext* ctx, config::CItem& item, CCommandLine& cl, std::mutex& m_down) = 0;
        virtual bool Encode(IWorkerContext* ctx, config::CItem& item, CCommandLine& cl, std::mutex& m_down) = 0;
        virtual bool Convert(IWorkerContext* ctx, config::CItem& item, std::mutex& m_dir, std::mutex& m_down) = 0;
        virtual bool Convert(IWorkerContext* ctx, std::queue<int>& queue, std::mutex& m_queue, std::mutex &m_dir, std::mutex &m_down) = 0;
        virtual void Convert(IWorkerContext* ctx, config::CItem& item) = 0;
        virtual void Convert(IWorkerContext* ctx, std::vector<config::CItem>& items) = 0;
    };

    class CConsoleConverter : public IConverter
    {
    public:
        bool Run(IWorkerContext* ctx, CCommandLine& cl, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            auto process = ctx->pFactory->CreateProcessPtr();
            auto Stderr = ctx->pFactory->CreatePipePtr();
            auto parser = ctx->pFactory->CreateOutputParserPtr();
            auto writer = ctx->pFactory->CreateStringWriterPtr();
            util::CTimeCount timer;

            if ((cl.bUseReadPipes == true) || (cl.bUseWritePipes == true))
            {
                ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120001));
                ctx->ItemProgress(cl.nItemId, -1, true, true);
                return false;
            }

            // create pipes for stderr
            if (Stderr->Create() == false)
            {
                ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120002));
                ctx->ItemProgress(cl.nItemId, -1, true, true);
                return false;
            }

            // duplicate stderr read pipe handle to prevent child process from closing the pipe
            if (Stderr->DuplicateRead() == false)
            {
                ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120003));
                ctx->ItemProgress(cl.nItemId, -1, true, true);
                return false;
            }

            // connect pipes to process
            process->ConnectStdInput(nullptr);
            process->ConnectStdOutput(Stderr->WriteHandle());
            process->ConnectStdError(Stderr->WriteHandle());

            m_down.lock();
            config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);

            timer.Start();
            if (process->Start(cl.szCommandLine, config->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;
                if (config->m_Options.bTryToDownloadTools == true)
                {
                    auto downloader = ctx->pFactory->CreateDownloaderPtr();
                    if (downloader->Download(ctx, cl.format, cl.nItemId) == true)
                    {
                        config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);
                        if (process->Start(cl.szCommandLine, config->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }

                if (bFailed == true)
                {
                    m_down.unlock();

                    timer.Stop();

                    Stderr->CloseRead();
                    Stderr->CloseWrite();

                    std::wstring szStatus = ctx->GetString(0x00120004) + L" (" + std::to_wstring(::GetLastError()) + L")";
                    ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), szStatus);
                    ctx->ItemProgress(cl.nItemId, -1, true, true);
                    return false;
                }
            }

            m_down.unlock();

            // close unused pipe handle
            Stderr->CloseWrite();

            // init output parser
            m_down.lock();
            config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);

            parser->nIndex = cl.nItemId;
            parser->nProgress = 0;
            parser->nPreviousProgress = 0;

            //auto log = std::make_unique<util::MemoryLog>();
            //parser->log = log.get();
            if (parser->Open(ctx, cl.format.szFunction) == false)
            {
                timer.Stop();
                Stderr->CloseRead();
                process->Stop(false, cl.format.nExitCodeSuccess);
                m_down.unlock();
                return false;
            }

            m_down.unlock();

            // console progress loop
            writer->nIndex = cl.nItemId;
            writer->bError = false;
            writer->bFinished = false;

            if (writer->WriteLoop(ctx, Stderr.get(), parser.get()) == false)
            {
                timer.Stop();
                Stderr->CloseRead();
                process->Stop(false, cl.format.nExitCodeSuccess);
                return false;
            }

            timer.Stop();
            Stderr->CloseRead();
            if (process->Stop(parser->nProgress == 100, cl.format.nExitCodeSuccess) == false)
                parser->nProgress = -1;

            if (parser->nProgress != 100)
            {
                ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00120005));
                ctx->ItemProgress(cl.nItemId, -1, true, true);
                return false;
            }
            else
            {
                ctx->ItemStatus(cl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->GetString(0x00120006));
                ctx->ItemProgress(cl.nItemId, 100, true, false);
                return true;
            }
        }
    };

    class CPipesConverter : public IConverter
    {
    public:
        bool Run(IWorkerContext* ctx, CCommandLine& cl, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            auto process = ctx->pFactory->CreateProcessPtr();
            auto Stdin = ctx->pFactory->CreatePipePtr();
            auto Stdout = ctx->pFactory->CreatePipePtr();
            auto readContext = ctx->pFactory->CreateFileReaderPtr();
            auto writeContext = ctx->pFactory->CreateFileWriterPtr();
            int nProgress = 0;
            std::thread readThread;
            std::thread writeThread;
            util::CTimeCount timer;

            if ((cl.bUseReadPipes == false) && (cl.bUseWritePipes == false))
            {
                ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130001));
                ctx->ItemProgress(cl.nItemId, -1, true, true);
                return false;
            }

            if (cl.bUseReadPipes == true)
            {
                // create pipes for stdin
                if (Stdin->Create() == false)
                {
                    ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130002));
                    ctx->ItemProgress(cl.nItemId, -1, true, true);
                    return false;
                }

                // set stdin write pipe inherit flag
                if (Stdin->InheritWrite() == false)
                {
                    ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130003));
                    ctx->ItemProgress(cl.nItemId, -1, true, true);
                    return false;
                }
            }

            if (cl.bUseWritePipes == true)
            {
                // create pipes for stdout
                if (Stdout->Create() == false)
                {
                    if (cl.bUseReadPipes == true)
                    {
                        Stdin->CloseRead();
                        Stdin->CloseWrite();
                    }

                    ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130004));
                    ctx->ItemProgress(cl.nItemId, -1, true, true);
                    return false;
                }

                // set stdout read pipe inherit flag
                if (Stdout->InheritRead() == false)
                {
                    if (cl.bUseReadPipes == true)
                    {
                        Stdin->CloseRead();
                        Stdin->CloseWrite();
                    }

                    Stdout->CloseRead();
                    Stdout->CloseWrite();

                    ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130005));
                    ctx->ItemProgress(cl.nItemId, -1, true, true);
                    return false;
                }
            }

            // connect pipes to process
            if ((cl.bUseReadPipes == true) && (cl.bUseWritePipes == false))
            {
                process->ConnectStdInput(Stdin->ReadHandle());
                process->ConnectStdOutput(process->StdoutHandle());
                process->ConnectStdError(process->StderrHandle());
            }
            else if ((cl.bUseReadPipes == false) && (cl.bUseWritePipes == true))
            {
                process->ConnectStdInput(process->StdinHandle());
                process->ConnectStdOutput(Stdout->WriteHandle());
                process->ConnectStdError(process->StderrHandle());
            }
            else if ((cl.bUseReadPipes == true) && (cl.bUseWritePipes == true))
            {
                process->ConnectStdInput(Stdin->ReadHandle());
                process->ConnectStdOutput(Stdout->WriteHandle());
                process->ConnectStdError(process->StderrHandle());
            }

            m_down.lock();
            config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);

            timer.Start();
            if (process->Start(cl.szCommandLine, config->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;
                if (config->m_Options.bTryToDownloadTools == true)
                {
                    auto downloader = ctx->pFactory->CreateDownloaderPtr();
                    if (downloader->Download(ctx, cl.format, cl.nItemId) == true)
                    {
                        config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);
                        if (process->Start(cl.szCommandLine, config->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }

                if (bFailed == true)
                {
                    m_down.unlock();

                    timer.Stop();

                    if (cl.bUseReadPipes == true)
                    {
                        Stdin->CloseRead();
                        Stdin->CloseWrite();
                    }

                    if (cl.bUseWritePipes == true)
                    {
                        Stdout->CloseRead();
                        Stdout->CloseWrite();
                    }

                    std::wstring szStatus = ctx->GetString(0x00130006) + L" (" + std::to_wstring(::GetLastError()) + L")";
                    ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), szStatus);
                    ctx->ItemProgress(cl.nItemId, -1, true, true);
                    return false;
                }
            }

            m_down.unlock();

            // close unused pipe handles
            if (cl.bUseReadPipes == true)
                Stdin->CloseRead();

            if (cl.bUseWritePipes == true)
                Stdout->CloseWrite();

            // create read thread
            if (cl.bUseReadPipes == true)
            {
                readContext->bError = false;
                readContext->bFinished = false;
                readContext->szFileName = cl.szInputFile;
                readContext->nIndex = cl.nItemId;

                readThread = std::thread([&]() { readContext->ReadLoop(ctx, Stdin.get()); });

                // wait for read thread to finish
                if (cl.bUseWritePipes == false)
                {
                    readThread.join();

                    // NOTE: Handle is closed in ReadThread.
                    //Stdin->CloseWrite();

                    // check for result from read thread
                    if ((readContext->bError == false) && (readContext->bFinished == true))
                        nProgress = 100;
                    else
                        nProgress = -1;
                }
            }

            // create write thread
            if (cl.bUseWritePipes == true)
            {
                writeContext->bError = false;
                writeContext->bFinished = false;
                writeContext->szFileName = cl.szOutputFile;
                writeContext->nIndex = cl.nItemId;

                writeThread = std::thread([&]() { writeContext->WriteLoop(ctx, Stdout.get()); });

                if (cl.bUseReadPipes == true)
                {
                    // wait for read thread to finish
                    readThread.join();

                    // NOTE: Handle is closed in ReadThread.
                    //Stdin->CloseWrite();

                    if ((readContext->bError == true) || (readContext->bFinished == false))
                    {
                        // close write thread handle
                        Stdout->CloseRead();

                        // read thread failed so terminate write thread
                        writeThread.join();
                    }
                    else
                    {
                        // wait for write thread to finish
                        writeThread.join();

                        // close write thread handle
                        Stdout->CloseRead();
                    }

                    // check for result from read thread
                    if ((readContext->bError == false) && (readContext->bFinished == true)
                        && (writeContext->bError == false) && (writeContext->bFinished == true))
                        nProgress = 100;
                    else
                        nProgress = -1;
                }
                else
                {
                    // wait for write thread to finish
                    writeThread.join();

                    // close write thread handle
                    Stdout->CloseRead();

                    // check for result from write thread
                    if ((writeContext->bError == false) && (writeContext->bFinished == true))
                        nProgress = 100;
                    else
                        nProgress = -1;
                }
            }

            timer.Stop();

            if (process->Stop(nProgress == 100, cl.format.nExitCodeSuccess) == false)
                nProgress = -1;

            if (nProgress != 100)
            {
                ctx->ItemStatus(cl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130009));
                ctx->ItemProgress(cl.nItemId, -1, true, true);
                return false;
            }
            else
            {
                ctx->ItemStatus(cl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->GetString(0x0013000B));
                ctx->ItemProgress(cl.nItemId, 100, true, false);
                return true;
            }
        }
    };

    class CPipesTranscoder : public ITranscoder
    {
    public:
        bool Run(IWorkerContext* ctx, CCommandLine &dcl, CCommandLine& ecl, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            auto decoderProcess = ctx->pFactory->CreateProcessPtr();
            auto encoderProcess = ctx->pFactory->CreateProcessPtr();
            auto Stdin = ctx->pFactory->CreatePipePtr();
            auto Stdout = ctx->pFactory->CreatePipePtr();
            auto Bridge = ctx->pFactory->CreatePipePtr();
            auto readContext = ctx->pFactory->CreateFileReaderPtr();
            auto writeContext = ctx->pFactory->CreateFileWriterPtr();;
            int nProgress = 0;
            std::thread readThread;
            std::thread writeThread;
            util::CTimeCount timer;

            // create pipes for stdin
            if (Stdin->Create() == false)
            {
                ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130002));
                ctx->ItemProgress(dcl.nItemId, -1, true, true);
                return false;
            }

            // set stdin write pipe inherit flag
            if (Stdin->InheritWrite() == false)
            {
                ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130003));
                ctx->ItemProgress(dcl.nItemId, -1, true, true);
                return false;
            }

            // create pipes for stdout
            if (Stdout->Create() == false)
            {
                Stdin->CloseRead();
                Stdin->CloseWrite();

                ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130004));
                ctx->ItemProgress(dcl.nItemId, -1, true, true);
                return false;
            }

            // set stdout read pipe inherit flag
            if (Stdout->InheritRead() == false)
            {
                Stdin->CloseRead();
                Stdin->CloseWrite();

                Stdout->CloseRead();
                Stdout->CloseWrite();

                ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130005));
                ctx->ItemProgress(dcl.nItemId, -1, true, true);
                return false;
            }

            // create pipes for processes bridge
            if (Bridge->Create() == false)
            {
                Stdin->CloseRead();
                Stdin->CloseWrite();

                Stdout->CloseRead();
                Stdout->CloseWrite();

                ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x0013000A));
                ctx->ItemProgress(dcl.nItemId, -1, true, true);
                return false;
            }

            // connect pipes to decoder process
            decoderProcess->ConnectStdInput(Stdin->ReadHandle());
            decoderProcess->ConnectStdOutput(Bridge->WriteHandle());
            decoderProcess->ConnectStdError(decoderProcess->StderrHandle());

            // connect pipes to encoder process
            encoderProcess->ConnectStdInput(Bridge->ReadHandle());
            encoderProcess->ConnectStdOutput(Stdout->WriteHandle());
            encoderProcess->ConnectStdError(encoderProcess->StderrHandle());

            timer.Start();

            m_down.lock();
            config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);

            // create decoder process
            if (decoderProcess->Start(dcl.szCommandLine, config->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;
                if (config->m_Options.bTryToDownloadTools == true)
                {
                    auto downloader = ctx->pFactory->CreateDownloaderPtr();
                    if (downloader->Download(ctx, dcl.format, dcl.nItemId) == true)
                    {
                        config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);
                        if (decoderProcess->Start(dcl.szCommandLine, config->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }

                if (bFailed == true)
                {
                    m_down.unlock();

                    timer.Stop();

                    Stdin->CloseRead();
                    Stdin->CloseWrite();

                    Stdout->CloseRead();
                    Stdout->CloseWrite();

                    Bridge->CloseRead();
                    Bridge->CloseWrite();

                    std::wstring szStatus = ctx->GetString(0x00130006) + L" (" + std::to_wstring(::GetLastError()) + L")";
                    ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), szStatus);
                    ctx->ItemProgress(dcl.nItemId, -1, true, true);
                    return false;
                }
            }

            config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);

            // create encoder process
            if (encoderProcess->Start(ecl.szCommandLine, config->m_Options.bHideConsoleWindow) == false)
            {
                bool bFailed = true;
                if (config->m_Options.bTryToDownloadTools == true)
                {
                    auto downloader = ctx->pFactory->CreateDownloaderPtr();
                    if (downloader->Download(ctx, ecl.format, ecl.nItemId) == true)
                    {
                        config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);
                        if (encoderProcess->Start(ecl.szCommandLine, config->m_Options.bHideConsoleWindow) == true)
                        {
                            bFailed = false;
                        }
                    }
                }

                if (bFailed == true)
                {
                    m_down.unlock();

                    timer.Stop();

                    decoderProcess->Stop(false, dcl.format.nExitCodeSuccess);

                    Stdin->CloseRead();
                    Stdin->CloseWrite();

                    Stdout->CloseRead();
                    Stdout->CloseWrite();

                    Bridge->CloseRead();
                    Bridge->CloseWrite();

                    std::wstring szStatus = ctx->GetString(0x00130006) + L" (" + std::to_wstring(::GetLastError()) + L")";
                    ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), szStatus);
                    ctx->ItemProgress(dcl.nItemId, -1, true, true);
                    return false;
                }
            }

            m_down.unlock();

            // close unused pipe handles
            Stdin->CloseRead();
            Stdout->CloseWrite();
            Bridge->CloseWrite();
            Bridge->CloseRead();

            // create read thread
            readContext->bError = false;
            readContext->bFinished = false;
            readContext->szFileName = dcl.szInputFile;
            readContext->nIndex = dcl.nItemId;

            readThread = std::thread([&]() { readContext->ReadLoop(ctx, Stdin.get()); });

            // create write thread
            writeContext->bError = false;
            writeContext->bFinished = false;
            writeContext->szFileName = ecl.szOutputFile;
            writeContext->nIndex = ecl.nItemId;

            writeThread = std::thread([&]() { writeContext->WriteLoop(ctx, Stdout.get()); });

            // wait for read thread to finish after write thread finished
            readThread.join();

            // NOTE: Handle is closed in ReadThread.
            //Stdin->CloseWrite();

            if ((readContext->bError == true) || (readContext->bFinished == false))
            {
                Stdout->CloseRead();

                // read thread failed so terminate write thread
                writeThread.join();
            }
            else
            {
                Stdout->CloseRead();

                // wait for write thread to finish
                writeThread.join();
            }

            // check for result from read and write thread
            if ((readContext->bError == false) && (readContext->bFinished == true)
                && (writeContext->bError == false) && (writeContext->bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;

            timer.Stop();

            if (decoderProcess->Stop(nProgress == 100, dcl.format.nExitCodeSuccess) == false)
                nProgress = -1;

            if (encoderProcess->Stop(nProgress == 100, ecl.format.nExitCodeSuccess) == false)
                nProgress = -1;

            if (nProgress != 100)
            {
                ctx->ItemStatus(dcl.nItemId, ctx->GetString(0x00150001), ctx->GetString(0x00130009));
                ctx->ItemProgress(dcl.nItemId, -1, true, true);
                return false;
            }
            else
            {
                ctx->ItemStatus(dcl.nItemId, util::CTimeCount::Format(timer.ElapsedTime()), ctx->GetString(0x0013000B));
                ctx->ItemProgress(dcl.nItemId, 100, true, false);
                return true;
            }
        }
    };

    class CWorker : public IWorker
    {
    public:
        std::unique_ptr<IConverter> ConsoleConverter;
        std::unique_ptr<IConverter> PipesConverter;
        std::unique_ptr<ITranscoder> PipesTranscoder;
    public:
        bool Transcode(IWorkerContext* ctx, config::CItem& item, CCommandLine& dcl, CCommandLine& ecl, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            try
            {
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000C));
                item.ResetProgress();

                bool bResult = PipesTranscoder->Run(ctx, dcl, ecl, m_down);
                if (bResult == true)
                {
                    if (config->m_Options.bDeleteSourceFiles == true)
                        config->FileSystem->DeleteFile_(dcl.szInputFile);

                    return true;
                }

                if (config->m_Options.bDeleteOnErrors == true)
                    config->FileSystem->DeleteFile_(ecl.szOutputFile);

                return false;
            }
            catch (...)
            {
                if (config->m_Options.bDeleteOnErrors == true)
                    config->FileSystem->DeleteFile_(ecl.szOutputFile);

                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000E));
                ctx->ItemProgress(item.nId, -1, true, true);
            }
            return false;
        }
        bool Decode(IWorkerContext* ctx, config::CItem& item, CCommandLine& cl, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            try
            {
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140007));
                item.ResetProgress();

                bool bUseConsole = (cl.bUseReadPipes == false) && (cl.bUseWritePipes == false);
                bool bResult = false;
                if (bUseConsole)
                    bResult = ConsoleConverter->Run(ctx, cl, m_down);
                else
                    bResult = PipesConverter->Run(ctx, cl, m_down);
                if (bResult == false)
                {
                    if (config->m_Options.bDeleteOnErrors == true)
                        config->FileSystem->DeleteFile_(cl.szOutputFile);

                    return false;
                }

                if (config->FileSystem->FileExists(cl.szOutputFile) == false)
                {
                    ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140008));
                    return false;
                }

                return true;
            }
            catch (...)
            {
                if (config->m_Options.bDeleteOnErrors == true)
                    config->FileSystem->DeleteFile_(cl.szOutputFile);

                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140009));
                ctx->ItemProgress(item.nId, -1, true, true);
            }
            return false;
        }
        bool Encode(IWorkerContext* ctx, config::CItem& item, CCommandLine& cl, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            try
            {
                if (cl.format.nType == config::FormatType::Encoder)
                    ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000A));
                else if (cl.format.nType == config::FormatType::Decoder)
                    ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000B));

                item.ResetProgress();

                bool bUseConsole = (cl.bUseReadPipes == false) && (cl.bUseWritePipes == false);
                bool bResult = false;
                if (bUseConsole)
                    bResult = ConsoleConverter->Run(ctx, cl, m_down);
                else
                    bResult = PipesConverter->Run(ctx, cl, m_down);
                if (bResult == true)
                {
                    if (config->m_Options.bDeleteSourceFiles == true)
                        config->FileSystem->DeleteFile_(cl.szInputFile);

                    return true;
                }

                if (config->m_Options.bDeleteOnErrors == true)
                    config->FileSystem->DeleteFile_(cl.szOutputFile);

                return false;
            }
            catch (...)
            {
                if (config->m_Options.bDeleteOnErrors == true)
                    config->FileSystem->DeleteFile_(cl.szOutputFile);

                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000E));
                ctx->ItemProgress(item.nId, -1, true, true);
            }
            return false;
        }
        bool Convert(IWorkerContext* ctx, config::CItem& item, std::mutex& m_dir, std::mutex& m_down)
        {
            auto config = ctx->pConfig;
            COutputPath m_Output;

            if (item.m_Paths.size() <= 0)
            {
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140001));
                return false;
            }

            std::wstring szInputFile = item.m_Paths[0].szPath;
            if (config->FileSystem->FileExists(szInputFile) == false)
            {
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140001));
                return false;
            }

            int nEncoder = config::CFormat::GetFormatById(config->m_Formats, item.szFormatId);
            if (nEncoder == -1)
            {
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140002));
                return false;
            }

            auto& ef = config->m_Formats[nEncoder];
            if (item.nPreset >= ef.m_Presets.size())
            {
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140003));
                return false;
            }

            std::wstring szOutputFile = m_Output.CreateFilePath(config->FileSystem.get(), config->m_Options.szOutputPath, szInputFile, item.szName, ef.szOutputExtension);
            if (config->m_Options.bOverwriteExistingFiles == false)
            {
                if (config->m_Options.bRenameExistingFiles == true)
                {
                    int nCounter = 0;
                    while (config->FileSystem->FileExists(szOutputFile) == true)
                    {
                        if (nCounter >= config->m_Options.nRenameExistingFilesLimit)
                        {
                            ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140010));
                            return false;
                        }

                        CInputPath m_Input(szOutputFile.c_str());
                        szOutputFile = m_Input.AppendInputName(L"_");

                        nCounter++;
                    }
                }
                else
                {
                    if (config->FileSystem->FileExists(szOutputFile) == true)
                    {
                        ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140010));
                        return false;
                    }
                }
            }

            m_dir.lock();
            if (m_Output.CreateOutputPath(config->FileSystem.get(), szOutputFile) == false)
            {
                m_dir.unlock();
                ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x0014000F));
                return false;
            }
            m_dir.unlock();

            config->FileSystem->SetCurrentDirectory_(config->m_Settings.szSettingsPath);

            bool bCanEncode = config::CFormat::IsValidInputExtension(ef.szInputExtensions, config->FileSystem->GetFileExtension(szInputFile));
            if (bCanEncode == false)
            {
                int nDecoder = config::CFormat::GetDecoderByExtensionAndFormat(config->m_Formats, item.szExtension, ef);
                if (nDecoder == -1)
                {
                    ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140004));
                    return false;
                }

                auto& df = config->m_Formats[nDecoder];
                if (df.nDefaultPreset >= df.m_Presets.size())
                {
                    ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140005));
                    return false;
                }

                bool bCanDecode = config::CFormat::IsValidInputExtension(ef.szInputExtensions, df.szOutputExtension);
                if (bCanDecode == false)
                {
                    ctx->ItemStatus(item.nId, ctx->GetString(0x00150001), ctx->GetString(0x00140006));
                    return false;
                }

                std::wstring szPath = config->FileSystem->GetFilePath(szOutputFile);
                std::wstring szName = config->FileSystem->GenerateUuidString();
                std::wstring szExt = util::string::TowLower(df.szOutputExtension);
                std::wstring szDecodedFile = szPath + szName + L"." + szExt;

                auto dcl = CCommandLine(config->FileSystem.get(), df, df.nDefaultPreset, item.nId, szInputFile, szDecodedFile, L"");
                auto ecl = CCommandLine(config->FileSystem.get(), ef, item.nPreset, item.nId, szDecodedFile, szOutputFile, item.szOptions);

                if (ctx->bRunning == false)
                    return false;

                if (df.bPipeInput && df.bPipeOutput && ef.bPipeInput && ef.bPipeOutput)
                {
                    return Transcode(ctx, item, dcl, ecl, m_down);
                }

                if (Decode(ctx, item, dcl, m_down) == false)
                    return false;

                if (ctx->bRunning == false)
                    return false;

                bool bResult = Encode(ctx, item, ecl, m_down);
                if (bResult == true)
                {
                    if (config->m_Options.bDeleteSourceFiles == true)
                        config->FileSystem->DeleteFile_(dcl.szInputFile);
                }

                config->FileSystem->DeleteFile_(dcl.szOutputFile);
                return bResult;
            }

            if (ctx->bRunning == false)
                return false;

            auto cl = CCommandLine(config->FileSystem.get(), ef, item.nPreset, item.nId, szInputFile, szOutputFile, item.szOptions);

            return Encode(ctx, item, cl, m_down);
        }
        bool Convert(IWorkerContext* ctx, std::vector<int>& ids, std::mutex &m_dir, std::mutex &m_down)
        {
            auto config = ctx->pConfig;
            try
            {
                for (int id : ids)
                {
                    if (ctx->bRunning == false)
                        return false;

                    ctx->TotalProgress(id);

                    if (Convert(ctx, config->m_Items[id], m_dir, m_down) == true)
                    {
                        ctx->nProcessedFiles++;
                        ctx->TotalProgress(id);
                    }
                    else
                    {
                        ctx->nProcessedFiles++;
                        ctx->nErrors++;
                        ctx->TotalProgress(id);
                        if (config->m_Options.bStopOnErrors == true)
                            return false;
                    }

                    if (ctx->bRunning == false)
                        return false;
                }
            }
            catch (...)
            {
                return false;
            }
            return true;
        }
        bool Convert(IWorkerContext* ctx, std::queue<int>& queue, std::mutex& m_queue, std::mutex &m_dir, std::mutex &m_down)
        {
            auto config = ctx->pConfig;
            while (true)
            {
                try
                {
                    m_queue.lock();
                    if (!queue.empty())
                    {
                        int id = queue.front();
                        queue.pop();
                        m_queue.unlock();

                        if (ctx->bRunning == false)
                            return false;

                        ctx->TotalProgress(id);

                        if (Convert(ctx, config->m_Items[id], m_dir, m_down) == true)
                        {
                            ctx->nProcessedFiles++;
                            ctx->TotalProgress(id);
                        }
                        else
                        {
                            ctx->nProcessedFiles++;
                            ctx->nErrors++;
                            ctx->TotalProgress(id);
                            if (config->m_Options.bStopOnErrors == true)
                                return false;
                        }

                        if (ctx->bRunning == false)
                            return false;
                    }
                    else
                    {
                        m_queue.unlock();
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
        void Convert(IWorkerContext* ctx, config::CItem& item)
        {
            std::mutex m_dir;
            std::mutex m_down;

            ctx->Start();
            ctx->nTotalFiles = 1;
            ctx->TotalProgress(item.nId);

            if (Convert(ctx, item, m_dir, m_down) == true)
            {
                ctx->nProcessedFiles = 1;
                ctx->nErrors = 0;
                ctx->TotalProgress(item.nId);
            }
            else
            {
                ctx->nProcessedFiles = 1;
                ctx->nErrors = 1;
                ctx->TotalProgress(item.nId);
            }

            ctx->Stop();
            ctx->bDone = true;
        }
        void Convert(IWorkerContext* ctx, std::vector<config::CItem>& items)
        {
            if (ctx->nThreadCount <= 1)
            {
                std::mutex m_dir;
                std::mutex m_down;
                std::vector<int> ids;

                ctx->Start();

                for (auto& item : items)
                {
                    if (item.bChecked == true)
                    {
                        item.ResetProgress();
                        ids.emplace_back(item.nId);
                        ctx->nTotalFiles++;
                    }
                }

                this->Convert(ctx, ids, m_dir, m_down);

                ctx->Stop();
                ctx->bDone = true;
            }
            else
            {
                std::mutex m_dir;
                std::mutex m_down;
                std::mutex m_queue;
                std::queue<int> queue;

                ctx->Start();

                for (auto& item : items)
                {
                    if (item.bChecked == true)
                    {
                        item.ResetProgress();
                        queue.push(item.nId);
                        ctx->nTotalFiles++;
                    }
                }

                auto convert = [&]() { this->Convert(ctx, queue, m_queue, m_dir, m_down); };
                auto threads = std::make_unique<std::thread[]>(ctx->nThreadCount);

                for (int i = 0; i < ctx->nThreadCount; i++)
                {
                    threads[i] = std::thread(convert);
                }

                for (int i = 0; i < ctx->nThreadCount; i++)
                {
                    threads[i].join();
                }

                ctx->Stop();
                ctx->bDone = true;
            }
        }
    };
}
