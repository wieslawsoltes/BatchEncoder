// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

bool ConvertFile(CFileContext* pContext)
{
    // TODO:
    // if there is no input pipe and output pipe enabled
    // we can not get progress status for the ProgressBars
    // so for encoder/decoder mode treat this as an error
    // and for trans-coding treat this as decoder to encoder piping

    // TODO: handle bUseWritePipes flag like bUseReadPipes

    // NOTE:
    // if bUseReadPipes = true use pipes to read source file and send the data to console stdin
    // if bUseReadPipes = false create full command-line and read from stdout/stderr conversion progress

    // set the correct security attributes
    SECURITY_ATTRIBUTES secattr;
    ZeroMemory(&secattr, sizeof(secattr));
    secattr.nLength = sizeof(secattr);
    secattr.bInheritHandle = TRUE;

    HANDLE rOutErrPipe = INVALID_HANDLE_VALUE;
    HANDLE wOutErrPipe = INVALID_HANDLE_VALUE;
    HANDLE rInPipe = INVALID_HANDLE_VALUE;
    HANDLE wInPipe = INVALID_HANDLE_VALUE;
    HANDLE rOutPipe = INVALID_HANDLE_VALUE;
    HANDLE wOutPipe = INVALID_HANDLE_VALUE;

    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        // create pipes for stdout and stderr
        BOOL bRet = FALSE;
        bRet = ::CreatePipe(&rOutErrPipe, &wOutErrPipe, &secattr, 0);
        if (bRet == FALSE)
        {
            pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not create pipes for stdout and stderr."));
            pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
            return false;
        }
    }
    else
    {
        if (pContext->bUseReadPipes == true)
        {
            // create pipes for stdin
            BOOL bRet = FALSE;
            bRet = ::CreatePipe(&rInPipe, &wInPipe, &secattr, 0);
            if (bRet == FALSE)
            {
                pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not create pipes for stdin."));
                pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
                return false;
            }
        }

        if (pContext->bUseWritePipes == true)
        {
            // create pipes for stdout
            BOOL bRet = FALSE;
            bRet = ::CreatePipe(&rOutPipe, &wOutPipe, &secattr, 0);
            if (bRet == FALSE)
            {
                if (pContext->bUseReadPipes == true)
                {
                    ::CloseHandle(rInPipe);
                    ::CloseHandle(wInPipe);
                }

                pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not create pipes for stdout."));
                pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
                return false;
            }
        }
    }

    // this pipes are not inherited by child process
    if (pContext->bUseReadPipes == true)
    {
        BOOL bRet = FALSE;
        bRet = ::SetHandleInformation(wInPipe, HANDLE_FLAG_INHERIT, 0);
        if (bRet == FALSE)
        {
            ::CloseHandle(rInPipe);
            ::CloseHandle(wInPipe);

            pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not set stdin pipe to be inherited by child process."));
            pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
            return false;
        }
    }

    if (pContext->bUseWritePipes == true)
    {
        BOOL bRet = FALSE;
        bRet = ::SetHandleInformation(rOutPipe, HANDLE_FLAG_INHERIT, 0);
        if (bRet == FALSE)
        {
            if (pContext->bUseWritePipes == true)
            {
                ::CloseHandle(rInPipe);
                ::CloseHandle(wInPipe);
            }

            ::CloseHandle(rOutPipe);
            ::CloseHandle(wOutPipe);

            pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not set stdout pipe to be inherited by child process."));
            pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
            return false;
        }
    }

    // NOTE: DuplicateHandle prevents child process from closing the pipe
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        // NOTE: this could be used when reading from stderr
        if (::DuplicateHandle(::GetCurrentProcess(),
            rOutErrPipe,
            ::GetCurrentProcess(),
            &rOutErrPipe,
            0,
            TRUE,
            DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS) == FALSE)
        {
            ::CloseHandle(rOutErrPipe);
            ::CloseHandle(wOutErrPipe);

            pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not duplicate stderr pipe to prevent child process from closing the pipe."));
            pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
            return false;
        }
    }

    STARTUPINFO sInfo;
    ZeroMemory(&sInfo, sizeof(sInfo));

    PROCESS_INFORMATION pInfo;
    ZeroMemory(&pInfo, sizeof(pInfo));

    sInfo.cb = sizeof(sInfo);
    sInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    // TODO: 
    // when read pipes are disabled and write pipes enabled
    // we can try to get stderr progress from console
    // this was tested not tested with command-line tools

    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        sInfo.hStdInput = NULL;
        sInfo.hStdOutput = wOutErrPipe;
        sInfo.hStdError = wOutErrPipe;
    }
    else
    {
        if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == false))
        {
            sInfo.hStdInput = rInPipe;
            sInfo.hStdOutput = NULL;
            sInfo.hStdError = NULL;
        }
        else if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == true))
        {
            sInfo.hStdInput = NULL;
            sInfo.hStdOutput = wOutPipe;
            sInfo.hStdError = NULL;
        }
        else if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
        {
            sInfo.hStdInput = rInPipe;
            sInfo.hStdOutput = wOutPipe;
            sInfo.hStdError = NULL;
        }
    }

    int nProgress = 0;
    int nPreviousProgress = 0;

    CTimeCount timeCount;
    timeCount.Start();

    if (pContext->pWorkerContext->pConfig->m_Options.bForceConsoleWindow == true)
    {
        // in this mode all encoders/decoders are forced 
        // to run in native system console window
        // all pipes and progress options are omitted
        int nRet;

        _flushall();
        nRet = ::_tsystem(NULL);
        if ((nRet == 0) && (errno == ENOENT))
        {
            // the command interpreter is not found
            nProgress = -1;
        }
        else
        {
            // remove path to tool and leave only raw tool name
            // note that this is complicated method and in future
            // we need to simplify creation of command-line for system
            // or use CreateProcess here instead of using _tsystem
            CString szSysCommandLine = pContext->szCommandLine;
            CString szToolName;
            CString szToolPath;
            CString szReplace;

            int nStart = szSysCommandLine.Find('"', 0);
            int nEnd = szSysCommandLine.Find('"', nStart + 1);

            szToolPath = szSysCommandLine;
            szToolPath.Truncate(nEnd);
            szReplace = szToolPath + _T("\"");
            szToolPath.TrimLeft(_T("\""));
            szToolName = ::GetFileName(szToolPath);
            szToolPath = szToolPath.TrimRight(szToolName);
            szToolName.Replace(_T("\""), _T(""));
            szSysCommandLine.Replace(szReplace, szToolName);

            // set tool name to current directory because using only tool .exe name
            ::SetCurrentDirectory(szToolPath);

            // use system(...) stdlib.h/process.h function
            _flushall();
            nRet = ::_tsystem(szSysCommandLine); // szCommandLine
            if (nRet == -1)
                nProgress = -1;
            else
                nProgress = 100;
        }
    }
    else
    {
        // NOTE: lpCurrentDirectory param may be useful when app is loading some dll's
        BOOL bRet = FALSE;
        bRet = ::CreateProcess(0,
            pContext->szCommandLine,
            0,
            0,
            TRUE,
            NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW /* | CREATE_SUSPENDED */,
            0,
            0,
            &sInfo,
            &pInfo);
        if (bRet == FALSE)
        {
            timeCount.Stop();

            ::CloseHandle(rOutErrPipe);
            ::CloseHandle(wOutErrPipe);

            if (pContext->bUseReadPipes == true)
            {
                ::CloseHandle(rInPipe);
                ::CloseHandle(wInPipe);
            }

            if (pContext->bUseWritePipes == true)
            {
                ::CloseHandle(rOutPipe);
                ::CloseHandle(wOutPipe);
            }

            pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not create command-line process."));
            pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
            return false;
        }

        // close unused pipes handles
        if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
        {
            ::CloseHandle(wOutErrPipe);
        }
        else
        {
            if (pContext->bUseReadPipes == true)
                ::CloseHandle(rInPipe);

            if (pContext->bUseWritePipes == true)
                ::CloseHandle(wOutPipe);
        }

        bool bWriteSuccess = false;
        CPipeContext rd;
        CPipeContext wd;
        DWORD dwReadThreadID = 0L;
        DWORD dwWriteThreadID = 0L;
        HANDLE hReadThread = NULL;
        HANDLE hWriteThread = NULL;

        if (pContext->bUseReadPipes == true)
        {
            // NOTE: get pipe buffer size
            // DWORD dwOutBufferSize, dwInBufferSize, dwMaxInstances;
            // ::GetNamedPipeInfo(wInPipe, NULL, &dwOutBufferSize, &dwInBufferSize, &dwMaxInstances);

            rd.bError = false;
            rd.bFinished = false;
            rd.pWorkerContext = pContext->pWorkerContext;
            rd.szFileName = pContext->szInputFile;
            rd.hPipe = wInPipe;
            rd.nIndex = pContext->nIndex;

            dwReadThreadID = 0L;
            hReadThread = ::CreateThread(NULL, 0, ReadThread, (LPVOID)&rd, /* CREATE_SUSPENDED */ 0, &dwReadThreadID);
            if (hReadThread == NULL)
            {
                timeCount.Stop();

                ::CloseHandle(wInPipe);

                pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not create read thread."));
                pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
                return false;
            }

            // ::ResumeThread(pInfo.hThread);
            // ::ResumeThread(hReadThread);

            if (pContext->bUseWritePipes == false)
            {
                ::WaitForSingleObject(hReadThread, INFINITE);

                ::CloseHandle(wInPipe);

                // check for result from read thread
                if ((rd.bError == false) && (rd.bFinished == true))
                    nProgress = 100;
                else
                    nProgress = -1;

                // close read thread handle
                ::CloseHandle(hReadThread);
            }
        }

        // NOTE:
        // write pipes may cause serious problems
        // because seeking on written file is impossible
        // this means that file headers can not be written

        // NOTE: write pipes are designed for trans-coding only

        if (pContext->bUseWritePipes == true)
        {
            // NOTE: get pipe buffer size
            // DWORD dwOutBufferSize, dwInBufferSize, dwMaxInstances;
            // ::GetNamedPipeInfo(rInPipe, NULL, &dwOutBufferSize, &dwInBufferSize, &dwMaxInstances);

            wd.bError = false;
            wd.bFinished = false;
            wd.pWorkerContext = pContext->pWorkerContext;
            wd.szFileName = pContext->szOutputFile;
            wd.hPipe = rOutPipe;
            wd.nIndex = pContext->nIndex;

            dwWriteThreadID = 0L;
            hWriteThread = ::CreateThread(NULL, 0, WriteThread, (LPVOID)&wd, /* CREATE_SUSPENDED */ 0, &dwWriteThreadID);
            if (hWriteThread == NULL)
            {
                timeCount.Stop();

                ::CloseHandle(rOutPipe);

                pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not create write thread."));
                pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
                return false;
            }

            // ::ResumeThread(pInfo.hThread);
            // ::ResumeThread(hWriteThread);

            ::WaitForSingleObject(hWriteThread, INFINITE);

            ::CloseHandle(rOutPipe);

            // check for result from read thread
            if ((wd.bError == false) && (wd.bFinished == true))
            {
                bWriteSuccess = true;
                if (pContext->bUseReadPipes == false)
                    nProgress = 100;
            }
            else
            {
                bWriteSuccess = false;
                if (pContext->bUseReadPipes == false)
                    nProgress = -1;
            }

            // close read thread handle
            ::CloseHandle(hWriteThread);
        }

        if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
        {
            ::WaitForSingleObject(hReadThread, INFINITE);

            ::CloseHandle(wInPipe);

            // check for result from read thread
            if ((rd.bError == false) && (rd.bFinished == true) && (bWriteSuccess == true))
                nProgress = 100;
            else
                nProgress = -1;

            // close read thread handle
            ::CloseHandle(hReadThread);
        }

        if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
        {
            // ::ResumeThread(pInfo.hThread);

            // NOTE: 
            // do we need to check console output code-page?
            // all tools should be using system code-page or they are using UNICODE output?
            char szReadBuff[4096];
            char szLineBuff[4096];
            DWORD dwReadBytes = 0L;
            BOOL bRes = FALSE;
            bool bLineStart;
            bool bLineEnd;
            bool bRunning = true;
            int nLineLen = 0;

            // initialize buffers
            ZeroMemory(szReadBuff, sizeof(szReadBuff));
            ZeroMemory(szLineBuff, sizeof(szLineBuff));

            // NOTE: what we get is first start of line or end of line?
            bLineStart = false;
            bLineEnd = false;

            // create log file
            CFile fp;
            bool bHaveLogFile = false;

            if (pContext->pWorkerContext->pConfig->m_Options.bLogConsoleOutput == true && pContext->pWorkerContext->nThreadCount == 1)
            {
                ::UpdatePath();

                if (fp.Open(pContext->pWorkerContext->pConfig->m_Options.szLogFileName, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) == TRUE)
                {
                    bHaveLogFile = true;

                    if (fp.GetLength() == (ULONGLONG)0)
                    {
                        const unsigned char szHeaderUnicode[2] = { 0xFFU, 0xFEU };
                        const unsigned char szHeaderUtf8[3] = { 0xEFU, 0xBBU, 0xBFU };

                        switch (pContext->pWorkerContext->pConfig->m_Options.nLogEncoding)
                        {
                        case 1: // UNICODE
                            fp.Write(szHeaderUnicode, 2);
                            fp.Flush();
                            break;
                        case 2: // UTF-8
                            fp.Write(szHeaderUtf8, 3);
                            fp.Flush();
                            break;
                        };
                    }

                    fp.SeekToEnd();
                }
            }

            // NOTE: preload all progress dll's before conversion process

            // load progress function
            typedef int(*lpfnGetProgress)(char *szLineBuff, int nLineLen);

            lpfnGetProgress pProgressProc;
            HMODULE hDll = ::LoadLibrary(pContext->pWorkerContext->pConfig->m_Formats.GetData(pContext->nTool).szFunction);
            if (hDll != NULL)
            {
                // TODO: when failed to load dll we need to do more clean-up
                // NOTE: the GetProcAddress function has only ANSI version
                pProgressProc = (lpfnGetProgress) ::GetProcAddress(hDll, "GetProgress");
                if (pProgressProc == NULL)
                {
                    pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not get GetProgress function address."));
                    pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
                    return false; // ERROR
                }
            }
            else
            {
                pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: can not load function library dll."));
                pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
                return false; // ERROR
            }

            // main loop
            do
            {
                ZeroMemory(szReadBuff, sizeof(szReadBuff));

                // check if there is something to read, only for debug
                // DWORD dwAvailable;
                // ::PeekNamedPipe(rOutErrPipe, NULL, 0, NULL, &dwAvailable, NULL);

                bRes = ::ReadFile(rOutErrPipe, szReadBuff, 100, &dwReadBytes, 0);
                if (bRes == FALSE || dwReadBytes == 0)
                    break;

                // terminate read data by '\0'
                szReadBuff[dwReadBytes] = '\0';

                for (int i = 0; i < (int)dwReadBytes; i++)
                {
                    // NOTE: processed escape chars ( \r \n \t \b )
                    if (szReadBuff[i] == '\r') // '\r'
                    {
                        if ((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if (szReadBuff[i] == '\n') // '\n'
                    {
                        // do nothing
                    }
                    else if (szReadBuff[i] == '\t') // '\t'
                    {
                        // do nothing
                    }
                    else if (szReadBuff[i] == '\b') // '\b'
                    {
                        // do nothing (most of the tools)
                        // NOTE: same code as if(szReadBuff[i] == '\r')
                        if ((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if (bLineEnd == false)
                    {
                        bLineStart = true; // we have start
                        nLineLen++;
                        szLineBuff[nLineLen - 1] = szReadBuff[i];
                    }

                    // now we have correct full line of text
                    if ((bLineEnd == true) && (bLineStart == false))
                    {
                        // don't include empty lines
                        if (strlen(szLineBuff) > 0)
                        {
                            if (pContext->bDecode == false)
                            {
                                int nRet = (pProgressProc)(szLineBuff, nLineLen);
                                if (nRet != -1)
                                    nProgress = nRet;
                            }
                            else
                            {
                                int nRet = (pProgressProc)(szLineBuff, nLineLen);
                                if (nRet != -1)
                                    nProgress = nRet;
                            }

                            // log all console output for error checking
                            if (pContext->pWorkerContext->pConfig->m_Options.bLogConsoleOutput == true && pContext->pWorkerContext->nThreadCount == 1)
                            {
                                if ((bHaveLogFile == true)
                                    && (fp.m_hFile != NULL)
                                    && (strlen(szLineBuff) > (size_t)0))
                                {
                                    switch (pContext->pWorkerContext->pConfig->m_Options.nLogEncoding)
                                    {
                                    case 0: // ANSI, default string is ANSI
                                    {
                                        fp.Write(szLineBuff, (UINT)strlen(szLineBuff));
                                        fp.Write("\r\n", 2);
                                        fp.Flush();
                                    }
                                    break;
                                    case 1: // UNICODE
                                    {
                                        wchar_t *pszBuffUnicode;
                                        UnicodeEncode(szLineBuff, &pszBuffUnicode);
                                        fp.Write(pszBuffUnicode, (UINT)wcslen(pszBuffUnicode) * 2);
                                        fp.Write(L"\r\n", 4);
                                        fp.Flush();
                                        free(pszBuffUnicode);
                                    }
                                    break;
                                    case 2: // UTF-8
                                    {
                                        char *pszBuffUtf8;
                                        Utf8Encode(szLineBuff, &pszBuffUtf8);
                                        fp.Write(pszBuffUtf8, (UINT)strlen(pszBuffUtf8));
                                        fp.Write("\r\n", 2);
                                        fp.Flush();
                                        free(pszBuffUtf8);
                                    }
                                    break;
                                    default:
                                        break;
                                    };
                                }
                            }

                            ZeroMemory(szLineBuff, sizeof(szLineBuff));

                            if (nProgress != nPreviousProgress)
                            {
                                bRunning = pContext->pWorkerContext->Callback(pContext->nIndex, nProgress, false);
                                nPreviousProgress = nProgress;
                            }

                            if (bRunning == false)
                                break;
                        }

                        // reset line counter
                        nLineLen = 0;

                        // now find next start
                        bLineStart = true;
                        bLineEnd = false;
                    }
                }

                if (bRunning == false)
                    break;
            } while (bRes);

            // free memory by unloading unused dll
            if (hDll != NULL)
                ::FreeLibrary(hDll);

            // close log file
            if (pContext->pWorkerContext->pConfig->m_Options.bLogConsoleOutput == true && bHaveLogFile == true && pContext->pWorkerContext->nThreadCount == 1)
            {
                fp.Close();
                bHaveLogFile = false;
            }

            ::CloseHandle(rOutErrPipe);
        }
    }

    timeCount.Stop();

    if (pContext->pWorkerContext->pConfig->m_Options.bForceConsoleWindow == false)
    {
        // terminate console process if there was error
        HANDLE hProc;
        hProc = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, pInfo.dwProcessId); // PROCESS_TERMINATE
        if (hProc != NULL)
        {
            if (nProgress == 100)
            {
                // when properly finishing we need to wait
                // for process to terminate = 5 seconds max
                DWORD dwRet = ::WaitForSingleObject(hProc, 2000);
                switch (dwRet)
                {
                case WAIT_FAILED:
                    // failed, probably process has finished, but error could be to
                    ::TerminateProcess(hProc, 0);
                    break;
                case WAIT_ABANDONED:
                    // skip, only for mutex objects
                    break;
                case WAIT_OBJECT_0:
                    // skip, object is signaled, process has terminated
                    break;
                case WAIT_TIMEOUT:
                    // time-out interval elapsed
                    // object's state is non-signaled
                    // used when user had pressed stop button
                    ::TerminateProcess(hProc, 0);
                    break;
                };
            }
            else
            {
                // don't wait for process to terminate because
                // this code is only executed when user pressed Stop
                ::TerminateProcess(hProc, 0);

                // wait for process to terminate = 5 seconds max
                // this is because process must release file handles
                // and we need to delete it if there was an error
                ::WaitForSingleObject(hProc, 5000);
            }

            // release process handle
            ::CloseHandle(hProc);
        }

        ::CloseHandle(pInfo.hProcess);
        ::CloseHandle(pInfo.hThread);
    }

    if (nProgress != 100)
    {
        pContext->pWorkerContext->Status(pContext->nIndex, _T("--:--"), _T("Error: progress did not reach 100%."));
        pContext->pWorkerContext->Callback(pContext->nIndex, -1, true, true);
        return false;
    }
    else
    {
        pContext->pWorkerContext->Status(pContext->nIndex, ::FormatTime(timeCount.ElapsedTime(), 1), _T("Done"));
        pContext->pWorkerContext->Callback(pContext->nIndex, 100, true, false);
        return true;
    }
}
