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
    // NOTE:
    // if bUseReadPipes = true use pipes to read source file and send the data to console stdin
    // if bUseReadPipes = false create full command-line and read from stdout/stderr conversion progress

    // set the correct security attributes
    SECURITY_ATTRIBUTES secattr;
    ZeroMemory(&secattr, sizeof(secattr));
    secattr.nLength = sizeof(secattr);
    secattr.bInheritHandle = TRUE;

    HANDLE hReadPipeStderr = INVALID_HANDLE_VALUE;
    HANDLE hWritePipeStderr = INVALID_HANDLE_VALUE;
    HANDLE hReadPipeStdin = INVALID_HANDLE_VALUE;
    HANDLE hWritePipeStdin = INVALID_HANDLE_VALUE;
    HANDLE hReadPipeStdout = INVALID_HANDLE_VALUE;
    HANDLE hWritePipeStdout = INVALID_HANDLE_VALUE;

    // create pipes for stderr
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        BOOL bRet = ::CreatePipe(&hReadPipeStderr, &hWritePipeStderr, &secattr, 0);
        if (bRet == FALSE)
        {
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create pipes for stderr."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // duplicate stderr read pipe handle to prevent child process from closing the pipe
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        BOOL bRet = ::DuplicateHandle(::GetCurrentProcess(),
            hReadPipeStderr,
            ::GetCurrentProcess(),
            &hReadPipeStderr,
            0,
            TRUE,
            DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS);
        if (bRet == FALSE)
        {
            ::CloseHandle(hReadPipeStderr);
            ::CloseHandle(hWritePipeStderr);

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not duplicate stderr pipe to prevent child process from closing the pipe."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // create pipes for stdin
    if (pContext->bUseReadPipes == true)
    {
        BOOL bRet = ::CreatePipe(&hReadPipeStdin, &hWritePipeStdin, &secattr, 0);
        if (bRet == FALSE)
        {
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create pipes for stdin."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // create pipes for stdout
    if (pContext->bUseWritePipes == true)
    {
        ;
        BOOL bRet = ::CreatePipe(&hReadPipeStdout, &hWritePipeStdout, &secattr, 0);
        if (bRet == FALSE)
        {
            if (pContext->bUseReadPipes == true)
            {
                ::CloseHandle(hReadPipeStdin);
                ::CloseHandle(hWritePipeStdin);
            }

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create pipes for stdout."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // set stdin write pipe inherit flag
    if (pContext->bUseReadPipes == true)
    {
        BOOL bRet = ::SetHandleInformation(hWritePipeStdin, HANDLE_FLAG_INHERIT, 0);
        if (bRet == FALSE)
        {
            ::CloseHandle(hReadPipeStdin);
            ::CloseHandle(hWritePipeStdin);

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not set stdin pipe inherit flag."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // set stdout read pipe inherit flag
    if (pContext->bUseWritePipes == true)
    {
        BOOL bRet = ::SetHandleInformation(hReadPipeStdout, HANDLE_FLAG_INHERIT, 0);
        if (bRet == FALSE)
        {
            if (pContext->bUseReadPipes == true)
            {
                ::CloseHandle(hReadPipeStdin);
                ::CloseHandle(hWritePipeStdin);
            }

            ::CloseHandle(hReadPipeStdout);
            ::CloseHandle(hWritePipeStdout);

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not set stdout pipe inherit flag."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
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
    // try to get progress from console using stderr pipe

    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        sInfo.hStdInput = NULL;
        sInfo.hStdOutput = hWritePipeStderr;
        sInfo.hStdError = hWritePipeStderr;
    }
    else if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == false))
    {
        sInfo.hStdInput = hReadPipeStdin;
        sInfo.hStdOutput = NULL;
        sInfo.hStdError = NULL;
    }
    else if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == true))
    {
        sInfo.hStdInput = NULL;
        sInfo.hStdOutput = hWritePipeStdout;
        sInfo.hStdError = NULL;
    }
    else if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
    {
        sInfo.hStdInput = hReadPipeStdin;
        sInfo.hStdOutput = hWritePipeStdout;
        sInfo.hStdError = NULL;
    }

    int nProgress = 0;
    int nPreviousProgress = 0;
    CTimeCount timeCount;

    timeCount.Start();

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

        if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
        {
            ::CloseHandle(hReadPipeStderr);
            ::CloseHandle(hWritePipeStderr);
        }

        if (pContext->bUseReadPipes == true)
        {
            ::CloseHandle(hReadPipeStdin);
            ::CloseHandle(hWritePipeStdin);
        }

        if (pContext->bUseWritePipes == true)
        {
            ::CloseHandle(hReadPipeStdout);
            ::CloseHandle(hWritePipeStdout);
        }

        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create command-line process."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipes handles
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        ::CloseHandle(hWritePipeStderr);
    }

    if (pContext->bUseReadPipes == true)
        ::CloseHandle(hReadPipeStdin);

    if (pContext->bUseWritePipes == true)
        ::CloseHandle(hWritePipeStdout);

    bool bWriteSuccess = false;
    CPipeContext rd;
    CPipeContext wd;
    DWORD dwReadThreadID = 0L;
    DWORD dwWriteThreadID = 0L;
    HANDLE hReadThread = NULL;
    HANDLE hWriteThread = NULL;

    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        // ::ResumeThread(pInfo.hThread);

        // NOTE: 
        // do we need to check console output code-page?
        // all tools should be using system code-page or they are using UNICODE output?
        const int nBuffSize = 4096;
        char szReadBuff[nBuffSize];
        char szLineBuff[nBuffSize];
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

        // load progress function
        typedef int(*lpfnGetProgress)(char *szLineBuff, int nLineLen);

        lpfnGetProgress pProgressProc;
        HMODULE hDll = ::LoadLibrary(pContext->szFunction);
        if (hDll != NULL)
        {
            // TODO: when failed to load dll we need to do more clean-up
            // NOTE: the GetProcAddress function has only ANSI version
            pProgressProc = (lpfnGetProgress) ::GetProcAddress(hDll, "GetProgress");
            if (pProgressProc == NULL)
            {
                pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not get GetProgress function address."));
                pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
                return false; // ERROR
            }
        }
        else
        {
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not load function library dll."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false; // ERROR
        }

        // main loop
        do
        {
            ZeroMemory(szReadBuff, sizeof(szReadBuff));

            // check if there is something to read, only for debug
            // DWORD dwAvailable;
            // ::PeekNamedPipe(rOutErrPipe, NULL, 0, NULL, &dwAvailable, NULL);

            bRes = ::ReadFile(hReadPipeStderr, szReadBuff, 100, &dwReadBytes, 0);
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
                    if (nLineLen > nBuffSize)
                    {
                        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: console line is too large for read buffer."));
                        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
                        return false;
                    }

                    szLineBuff[nLineLen - 1] = szReadBuff[i];
                }

                // now we have correct full line of text
                if ((bLineEnd == true) && (bLineStart == false))
                {
                    // don't include empty lines
                    if (strlen(szLineBuff) > 0)
                    {
                        int nRet = (pProgressProc)(szLineBuff, nLineLen);
                        if (nRet != -1)
                            nProgress = nRet;

                        ZeroMemory(szLineBuff, sizeof(szLineBuff));

                        if (nProgress != nPreviousProgress)
                        {
                            bRunning = pContext->pWorkerContext->Callback(pContext->nItemId, nProgress, false);
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

        ::CloseHandle(hReadPipeStderr);
    }

    if (pContext->bUseReadPipes == true)
    {
        // NOTE: get pipe buffer size
        // DWORD dwOutBufferSize, dwInBufferSize, dwMaxInstances;
        // ::GetNamedPipeInfo(wInPipe, NULL, &dwOutBufferSize, &dwInBufferSize, &dwMaxInstances);

        rd.bError = false;
        rd.bFinished = false;
        rd.pWorkerContext = pContext->pWorkerContext;
        rd.szFileName = pContext->szInputFile;
        rd.hPipe = hWritePipeStdin;
        rd.nIndex = pContext->nItemId;

        dwReadThreadID = 0L;
        hReadThread = ::CreateThread(NULL, 0, ReadThread, (LPVOID)&rd, /* CREATE_SUSPENDED */ 0, &dwReadThreadID);
        if (hReadThread == NULL)
        {
            timeCount.Stop();

            ::CloseHandle(hWritePipeStdin);

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create read thread."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // ::ResumeThread(pInfo.hThread);
        // ::ResumeThread(hReadThread);

        if (pContext->bUseWritePipes == false)
        {
            ::WaitForSingleObject(hReadThread, INFINITE);

            // NOTE: Handle is closed in ReadThread.
            //::CloseHandle(wInPipe);

            // check for result from read thread
            if ((rd.bError == false) && (rd.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;

            // close read thread handle
            ::CloseHandle(hReadThread);
        }
    }

    if (pContext->bUseWritePipes == true)
    {
        // NOTE:
        // write pipes may cause problems because seeking on written file is not possible that means that file headers can not be written

        // NOTE: get pipe buffer size
        // DWORD dwOutBufferSize, dwInBufferSize, dwMaxInstances;
        // ::GetNamedPipeInfo(rInPipe, NULL, &dwOutBufferSize, &dwInBufferSize, &dwMaxInstances);

        wd.bError = false;
        wd.bFinished = false;
        wd.pWorkerContext = pContext->pWorkerContext;
        wd.szFileName = pContext->szOutputFile;
        wd.hPipe = hReadPipeStdout;
        wd.nIndex = pContext->nItemId;

        dwWriteThreadID = 0L;
        hWriteThread = ::CreateThread(NULL, 0, WriteThread, (LPVOID)&wd, /* CREATE_SUSPENDED */ 0, &dwWriteThreadID);
        if (hWriteThread == NULL)
        {
            timeCount.Stop();

            ::CloseHandle(hReadPipeStdout);

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create write thread."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // ::ResumeThread(pInfo.hThread);
        // ::ResumeThread(hWriteThread);

        ::WaitForSingleObject(hWriteThread, INFINITE);

        ::CloseHandle(hReadPipeStdout);

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

        // NOTE: Handle is closed in ReadThread.
        //::CloseHandle(wInPipe);

        // check for result from read thread
        if ((rd.bError == false) && (rd.bFinished == true) && (bWriteSuccess == true))
            nProgress = 100;
        else
            nProgress = -1;

        // close read thread handle
        ::CloseHandle(hReadThread);
    }

    timeCount.Stop();

    // terminate console process if there was an error
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

    if (nProgress != 100)
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: progress did not reach 100%."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pContext->pWorkerContext->Status(pContext->nItemId, ::FormatTime(timeCount.ElapsedTime(), 1), _T("Done"));
        pContext->pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}
