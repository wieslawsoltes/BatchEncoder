// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\MainApp.h"
#include "..\Strings.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\Utilities.h"
#include "LuaProgess.h"
#include "WorkThread.h"

bool ProgresssLoop(CFileContext* pContext, CProcess &processContext, CPipe &Stderr, int &nProgress)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;
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

    ::SetCurrentDirectory(::GetExeFilePath());

    // load progress function
    CLuaProgess luaProgress;
    if (luaProgress.Open(CT2CA(pContext->pFormat->szFunction)) == false)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110001, pszProgresssLoop[0]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false; // ERROR
    }

    if (luaProgress.HaveGetProgress() == false)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110002, pszProgresssLoop[1]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false; // ERROR
    }

    // initialize buffers
    ZeroMemory(szReadBuff, sizeof(szReadBuff));
    ZeroMemory(szLineBuff, sizeof(szLineBuff));

    do
    {
        ZeroMemory(szReadBuff, sizeof(szReadBuff));
        bRes = ::ReadFile(Stderr.hRead, szReadBuff, 100, &dwReadBytes, 0);
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
                    pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110003, pszProgresssLoop[2]));
                    pWorkerContext->Callback(pContext->nItemId, -1, true, true);
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
                    //OutputDebugStringA(szLineBuff); OutputDebugStringA("\n");
                    int nRet = (int)luaProgress.GetProgress(szLineBuff, nLineLen);
                    if (nRet != -1)
                        nProgress = nRet;

                    //CString szRet; szRet.Format(_T("%d#n"), nRet); OutputDebugString(szRet);

                    ZeroMemory(szLineBuff, sizeof(szLineBuff));

                    if (nProgress != nPreviousProgress)
                    {
                        bRunning = pWorkerContext->Callback(pContext->nItemId, nProgress, false);
                        nPreviousProgress = nProgress;
                    }

                    if ((pContext->pWorkerContext->bRunning == false) || (bRunning == false))
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

    luaProgress.Close();

    return true;
}

bool ConvertFileUsingConsole(CFileContext* pContext)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;

    if ((pContext->bUseReadPipes == true) || (pContext->bUseWritePipes == true))
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120001, pszConvertConsole[0]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    CProcess processContext;
    CPipe Stderr(TRUE);
    int nProgress = 0;
    CTimeCount timer;

    // create pipes for stderr
    if (Stderr.Create() == false)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120002, pszConvertConsole[1]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // duplicate stderr read pipe handle to prevent child process from closing the pipe
    if (Stderr.DuplicateRead() == false)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120003, pszConvertConsole[2]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // connect pipes to process
    processContext.ConnectStdInput(NULL);
    processContext.ConnectStdOutput(Stderr.hWrite);
    processContext.ConnectStdError(Stderr.hWrite);

    timer.Start();
    if (processContext.Start(pContext->pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        Stderr.CloseRead();
        Stderr.CloseWrite();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00120004, pszConvertConsole[3]), ::GetLastError());

        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handle
    Stderr.CloseWrite();

    // console progress loop
    if (ProgresssLoop(pContext, processContext, Stderr, nProgress) == false)
    {
        timer.Stop();
        Stderr.CloseRead();
        processContext.Stop(false);
        return false;
    }

    timer.Stop();
    Stderr.CloseRead();
    processContext.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120005, pszConvertConsole[4]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(pContext->nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x00120006, pszConvertConsole[5]));
        pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}

bool ReadLoop(CPipeContext* pContext)
{
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

    pContext->bError = false;
    pContext->bFinished = false;

    hFile = ::CreateFile(pContext->szFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        ::CloseHandle(pContext->hPipe);
        return false;
    }

    nFileSize = ::GetFileSize64(hFile);
    if (nFileSize == 0)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        ::CloseHandle(hFile);
        ::CloseHandle(pContext->hPipe);
        return false;
    }

    do
    {
        bRes = ::ReadFile(hFile, pReadBuff, 4096, &dwReadBytes, 0);
        if ((bRes == FALSE) || (dwReadBytes == 0))
            break;

        ::Sleep(0);

        bRes = ::WriteFile(pContext->hPipe, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        nTotalBytesRead += dwReadBytes;
        nProgress = (int)((nTotalBytesRead * 100) / nFileSize);

        if (nProgress != nPreviousProgress)
        {
            bRunning = pContext->pWorkerContext->Callback(pContext->nIndex, nProgress, false);
            nPreviousProgress = nProgress;
        }

        if ((pContext->pWorkerContext->bRunning == false) || (bRunning == false))
            break;
    } while (bRes != FALSE);

    ::CloseHandle(hFile);
    ::CloseHandle(pContext->hPipe);

    if (nTotalBytesRead != nFileSize)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return false;
    }
    else
    {
        pContext->bError = false;
        pContext->bFinished = true;
        return true;
    }
}

bool WriteLoop(CPipeContext* pContext)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE pReadBuff[4096];
    BOOL bRes = FALSE;
    DWORD dwReadBytes = 0;
    DWORD dwWriteBytes = 0;
    ULONGLONG nTotalBytesWrite = 0;
    ULONGLONG nFileSize = 0;
    int nProgress = -1;

    pContext->bError = false;
    pContext->bFinished = false;

    hFile = ::CreateFile(pContext->szFileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        0,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return false;
    }

    do
    {
        ::Sleep(0);

        DWORD dwAvailableBytes;
        if (FALSE == PeekNamedPipe(pContext->hPipe, 0, 0, 0, &dwAvailableBytes, 0))
            break;

        if (dwAvailableBytes > 0)
        {
            bRes = ::ReadFile(pContext->hPipe, pReadBuff, 4096, &dwReadBytes, 0);
            if ((bRes == FALSE) || (dwReadBytes == 0))
                break;

            bRes = ::WriteFile(hFile, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
            if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
                break;

            nTotalBytesWrite += dwReadBytes;
        }
        else
            bRes = TRUE;

        if (pContext->pWorkerContext->bRunning == false)
            break;
    } while (bRes != FALSE);

    ::CloseHandle(hFile);

    if (nTotalBytesWrite <= 0)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return false;
    }
    else
    {
        pContext->bError = false;
        pContext->bFinished = true;
        return true;
    }
}

DWORD WINAPI ReadThread(LPVOID lpParam)
{
    CPipeContext* pContext = (CPipeContext*)lpParam;
    if (pContext != NULL)
    {
        if (ReadLoop(pContext) == true)
            return TRUE;
    }
    return FALSE;
}

DWORD WINAPI WriteThread(LPVOID lpParam)
{
    CPipeContext* pContext = (CPipeContext*)lpParam;
    if (pContext != NULL)
    {
        if (WriteLoop(pContext) == true)
            return TRUE;
    }
    return FALSE;
}

bool ConvertFileUsingPipes(CFileContext* pContext)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;

    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130001, pszConvertPipes[0]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    CProcess processContext;
    CPipe Stdin(TRUE);
    CPipe Stdout(TRUE);
    CPipeContext readContext;
    CPipeContext writeContext;
    DWORD dwReadThreadID = 0L;
    DWORD dwWriteThreadID = 0L;
    HANDLE hReadThread = NULL;
    HANDLE hWriteThread = NULL;
    int nProgress = 0;
    CTimeCount timer;

    if (pContext->bUseReadPipes == true)
    {
        // create pipes for stdin
        if (Stdin.Create() == false)
        {
            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130002, pszConvertPipes[1]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // set stdin write pipe inherit flag
        if (Stdin.InheritWrite() == false)
        {
            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130003, pszConvertPipes[2]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    if (pContext->bUseWritePipes == true)
    {
        // create pipes for stdout
        if (Stdout.Create() == false)
        {
            if (pContext->bUseReadPipes == true)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();
            }

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130004, pszConvertPipes[3]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // set stdout read pipe inherit flag
        if (Stdout.InheritRead() == false)
        {
            if (pContext->bUseReadPipes == true)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();
            }

            Stdout.CloseRead();
            Stdout.CloseWrite();

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130005, pszConvertPipes[4]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // connect pipes to process
    if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == false))
    {
        processContext.ConnectStdInput(Stdin.hRead);
        processContext.ConnectStdOutput(GetStdHandle(STD_OUTPUT_HANDLE));
        processContext.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
    }
    else if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == true))
    {
        processContext.ConnectStdInput(GetStdHandle(STD_INPUT_HANDLE));
        processContext.ConnectStdOutput(Stdout.hWrite);
        processContext.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
    }
    else if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
    {
        processContext.ConnectStdInput(Stdin.hRead);
        processContext.ConnectStdOutput(Stdout.hWrite);
        processContext.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
    }

    timer.Start();
    if (processContext.Start(pContext->pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        if (pContext->bUseReadPipes == true)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();
        }

        if (pContext->bUseWritePipes == true)
        {
            Stdout.CloseRead();
            Stdout.CloseWrite();
        }

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handles
    if (pContext->bUseReadPipes == true)
        Stdin.CloseRead();

    if (pContext->bUseWritePipes == true)
        Stdout.CloseWrite();

    // create read thread
    if (pContext->bUseReadPipes == true)
    {
        readContext.bError = false;
        readContext.bFinished = false;
        readContext.pWorkerContext = pWorkerContext;
        readContext.szFileName = pContext->szInputFile;
        readContext.hPipe = Stdin.hWrite;
        readContext.nIndex = pContext->nItemId;

        dwReadThreadID = 0L;
        hReadThread = ::CreateThread(NULL, 0, ReadThread, (LPVOID)&readContext, 0, &dwReadThreadID);
        if (hReadThread == NULL)
        {
            timer.Stop();

            Stdin.CloseWrite();

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130007, pszConvertPipes[6]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // wait for read thread to finish
        if (pContext->bUseWritePipes == false)
        {
            ::WaitForSingleObject(hReadThread, INFINITE);

            // NOTE: Handle is closed in ReadThread.
            //Stdin.CloseWrite();

            // close read thread handle
            ::CloseHandle(hReadThread);

            // check for result from read thread
            if ((readContext.bError == false) && (readContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;
        }
    }

    // create write thread
    if (pContext->bUseWritePipes == true)
    {
        writeContext.bError = false;
        writeContext.bFinished = false;
        writeContext.pWorkerContext = pWorkerContext;
        writeContext.szFileName = pContext->szOutputFile;
        writeContext.hPipe = Stdout.hRead;
        writeContext.nIndex = pContext->nItemId;

        dwWriteThreadID = 0L;
        hWriteThread = ::CreateThread(NULL, 0, WriteThread, (LPVOID)&writeContext, 0, &dwWriteThreadID);
        if (hWriteThread == NULL)
        {
            timer.Stop();

            Stdout.CloseRead();

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130008, pszConvertPipes[7]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        if (pContext->bUseReadPipes == true)
        {
            // wait for read thread to finish
            ::WaitForSingleObject(hReadThread, INFINITE);

            // NOTE: Handle is closed in ReadThread.
            //Stdin.CloseWrite();

            // close read thread handle
            ::CloseHandle(hReadThread);

            if ((readContext.bError == true) || (readContext.bFinished == false))
            {
                // close read thread handle
                Stdout.CloseRead();

                // read thread failed so terminate write thread
                ::TerminateThread(hWriteThread, FALSE);
                ::CloseHandle(hWriteThread);
            }
            else
            {
                // close read thread handle
                Stdout.CloseRead();

                // wait for write thread to finish
                ::WaitForSingleObject(hWriteThread, INFINITE);

                // close write thread handle
                ::CloseHandle(hWriteThread);
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
            ::WaitForSingleObject(hWriteThread, INFINITE);

            Stdout.CloseRead();

            // close read thread handle
            ::CloseHandle(hWriteThread);

            // check for result from write thread
            if ((writeContext.bError == false) && (writeContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;
        }
    }

    timer.Stop();
    processContext.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130009, pszConvertPipes[8]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(pContext->nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x0013000B, pszConvertPipes[10]));
        pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}

bool ConvertFileUsingOnlyPipes(CFileContext* pDecoderContext, CFileContext* pEncoderContext)
{
    CWorkerContext *pWorkerContext = pDecoderContext->pWorkerContext;
    CProcess processContextDecoder;
    CProcess processContextEncoder;
    CPipe Stdin(TRUE);
    CPipe Stdout(TRUE);
    CPipe Bridge(TRUE);
    CPipeContext readContext;
    CPipeContext writeContext;
    DWORD dwReadThreadID = 0L;
    DWORD dwWriteThreadID = 0L;
    HANDLE hReadThread = NULL;
    HANDLE hWriteThread = NULL;
    int nProgress = 0;
    CTimeCount timer;

    // create pipes for stdin
    if (Stdin.Create() == false)
    {
        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130002, pszConvertPipes[1]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // set stdin write pipe inherit flag
    if (Stdin.InheritWrite() == false)
    {
        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130003, pszConvertPipes[2]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // create pipes for stdout
    if (Stdout.Create() == false)
    {
        Stdin.CloseRead();
        Stdin.CloseWrite();

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130004, pszConvertPipes[3]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // set stdout read pipe inherit flag
    if (Stdout.InheritRead() == false)
    {
        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130005, pszConvertPipes[4]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // create pipes for processes bridge
    if (Bridge.Create() == false)
    {
        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x0013000A, pszConvertPipes[9]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // connect pipes to decoder process
    processContextDecoder.ConnectStdInput(Stdin.hRead);
    processContextDecoder.ConnectStdOutput(Bridge.hWrite);
    processContextDecoder.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));

    // connect pipes to encoder process
    processContextEncoder.ConnectStdInput(Bridge.hRead);
    processContextEncoder.ConnectStdOutput(Stdout.hWrite);
    processContextEncoder.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));

    timer.Start();

    // create decoder process
    if (processContextDecoder.Start(pDecoderContext->pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        Bridge.CloseRead();
        Bridge.CloseWrite();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // create encoder process
    if (processContextEncoder.Start(pEncoderContext->pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        processContextDecoder.Stop(false);

        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        Bridge.CloseRead();
        Bridge.CloseWrite();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handles
    Stdin.CloseRead();
    Stdout.CloseWrite();
    Bridge.CloseWrite();
    Bridge.CloseRead();

    // create read thread
    readContext.bError = false;
    readContext.bFinished = false;
    readContext.pWorkerContext = pWorkerContext;
    readContext.szFileName = pDecoderContext->szInputFile;
    readContext.hPipe = Stdin.hWrite;
    readContext.nIndex = pDecoderContext->nItemId;

    dwReadThreadID = 0L;
    hReadThread = ::CreateThread(NULL, 0, ReadThread, (LPVOID)&readContext, 0, &dwReadThreadID);
    if (hReadThread == NULL)
    {
        timer.Stop();

        Stdin.CloseWrite();

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130007, pszConvertPipes[6]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // create write thread
    writeContext.bError = false;
    writeContext.bFinished = false;
    writeContext.pWorkerContext = pWorkerContext;
    writeContext.szFileName = pEncoderContext->szOutputFile;
    writeContext.hPipe = Stdout.hRead;
    writeContext.nIndex = pEncoderContext->nItemId;

    dwWriteThreadID = 0L;
    hWriteThread = ::CreateThread(NULL, 0, WriteThread, (LPVOID)&writeContext, 0, &dwWriteThreadID);
    if (hWriteThread == NULL)
    {
        timer.Stop();

        Stdout.CloseRead();

        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130008, pszConvertPipes[7]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }

    // wait for read thread to finish after write thread finished
    ::WaitForSingleObject(hReadThread, INFINITE);

    // NOTE: Handle is closed in ReadThread.
    //Stdin.CloseWrite();

    // close read thread handle
    ::CloseHandle(hReadThread);

    if ((readContext.bError == true) || (readContext.bFinished == false))
    {
        Stdout.CloseRead();

        // read thread failed so terminate write thread
        ::TerminateThread(hWriteThread, FALSE);
        ::CloseHandle(hWriteThread);
    }
    else
    {
        Stdout.CloseRead();

        // wait for write thread to finish
        ::WaitForSingleObject(hWriteThread, INFINITE);

        // close write thread handle
        ::CloseHandle(hWriteThread);
    }

    // check for result from read and write thread
    if ((readContext.bError == false) && (readContext.bFinished == true)
        && (writeContext.bError == false) && (writeContext.bFinished == true))
        nProgress = 100;
    else
        nProgress = -1;

    timer.Stop();
    processContextDecoder.Stop(nProgress == 100);
    processContextEncoder.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pWorkerContext->Status(pDecoderContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130009, pszConvertPipes[8]));
        pWorkerContext->Callback(pDecoderContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(pDecoderContext->nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x0013000B, pszConvertPipes[10]));
        pWorkerContext->Callback(pDecoderContext->nItemId, 100, true, false);
        return true;
    }
}

bool ConvertItem(CItemContext* pContext)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;
    CFormat *pEncFormat = NULL;
    CFormat *pDecFormat = NULL;
    CString szEncInputFile;
    CString szEncOutputFile;
    CString szDecInputFile;
    CString szDecOutputFile;
    CString szOutPath;

    if (pWorkerContext->pConfig->m_Options.bOutputPathChecked == true)
        szOutPath = pWorkerContext->pConfig->m_Options.szOutputPath;
    else
        szOutPath = ::GetFilePath(pContext->item->szPath);

    // prepare encoder
    szEncInputFile = pContext->item->szPath;
    if (::FileExists(szEncInputFile) == false)
    {
        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140001, pszConvertItem[0]));
        return false;
    }

    int nEncoder = pWorkerContext->pConfig->m_Formats.GetFormatById(pContext->item->szFormatId);
    if (nEncoder == -1)
    {
        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140002, pszConvertItem[1]));
        return false;
    }

    pEncFormat = &pWorkerContext->pConfig->m_Formats.GetData(nEncoder);

    if (pContext->item->nPreset >= pEncFormat->m_Presets.GetSize())
    {
        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140003, pszConvertItem[2]));
        return false;
    }

    bool bIsValidEncoderInput = pEncFormat->IsValidInputExtension(::GetFileExtension(szEncInputFile));

    szEncOutputFile = pContext->item->szName + _T(".") + CString(pEncFormat->szOutputExtension).MakeLower();
    if (szOutPath.GetLength() >= 1)
    {
        if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
            szEncOutputFile = szOutPath + szEncOutputFile;
        else
            szEncOutputFile = szOutPath + _T("\\") + szEncOutputFile;
    }

    // prepare decoder
    if (bIsValidEncoderInput == false)
    {
        int nDecoder = pWorkerContext->pConfig->m_Formats.GetDecoderByExtensionAndFormat(pContext->item->szExtension, pEncFormat);
        if (nDecoder == -1)
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140004, pszConvertItem[3]));
            return false;
        }

        pDecFormat = &pWorkerContext->pConfig->m_Formats.GetData(nDecoder);

        if (pDecFormat->nDefaultPreset >= pDecFormat->m_Presets.GetSize())
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140005, pszConvertItem[4]));
            return false;
        }

        bool bIsValidDecoderOutput = pEncFormat->IsValidInputExtension(pDecFormat->szOutputExtension);
        if (bIsValidDecoderOutput == false)
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140006, pszConvertItem[5]));
            return false;
        }

        szDecInputFile = szEncInputFile;
        szDecOutputFile = szEncOutputFile + _T(".") + CString(pDecFormat->szOutputExtension).MakeLower();
    }

    CFileContext decoderContext;
    if (bIsValidEncoderInput == false)
    {
        decoderContext.Init(
            pWorkerContext,
            pDecFormat,
            pDecFormat->nDefaultPreset,
            pContext->item->nId,
            szDecInputFile,
            szDecOutputFile,
            pDecFormat->bPipeInput,
            pDecFormat->bPipeOutput);
    }

    CFileContext encoderContext;
    encoderContext.Init(
        pWorkerContext,
        pEncFormat,
        pContext->item->nPreset,
        pContext->item->nId,
        bIsValidEncoderInput == true ? szEncInputFile : szDecOutputFile,
        szEncOutputFile,
        pEncFormat->bPipeInput,
        pEncFormat->bPipeOutput);

    if (bIsValidEncoderInput == false
        && decoderContext.bUseReadPipes == true
        && decoderContext.bUseWritePipes == true
        && encoderContext.bUseReadPipes == true
        && encoderContext.bUseWritePipes == true)
    {
        // trans-code
        try
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000C, pszConvertItem[11]));

            bool bResult = ConvertFileUsingOnlyPipes(&decoderContext, &encoderContext);
            if (bResult == true)
            {
                if (::FileExists(szEncOutputFile) == false)
                {
                    if (bIsValidEncoderInput == false)
                    {
                        if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szEncInputFile);
                    }

                    pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000D, pszConvertItem[12]));
                    return false;
                }


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

            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000E, pszConvertItem[13]));
            pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }
    else
    {
        // decode
        if (bIsValidEncoderInput == false)
        {
            try
            {
                pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140007, pszConvertItem[6]));

                bool bResult = false;
                if ((decoderContext.bUseReadPipes == false) && (decoderContext.bUseWritePipes == false))
                    bResult = ConvertFileUsingConsole(&decoderContext);
                else
                    bResult = ConvertFileUsingPipes(&decoderContext);
                if (bResult == false)
                {
                    if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szDecOutputFile);

                    return false;
                }

                if (::FileExists(szDecOutputFile) == false)
                {
                    pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140008, pszConvertItem[7]));
                    return false;
                }
            }
            catch (...)
            {
                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile);

                pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140009, pszConvertItem[8]));
                pWorkerContext->Callback(pContext->item->nId, -1, true, true);
            }
        }

        if (pWorkerContext->bRunning == false)
            return false;

        // encode
        try
        {
            if (pEncFormat->nType == 0)
                pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000A, pszConvertItem[9]));
            else if (pEncFormat->nType == 1)
                pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000B, pszConvertItem[10]));

            bool bResult = false;
            if ((encoderContext.bUseReadPipes == false) && (encoderContext.bUseWritePipes == false))
                bResult = ConvertFileUsingConsole(&encoderContext);
            else
                bResult = ConvertFileUsingPipes(&encoderContext);
            if (bResult == true)
            {
                if (::FileExists(szEncOutputFile) == false)
                {
                    if (bIsValidEncoderInput == false)
                    {
                        if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szEncInputFile);
                    }

                    pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000D, pszConvertItem[12]));
                    return false;
                }

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

            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000E, pszConvertItem[13]));
            pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    return false;
}

bool ConvertLoop(CWorkerContext* pWorkerContext)
{
    while (TRUE)
    {
        try
        {
            CItemContext* pContext = NULL;
            if (pWorkerContext->pSync->Wait() == true)
            {
                if (!pWorkerContext->pQueue->IsEmpty())
                    pContext = (CItemContext*)pWorkerContext->pQueue->RemoveHead();
            }
            else
                return false;

            if (pWorkerContext->pSync->Release() == false)
                return false;

            if (pContext != NULL)
            {
                if (pContext->pWorkerContext->bRunning == false)
                    return false;

                pContext->pWorkerContext->Next(pContext->item->nId);
                if (ConvertItem(pContext) == true)
                {
                    pContext->pWorkerContext->nDoneWithoutError++;
                }
                else
                {
                    if (pContext->pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                        return false;
                }

                if (pContext->pWorkerContext->bRunning == false)
                    return false;
            }
            else
            {
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

DWORD WINAPI ConvertThread(LPVOID lpParam)
{
    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    if (pWorkerContext != NULL)
    {
        if (ConvertLoop(pWorkerContext) == true)
            return TRUE;
    }
    return FALSE;
}

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    if (pWorkerContext == NULL)
        return (DWORD)(-1);

    int nItems = pWorkerContext->pConfig->m_Items.GetSize();
    CItemContext *pItemsContext = new CItemContext[nItems];

    pWorkerContext->nTotalFiles = 0;
    pWorkerContext->nProcessedFiles = 0;
    pWorkerContext->nDoneWithoutError = 0;
    pWorkerContext->nErrors = 0;
    pWorkerContext->pQueue = new CObList(nItems);
    pWorkerContext->nProgess = new int[nItems];
    pWorkerContext->nPreviousProgess = new int[nItems];
    pWorkerContext->nLastItemId = -1;

    for (int i = 0; i < nItems; i++)
    {
        CItem& item = pWorkerContext->pConfig->m_Items.GetData(i);
        if (item.bChecked == true)
        {
            pWorkerContext->nTotalFiles++;

            pWorkerContext->nProgess[i] = 0;
            pWorkerContext->nPreviousProgess[i] = 0;

            pItemsContext[i].pWorkerContext = pWorkerContext;
            pItemsContext[i].item = &item;
            pWorkerContext->pQueue->AddTail(&pItemsContext[i]);
        }
        else
        {
            pWorkerContext->nProgess[i] = 100;
            pWorkerContext->nPreviousProgess[i] = 100;
        }
    }

    pWorkerContext->nThreadCount = pWorkerContext->pConfig->m_Options.nThreadCount;
    if (pWorkerContext->nThreadCount < 1)
    {
        // auto-detect number of available threads
        LogicalProcessorInformation info;
        if (GetLogicalProcessorInformation(&info) == 0)
            pWorkerContext->nThreadCount = info.processorCoreCount;
        else
            pWorkerContext->nThreadCount = 1;
    }

    pWorkerContext->pSync = new CSynchronize();

    HANDLE* hConvertThread = new HANDLE[pWorkerContext->nThreadCount];
    DWORD* dwConvertThreadID = new DWORD[pWorkerContext->nThreadCount];

    pWorkerContext->Init();

    // single-threaded
    if (pWorkerContext->nThreadCount == 1)
    {
        ConvertLoop(pWorkerContext);
    }

    // multi-threaded
    if (pWorkerContext->nThreadCount > 1)
    {
        // create worker threads
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
        {
            dwConvertThreadID[i] = i;
            hConvertThread[i] = ::CreateThread(NULL, 0, ConvertThread, pWorkerContext, CREATE_SUSPENDED, &dwConvertThreadID[i]);
            if (hConvertThread[i] == NULL)
                break;
            ::ResumeThread(hConvertThread[i]);
        }

        // wait for all workers to finish
        ::WaitForMultipleObjects(pWorkerContext->nThreadCount, hConvertThread, TRUE, INFINITE);

        // close convert thread handles
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
            ::CloseHandle(hConvertThread[i]);
    }

    delete hConvertThread;
    delete dwConvertThreadID;
    delete pWorkerContext->pSync;
    delete pWorkerContext->pQueue;
    delete pWorkerContext->nProgess;
    delete pWorkerContext->nPreviousProgess;
    delete[] pItemsContext;

    pWorkerContext->Done();
    pWorkerContext->bDone = true;

    return ::CloseHandle(pWorkerContext->hThread);
}
