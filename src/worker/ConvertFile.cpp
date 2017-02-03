// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

void ProgresssLoop(CFileContext* pContext, CProcessContext &processContext, int nProgress)
{
    GetProgress *pGetProgress;
    const int nBuffSize = 4096;
    char szReadBuff[nBuffSize];
    char szLineBuff[nBuffSize];
    DWORD dwReadBytes = 0L;
    BOOL bRes = FALSE;
    bool bLineStart = false;
    bool bLineEnd = false;
    bool bRunning = true;
    int nLineLen = 0;
    int nPreviousProgress = 0;
    
    // initialize buffers
    ZeroMemory(szReadBuff, sizeof(szReadBuff));
    ZeroMemory(szLineBuff, sizeof(szLineBuff));
    
    // load progress function
    HMODULE hDll = ::LoadLibrary(pContext->szFunction);
    if (hDll != NULL)
    {
        // the GetProcAddress function has only ANSI version
        pGetProgress = (GetProgress*) ::GetProcAddress(hDll, "GetProgress");
        if (pGetProgress == NULL)
        {
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not get GetProgress function address."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false; // ERROR
        }
    }
    else
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not load GetProgress function library dll."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false; // ERROR
    }

    do
    {
        ZeroMemory(szReadBuff, sizeof(szReadBuff));
        bRes = ::ReadFile(processContext.hReadPipeStderr, szReadBuff, 100, &dwReadBytes, 0);
        if (bRes == FALSE || dwReadBytes == 0)
            break;

        // terminate read data by '\0'
        szReadBuff[dwReadBytes] = '\0';

        for (int i = 0; i < (int)dwReadBytes; i++)
        {
            // processed escape chars ( \r \n \t \b )
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
                    int nRet = (pGetProgress)(szLineBuff, nLineLen);
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

                // find next start
                bLineStart = true;
                bLineEnd = false;
            }
        }

        if (bRunning == false)
            break;
    } while (bRes);

    if (hDll != NULL)
        ::FreeLibrary(hDll);
    
    return true;
}

bool ConvertFileUsingConsole(CFileContext* pContext)
{
    if ((pContext->bUseReadPipes == true) || (pContext->bUseWritePipes == true))
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: invalid format pipe configuration."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    CProcessContext processContext;
    int nProgress = 0;
    CTimeCount timer;

    // create pipes for stderr
    if (processContext.CreateStderrPipe() == false)
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create pipes for stderr."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // duplicate stderr read pipe handle to prevent child process from closing the pipe
    if (processContext.DuplicateStderrReadPipe() == false)
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not duplicate stderr pipe to prevent child process from closing the pipe."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // connect pipes to process
    processContext.ConnectStdInput(NULL);
    processContext.ConnectStdOutput(processContext.hWritePipeStderr);
    processContext.ConnectStdError(processContext.hWritePipeStderr);

    timer.Start();
    if (processContext.Start(pContext->szCommandLine) == false)
    {
        timer.Stop();

        processContext.CloseStderrReadPipe();
        processContext.CloseStderrWritePipe();

        CString szStatus;
        szStatus.Format(_T("Error: can not create command-line process (%d)."), ::GetLastError());

        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), szStatus);
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handle
    processContext.CloseStderrWritePipe();

    // console progresss loop
    if (ProgresssLoop(pContext, &processContext, &nProgress) == false)
    {
        timer.Stop();
        processContext.CloseStderrReadPipe();
        processContext.Stop(false);
        return false;
    }

    timer.Stop();
    processContext.CloseStderrReadPipe();
    processContext.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: progress did not reach 100%."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pContext->pWorkerContext->Status(pContext->nItemId, timer.Format(timer.ElapsedTime(), 1), _T("Done"));
        pContext->pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}

bool ConvertFileUsingPipes(CFileContext* pContext)
{
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: invalid format pipe configuration."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    CProcessContext processContext;
    CPipeContext readContext;
    CPipeContext writeContext;
    DWORD dwReadThreadID = 0L;
    DWORD dwWriteThreadID = 0L;
    HANDLE hReadThread = NULL;
    HANDLE hWriteThread = NULL;
    bool bWriteThreadResult = false;
    int nProgress = 0;
    CTimeCount timer;

    if (pContext->bUseReadPipes == true)
    {
        // create pipes for stdin
        if (processContext.CreateStdinPipe() == false)
        {
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create pipes for stdin."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // set stdin write pipe inherit flag
        if (processContext.InheritStdinWritePipe() == false)
        {
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not set stdin pipe inherit flag."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    if (pContext->bUseWritePipes == true)
    {
        // create pipes for stdout
        if (processContext.CreateStdoutPipe() == false)
        {
            if (pContext->bUseReadPipes == true)
            {
                processContext.CloseStdinReadPipe();
                processContext.CloseStdinWritePipe();
            }
            
            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create pipes for stdout."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // set stdout read pipe inherit flag
        if (processContext.InheritStdoutReadPipe() == false)
        {
            if (pContext->bUseReadPipes == true)
            {
                processContext.CloseStdinReadPipe();
                processContext.CloseStdinWritePipe();
            }

            processContext.CloseStdoutReadPipe();
            processContext.CloseStdoutWritePipe();

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not set stdout pipe inherit flag."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // connect pipes to process
    if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == false))
    {
        processContext.ConnectStdInput(processContext.hReadPipeStdin);
        processContext.ConnectStdOutput(NULL);
        processContext.ConnectStdError(NULL);
    }
    else if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == true))
    {
        processContext.ConnectStdInput(NULL);
        processContext.ConnectStdOutput(processContext.hWritePipeStdout);
        processContext.ConnectStdError(NULL); 
    }
    else if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
    {
        processContext.ConnectStdInput(processContext.hReadPipeStdin);
        processContext.ConnectStdOutput(processContext.hWritePipeStdout);
        processContext.ConnectStdError(NULL);
    }

    timer.Start();
    if (processContext.Start(pContext->szCommandLine) == false)
    {
        timer.Stop();

        if (pContext->bUseReadPipes == true)
        {
            processContext.CloseStdinReadPipe();
            processContext.CloseStdinWritePipe();
        }

        if (pContext->bUseWritePipes == true)
        {
            processContext.CloseStdoutReadPipe();
            processContext.CloseStdoutWritePipe();
        }

        CString szStatus;
        szStatus.Format(_T("Error: can not create command-line process (%d)."), ::GetLastError());

        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), szStatus);
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handles
    if (pContext->bUseReadPipes == true)
        processContext.CloseStdinReadPipe();

    if (pContext->bUseWritePipes == true)
        processContext.CloseStdoutWritePipe();

    // create read thread
    if (pContext->bUseReadPipes == true)
    {
        readContext.bError = false;
        readContext.bFinished = false;
        readContext.pWorkerContext = pContext->pWorkerContext;
        readContext.szFileName = pContext->szInputFile;
        readContext.hPipe = processContext.hWritePipeStdin;
        readContext.nIndex = pContext->nItemId;

        dwReadThreadID = 0L;
        hReadThread = ::CreateThread(NULL, 0, ReadThread, (LPVOID)&readContext, 0, &dwReadThreadID);
        if (hReadThread == NULL)
        {
            timer.Stop();

            processContext.CloseStdinWritePipe();

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create read thread."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // wait for read thread to finish
        if (pContext->bUseWritePipes == false)
        {
            ::WaitForSingleObject(hReadThread, INFINITE);

            // NOTE: Handle is closed in ReadThread.
            //processContext.CloseStdinWritePipe();

            // check for result from read thread
            if ((readContext.bError == false) && (readContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;

            // close read thread handle
            ::CloseHandle(hReadThread);
        }
    }

    // create write thread
    if (pContext->bUseWritePipes == true)
    {
        writeContext.bError = false;
        writeContext.bFinished = false;
        writeContext.pWorkerContext = pContext->pWorkerContext;
        writeContext.szFileName = pContext->szOutputFile;
        writeContext.hPipe = processContext.hReadPipeStdout;
        writeContext.nIndex = pContext->nItemId;

        dwWriteThreadID = 0L;
        hWriteThread = ::CreateThread(NULL, 0, WriteThread, (LPVOID)&writeContext, 0, &dwWriteThreadID);
        if (hWriteThread == NULL)
        {
            timer.Stop();

            processContext.CloseStdoutReadPipe();

            pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: can not create write thread."));
            pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // wait for write thread to finish
        ::WaitForSingleObject(hWriteThread, INFINITE);

        processContext.CloseStdoutReadPipe();

        // check for result from read thread
        if ((writeContext.bError == false) && (writeContext.bFinished == true))
        {
            bWriteThreadResult = true;
            if (pContext->bUseReadPipes == false)
                nProgress = 100;
        }
        else
        {
            bWriteThreadResult = false;
            if (pContext->bUseReadPipes == false)
                nProgress = -1;
        }

        // close read thread handle
        ::CloseHandle(hWriteThread);
    }

    // wait for read thread to finish after write thread finished
    if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
    {
        ::WaitForSingleObject(hReadThread, INFINITE);

        // NOTE: Handle is closed in ReadThread.
        //processContext.CloseStdinWritePipe();

        // check for result from read thread
        if ((readContext.bError == false) && (readContext.bFinished == true) && (bWriteThreadResult == true))
            nProgress = 100;
        else
            nProgress = -1;

        // close read thread handle
        ::CloseHandle(hReadThread);
    }

    timer.Stop();
    processContext.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pContext->pWorkerContext->Status(pContext->nItemId, _T("--:--"), _T("Error: progress did not reach 100%."));
        pContext->pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pContext->pWorkerContext->Status(pContext->nItemId, timer.Format(timer.ElapsedTime(), 1), _T("Done"));
        pContext->pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}

bool ConvertFile(CFileContext* pContext)
{
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
        return ConvertFileUsingConsole(pContext);
    else
        return ConvertFileUsingPipes(pContext);
}
