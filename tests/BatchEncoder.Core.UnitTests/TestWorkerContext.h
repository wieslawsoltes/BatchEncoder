// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "worker\WorkerContext.h"

namespace BatchEncoderCoreUnitTests
{
    using namespace worker;

    class TestDownloader : public IDownloader
    {
    public:
        bool Download(IWorkerContext* ctx, config::CFormat& format, int nItemId)
        {
            return true;
        }
    };

    class TestProcess : public IProcess
    {
    public:
        void ConnectStdInput(void* hPipeStdin)
        {
        }
        void ConnectStdOutput(void* hPipeStdout)
        {
        }
        void ConnectStdError(void* hPipeStderr)
        {
        }
        bool Start(const std::wstring& szCommandLine, bool bNoWindow)
        {
            return true;
        }
        bool Wait()
        {
            return true;
        }
        bool Wait(int milliseconds)
        {
            return true;
        }
        bool Terminate(int code = 0)
        {
            return true;
        }
        bool Close()
        {
            return true;
        }
        bool Stop(bool bWait, int nExitCodeSucess)
        {
            return true;
        }
    public:
        void* StdinHandle()
        {
            return nullptr;
        }
        void* StdoutHandle()
        {
            return nullptr;
        }
        void* StderrHandle()
        {
            return nullptr;
        }
    };

    class TestPipe : public IPipe
    {
    public:
        bool Create()
        {
            return true;
        }
        void CloseRead()
        {
        }
        void CloseWrite()
        {
        }
        bool InheritRead()
        {
            return true;
        }
        bool InheritWrite()
        {
            return true;
        }
        bool DuplicateRead()
        {
            return true;
        }
        bool DuplicateWrite()
        {
            return true;
        }
        void* ReadHandle()
        {
            return nullptr;
        }
        void* WriteHandle()
        {
            return nullptr;
        }
    };

    class TestPipeToFileWriter : public IFileWriter
    {
    public:
        bool WriteLoop(IWorkerContext* ctx, IPipe* Stdout)
        {
            return true;
        }
    };

    class TestFileToPipeReader : public IFileReader
    {
    public:
        bool ReadLoop(IWorkerContext* ctx, IPipe* Stdin)
        {
            return true;
        }
    };

    class TestOutputParser : public IOutputParser
    {
    public:
        bool Open(IWorkerContext* ctx, const std::wstring& szFunction)
        {
            return true;
        }
        bool Parse(IWorkerContext* ctx, const char *szLine)
        {
            return true;
        }
    };

    class TestPipeToStringWriter : public IStringWriter
    {
    public:
        bool WriteLoop(IWorkerContext* ctx, IPipe* Stderr, IOutputParser* parser)
        {
            return true;
        }
    };

    class TestFileSystem : public util::IFileSystem
    {
    public:
        std::wstring GenerateUuidString()
        {
            return util::GenerateUuidString();
        }
        std::wstring CombinePath(const std::wstring& szPath, const std::wstring& szFile)
        {
            return util::CombinePath(szPath, szFile);
        }
        std::wstring GetFileName(const std::wstring& szFilePath)
        {
            return util::GetFileName(szFilePath);
        }
        std::wstring GetFilePath(const std::wstring& szFilePath)
        {
            return util::GetFilePath(szFilePath);
        }
        std::wstring GetFileExtension(const std::wstring& szFilePath)
        {
            return util::GetFileExtension(szFilePath);
        }
        std::wstring GetOnlyFileName(const std::wstring& szFilePath)
        {
            return util::GetOnlyFileName(szFilePath);
        }
        __int64 GetFileSize64(void* hFile)
        {
            return util::GetFileSize64(hFile);
        }
        __int64 GetFileSize64(const std::wstring& szFileName)
        {
            return util::GetFileSize64(szFileName);
        }
        __int64 GetFileSizeInt64(FILE *fp)
        {
            return util::GetFileSizeInt64(fp);
        }
        std::wstring GetExeFilePath()
        {
            return util::GetExeFilePath();
        }
        std::wstring GetSettingsFilePath(const std::wstring& szFileName, const std::wstring& szConfigDirectory)
        {
            return util::GetSettingsFilePath(szFileName, szConfigDirectory);
        }
        std::wstring GetFullPathName_(const std::wstring& szFilePath)
        {
            return util::GetFullPathName_(szFilePath);
        }
        bool FileExists(const std::wstring& szPath)
        {
            return util::FileExists(szPath);
        }
        bool PathFileExists_(const std::wstring& szFilePath)
        {
            return util::PathFileExists_(szFilePath);
        }
        void DeleteFile_(const std::wstring& szFilePath)
        {
            util::DeleteFile_(szFilePath);
        }
        bool CreateDirectory_(const std::wstring& szPath)
        {
            return util::CreateDirectory_(szPath);
        }
        std::wstring GetCurrentDirectory_()
        {
            return util::GetCurrentDirectory_();
        }
        void SetCurrentDirectory_(const std::wstring& szPath)
        {
            util::SetCurrentDirectory_(szPath);
        }
        bool DirectoryExists(const std::wstring& szPath)
        {
            return util::DirectoryExists(szPath);
        }
        bool MakeFullPath(const std::wstring& szTargetPath)
        {
            return util::MakeFullPath(szTargetPath);
        }
        std::vector<std::wstring> FindFiles(const std::wstring& pattern)
        {
            return util::FindFiles(pattern);
        }
        bool FindFiles(const std::wstring path, std::vector<std::wstring>& files, const bool bRecurse = false)
        {
            return util::FindFiles(path, files, bRecurse);
        }
    };

    class TestWorkerFactory : public IWorkerFactory
    {
    public:
        std::shared_ptr<IDownloader> CreateDownloaderPtr()
        {
            return std::make_shared<TestDownloader>();
        }
        std::shared_ptr<IProcess> CreateProcessPtr()
        {
            return std::make_shared<TestProcess>();
        }
        std::shared_ptr<IPipe> CreatePipePtr()
        {
            return std::make_shared<TestPipe>();
        }
        std::shared_ptr<IFileReader> CreateFileReaderPtr()
        {
            return std::make_shared<TestFileToPipeReader>();
        }
        std::shared_ptr<IFileWriter> CreateFileWriterPtr()
        {
            return std::make_shared<TestPipeToFileWriter>();
        }
        std::shared_ptr<IOutputParser> CreateOutputParserPtr()
        {
            return std::make_shared<TestOutputParser>();
        }
        std::shared_ptr<IStringWriter> CreateStringWriterPtr()
        {
            return std::make_shared<TestPipeToStringWriter>();
        }
    };

    class TestWorkerContext : public IWorkerContext
    {
    public:
        TestWorkerContext()
        {
            this->bDone = true;
            this->bRunning = false;
            this->pConfig = nullptr;
            this->pFactory = std::make_shared<TestWorkerFactory>();
        }
        virtual ~TestWorkerContext() { }
    public:
        std::wstring GetString(int nKey)
        {
            return pConfig->GetString(nKey);
        }
        void Init()
        {
            this->bRunning = true;
            this->bDone = false;
            this->nTotalFiles = 0;
            this->nProcessedFiles = 0;
            this->nErrors = 0;
            this->nLastItemId = -1;
        }
        void Start()
        {
        }
        void Stop()
        {
            this->pConfig = nullptr;
            this->bRunning = false;
        }
        bool ItemProgress(int nItemId, int nProgress, bool bFinished, bool bError = false)
        {
            if (bError == true)
            {
                if (this->pConfig->m_Options.bStopOnErrors == true)
                {
                    this->bRunning = false;
                }
                return this->bRunning;
            }

            if (bFinished == true)
            {
            }

            if ((bFinished == false) && (this->bRunning == true))
            {
            }

            return this->bRunning;
        }
        void ItemStatus(int nItemId, const std::wstring& szTime, const std::wstring& szStatus)
        {
        }
        void TotalProgress(int nItemId)
        {
            if (nItemId > this->nLastItemId)
            {
                this->nLastItemId = nItemId;
            }
        }
    };
}
