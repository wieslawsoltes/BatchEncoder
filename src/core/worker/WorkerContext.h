// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <utility>
#include <memory>
#include "config\Config.h"

namespace worker
{
    class IWorkerContext;

    class IDownloader
    {
    public:
        virtual ~IDownloader() { };
        virtual bool Download(IWorkerContext* ctx, config::CFormat& format, int nItemId) = 0;
    };

    class IProcess
    {
    public:
        virtual ~IProcess() { };
        virtual void ConnectStdInput(void* hPipeStdin) = 0;
        virtual void ConnectStdOutput(void* hPipeStdout) = 0;
        virtual void ConnectStdError(void* hPipeStderr) = 0;
        virtual bool Start(const std::wstring& szCommandLine, bool bNoWindow) = 0;
        virtual bool Wait() = 0;
        virtual bool Wait(int milliseconds) = 0;
        virtual bool Terminate(int code = 0) = 0;
        virtual bool Close() = 0;
        virtual bool Stop(bool bWait, int nExitCodeSucess) = 0;
    public:
        virtual void* StdinHandle() = 0;
        virtual void* StdoutHandle() = 0;
        virtual void* StderrHandle() = 0;
    };

    class IPipe
    {
    public:
        virtual ~IPipe() { }
        virtual bool Create() = 0;
        virtual void CloseRead() = 0;
        virtual void CloseWrite() = 0;
        virtual bool InheritRead() = 0;
        virtual bool InheritWrite() = 0;
        virtual bool DuplicateRead() = 0;
        virtual bool DuplicateWrite() = 0;
        virtual void* ReadHandle() = 0;
        virtual void* WriteHandle() = 0;
    };

    class IFileReader
    {
    public:
        std::wstring szFileName;
        int nIndex;
        volatile bool bError;
        volatile bool bFinished;
    public:
        virtual ~IFileReader() { };
        virtual bool ReadLoop(IWorkerContext* ctx, IPipe* Stdin) = 0;
    };

    class IFileWriter
    {
    public:
        std::wstring szFileName;
        int nIndex;
        volatile bool bError;
        volatile bool bFinished;
    public:
        virtual ~IFileWriter() { };
        virtual bool WriteLoop(IWorkerContext* ctx, IPipe* Stdout) = 0;
    };

    class IOutputParser
    {
    public:
        int nIndex;
        int nProgress;
        int nPreviousProgress;
    public:
        virtual ~IOutputParser() { }
        virtual bool Open(IWorkerContext* ctx, const std::wstring& szFunction) = 0;
        virtual bool Parse(IWorkerContext* ctx, const char *szLine) = 0;
    };

    class IStringWriter
    {
    public:
        int nIndex;
        volatile bool bError;
        volatile bool bFinished;
    public:
        virtual ~IStringWriter() { };
        virtual bool WriteLoop(IWorkerContext* ctx, IPipe* Stdout, IOutputParser* parser) = 0;
    };

    class IWorkerFactory
    {
    public:
        virtual ~IWorkerFactory() { };
        virtual std::shared_ptr<IDownloader> CreateDownloaderPtr() = 0;
        virtual std::shared_ptr<IProcess> CreateProcessPtr() = 0;
        virtual std::shared_ptr<IPipe> CreatePipePtr() = 0;
        virtual std::shared_ptr<IFileReader> CreateFileReaderPtr() = 0;
        virtual std::shared_ptr<IFileWriter> CreateFileWriterPtr() = 0;
        virtual std::shared_ptr<IOutputParser> CreateOutputParserPtr() = 0;
        virtual std::shared_ptr<IStringWriter> CreateStringWriterPtr() = 0;
    };

    class IWorkerContext
    {
    public:
        volatile bool bRunning;
        volatile bool bDone;
        volatile int nTotalFiles;
        volatile int nProcessedFiles;
        volatile int nErrors;
        volatile int nLastItemId;
    public:
        int nThreadCount;
        config::CConfig* pConfig;
    public:
        std::shared_ptr<IWorkerFactory> pFactory;
    public:
        virtual ~IWorkerContext() { }
        virtual std::wstring GetString(int nKey) = 0;
        virtual void Init() = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual bool ItemProgress(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
        virtual void ItemStatus(int nItemId, const std::wstring& szTime, const std::wstring& szStatus) = 0;
        virtual void TotalProgress(int nItemId) = 0;
    };
}
