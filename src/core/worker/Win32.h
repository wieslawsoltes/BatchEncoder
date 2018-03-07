// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <memory>
#include <utility>
#include <string>
#include <cstring>
#include "utilities\FileSystem.h"
#include "utilities\Log.h"
#include "utilities\Pipe.h"
#include "utilities\Process.h"
#include "utilities\String.h"
#include "utilities\Utilities.h"
#include "ToolDownloader.h"
#include "LuaProgess.h"
#include "WorkerContext.h"

namespace worker
{
    class Win32Downloader : public IDownloader
    {
    public:
        bool Download(IWorkerContext* ctx, config::CFormat& format, int nItemId)
        {
            auto config = ctx->pConfig;
            CToolDownloader m_Downloader;
            int nTool = config::CTool::GetToolByPath(config->m_Tools, format.szPath);
            if (nTool < 0)
            {
                nTool = config::CConfig::FindTool(config->m_Tools, format.szId);
            }
            if (nTool >= 0)
            {
                config::CTool& tool = config->m_Tools[nTool];
                auto callback = [&](int nIndex, std::wstring szStatus) -> bool
                {
                    ctx->ItemStatus(nItemId, ctx->GetString(0x00150001), szStatus);
                    if (ctx->bRunning == false)
                        return true;
                    return false;
                };
                return m_Downloader.Download(config, tool, true, true, nTool, callback);
            }
            return false;
        }
    };

    class Win32Process : public IProcess
    {
        util::CProcess process;
    public:
        void ConnectStdInput(void* hPipeStdin)
        {
            process.ConnectStdInput(hPipeStdin);
        }
        void ConnectStdOutput(void* hPipeStdout)
        {
            process.ConnectStdOutput(hPipeStdout);
        }
        void ConnectStdError(void* hPipeStderr)
        {
            process.ConnectStdError(hPipeStderr);
        }
        bool Start(const std::wstring& szCommandLine, bool bNoWindow)
        {
            return process.Start(szCommandLine, bNoWindow);
        }
        bool Wait()
        {
            return process.Wait();
        }
        bool Wait(int milliseconds)
        {
            return process.Wait(milliseconds);
        }
        bool Terminate(int code = 0)
        {
            return process.Terminate(code);
        }
        bool Close()
        {
            return process.Close();
        }
        bool Stop(bool bWait, int nExitCodeSucess)
        {
            return process.Stop(bWait, nExitCodeSucess);
        }
    public:
        void* StdinHandle()
        {
            return ::GetStdHandle(STD_INPUT_HANDLE);
        }
        void* StdoutHandle()
        {
            return ::GetStdHandle(STD_OUTPUT_HANDLE);
        }
        void* StderrHandle()
        {
            return ::GetStdHandle(STD_ERROR_HANDLE);
        }
    };

    class Win32Pipe : public IPipe
    {
        util::CPipe pipe;
    public:
        bool Create()
        {
            return pipe.Create();
        }
        void CloseRead()
        {
            pipe.CloseRead();
        }
        void CloseWrite()
        {
            pipe.CloseWrite();
        }
        bool InheritRead()
        {
            return pipe.InheritRead();
        }
        bool InheritWrite()
        {
            return pipe.InheritWrite();
        }
        bool DuplicateRead()
        {
            return pipe.DuplicateRead();
        }
        bool DuplicateWrite()
        {
            return pipe.DuplicateWrite();
        }        
        void* ReadHandle()
        {
            return pipe.hRead;
        }
        void* WriteHandle()
        {
            return pipe.hWrite;
        }
    };

    class CPipeToFileWriter : public IFileWriter
    {
    public:
        bool WriteLoop(IWorkerContext* ctx, IPipe* Stdout)
        {
            HANDLE hPipe = Stdout->ReadHandle();
            HANDLE hFile = INVALID_HANDLE_VALUE;
            BYTE pReadBuff[4096];
            BOOL bRes = FALSE;
            DWORD dwReadBytes = 0;
            DWORD dwWriteBytes = 0;
            ULONGLONG nTotalBytesWrite = 0;

            bError = false;
            bFinished = false;

            hFile = ::CreateFile(szFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                bError = true;
                bFinished = true;
                return false;
            }

            do
            {
                ::Sleep(0);

                DWORD dwAvailableBytes;
                if (FALSE == PeekNamedPipe(hPipe, 0, 0, 0, &dwAvailableBytes, 0))
                    break;

                if (dwAvailableBytes > 0)
                {
                    bRes = ::ReadFile(hPipe, pReadBuff, 4096, &dwReadBytes, 0);
                    if ((bRes == FALSE) || (dwReadBytes == 0))
                        break;

                    bRes = ::WriteFile(hFile, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
                    if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
                        break;

                    nTotalBytesWrite += dwReadBytes;
                }
                else
                    bRes = TRUE;

                if (ctx->bRunning == false)
                    break;
            } while (bRes != FALSE);

            ::CloseHandle(hFile);

            if (nTotalBytesWrite <= 0)
            {
                bError = true;
                bFinished = true;
                return false;
            }
            else
            {
                bError = false;
                bFinished = true;
                return true;
            }
        }
    };

    class CFileToPipeReader : public IFileReader
    {
    public:
        bool ReadLoop(IWorkerContext* ctx, IPipe* Stdin)
        {
            HANDLE hPipe = Stdin->WriteHandle();
            HANDLE hFile = INVALID_HANDLE_VALUE;
            BYTE pReadBuff[4096];
            BOOL bRes = FALSE;
            DWORD dwReadBytes = 0;
            DWORD dwWriteBytes = 0;
            ULONGLONG nTotalBytesRead = 0;
            ULONGLONG nFileSize = 0;
            int nProgress = -1;
            int nPreviousProgress = -1;
            bool bRunning = true;

            bError = false;
            bFinished = false;

            hFile = ::CreateFile(szFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                bError = true;
                bFinished = true;
                ::CloseHandle(hPipe);
                return false;
            }

            nFileSize = util::GetFileSize64(hFile);
            if (nFileSize == 0)
            {
                bError = true;
                bFinished = true;
                ::CloseHandle(hFile);
                ::CloseHandle(hPipe);
                return false;
            }

            do
            {
                bRes = ::ReadFile(hFile, pReadBuff, 4096, &dwReadBytes, 0);
                if ((bRes == FALSE) || (dwReadBytes == 0))
                    break;

                ::Sleep(0);

                bRes = ::WriteFile(hPipe, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
                if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
                    break;

                nTotalBytesRead += dwReadBytes;
                nProgress = (int)((nTotalBytesRead * 100) / nFileSize);

                if (nProgress != nPreviousProgress)
                {
                    bRunning = ctx->ItemProgress(nIndex, nProgress, false);
                    nPreviousProgress = nProgress;
                }

                if ((ctx->bRunning == false) || (bRunning == false))
                    break;
            } while (bRes != FALSE);

            ::CloseHandle(hFile);
            ::CloseHandle(hPipe);

            if (nTotalBytesRead != nFileSize)
            {
                bError = true;
                bFinished = true;
                return false;
            }
            else
            {
                bError = false;
                bFinished = true;
                return true;
            }
        }
    };

    class CDebugOutputParser : public IOutputParser
    {
    public:
        bool Open(IWorkerContext* ctx, const std::wstring& szFunction)
        {
            return true;
        }
        bool Parse(IWorkerContext* ctx, const char *szLine)
        {
            OutputDebugStringA(szLine);
            OutputDebugStringA("\n");
            return ctx->bRunning;
        }
    };

    class CLuaOutputParser : public IOutputParser
    {
        CLuaProgess luaProgress;
    //public:
    //    util::ILog* log = nullptr;
    public:
        bool Open(IWorkerContext* ctx, const std::wstring& szFunction)
        {
            std::string szAnsiFunction = util::string::Convert(szFunction);

            if (this->luaProgress.Open(szAnsiFunction.c_str()) == false)
            {
                ctx->ItemStatus(nIndex, ctx->GetString(0x00150001), ctx->GetString(0x00110001));
                ctx->ItemProgress(nIndex, -1, true, true);
                return false;
            }

            if (this->luaProgress.Init() == false)
            {
                ctx->ItemStatus(nIndex, ctx->GetString(0x00150001), ctx->GetString(0x00110002));
                ctx->ItemProgress(nIndex, -1, true, true);
                return false;
            }

            return true;
        }
        bool Parse(IWorkerContext* ctx, const char *szLine)
        {
            //if (log != nullptr)
            //{
            //    std::wstring szUnicode = util::string::Convert(szLine);
            //    log->Log(szUnicode, false);
            //}

            int nRet = (int)this->luaProgress.GetProgress(szLine);
            if (nRet != -1)
            {
                this->nProgress = nRet;
            }

            if (this->nProgress != this->nPreviousProgress)
            {
                nPreviousProgress = nProgress;
                return ctx->ItemProgress(nIndex, nProgress, false);
            }

            return ctx->bRunning;
        }
    };

    class CPipeToStringWriter : public IStringWriter
    {
    public:
        bool WriteLoop(IWorkerContext* ctx, IPipe* Stderr, IOutputParser* parser)
        {
            const int nBuffSize = 4096;
            char szReadBuff[nBuffSize];
            char szLineBuff[nBuffSize];
            DWORD dwReadBytes = 0L;
            BOOL bRes = FALSE;
            bool bLineStart = false;
            bool bLineEnd = false;
            bool bRunning = true;
            int nLineLen = 0;

            bError = false;
            bFinished = false;

            std::memset(szReadBuff, 0, sizeof(szReadBuff));
            std::memset(szLineBuff, 0, sizeof(szLineBuff));

            do
            {
                std::memset(szReadBuff, 0, sizeof(szReadBuff));
                bRes = ::ReadFile(Stderr->ReadHandle(), szReadBuff, 100, &dwReadBytes, 0);
                if (bRes == FALSE || dwReadBytes == 0)
                    break;

                szReadBuff[dwReadBytes] = '\0';

                for (int i = 0; i < (int)dwReadBytes; i++)
                {
                    if (szReadBuff[i] == '\r') // '\r'
                    {
                        if ((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if (szReadBuff[i] == '\n')
                    {
                    }
                    else if (szReadBuff[i] == '\t')
                    {
                    }
                    else if (szReadBuff[i] == '\b')
                    {
                        if ((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if (bLineEnd == false)
                    {
                        bLineStart = true;
                        nLineLen++;
                        if (nLineLen > nBuffSize)
                        {
                            ctx->ItemStatus(nIndex, ctx->GetString(0x00150001), ctx->GetString(0x00110003));
                            ctx->ItemProgress(nIndex, -1, true, true);
                            bError = true;
                            bFinished = true;
                            return false;
                        }
                        szLineBuff[nLineLen - 1] = szReadBuff[i];
                    }

                    if ((bLineEnd == true) && (bLineStart == false))
                    {
                        if (strlen(szLineBuff) > 0)
                        {
                            bRunning = parser->Parse(ctx, szLineBuff);
                            std::memset(szLineBuff, 0, sizeof(szLineBuff));
                            if ((ctx->bRunning == false) || (bRunning == false))
                                break;
                        }
                        nLineLen = 0;
                        bLineStart = true;
                        bLineEnd = false;
                    }
                }

                if (bRunning == false)
                    break;
            } while (bRes);

            bError = false;
            bFinished = true;
            return true;
        }
    };

    class Win32FileSystem : public util::IFileSystem
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

    class Win32WorkerFactory : public IWorkerFactory
    {
    public:
        std::shared_ptr<IDownloader> CreateDownloaderPtr()
        {
            return std::make_shared<Win32Downloader>();
        }
        std::shared_ptr<IProcess> CreateProcessPtr()
        {
            return std::make_shared<Win32Process>();
        }
        std::shared_ptr<IPipe> CreatePipePtr()
        {
            return std::make_shared<Win32Pipe>();
        }
        std::shared_ptr<IFileReader> CreateFileReaderPtr()
        {
            return std::make_shared<CFileToPipeReader>();
        }
        std::shared_ptr<IFileWriter> CreateFileWriterPtr()
        {
            return std::make_shared<CPipeToFileWriter>();
        }
        std::shared_ptr<IOutputParser> CreateOutputParserPtr()
        {
            return std::make_shared<CLuaOutputParser>();
        }
        std::shared_ptr<IStringWriter> CreateStringWriterPtr()
        {
            return std::make_shared<CPipeToStringWriter>();
        }
    };
}
