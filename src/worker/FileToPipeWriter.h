// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "WorkerContext.h"

class CFileToPipeWriter
{
public:
    CString szFileName;
    HANDLE hPipe;
    int nIndex;
    volatile bool bError;
    volatile bool bFinished;
public:
    CFileToPipeWriter() { }
    virtual ~CFileToPipeWriter() { }
public:
    bool ReadLoop(IWorkerContext* pWorkerContext)
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

        bError = false;
        bFinished = false;

        hFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            bError = true;
            bFinished = true;
            ::CloseHandle(hPipe);
            return false;
        }

        nFileSize = ::GetFileSize64(hFile);
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
                bRunning = pWorkerContext->Callback(nIndex, nProgress, false);
                nPreviousProgress = nProgress;
            }

            if ((pWorkerContext->bRunning == false) || (bRunning == false))
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
