// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "WorkThread.h"

DWORD WINAPI ReadThread(LPVOID lpParam)
{
    PipeContext* pContext = (PipeContext*)lpParam;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE pReadBuff[4096];
    BOOL bRes = FALSE;
    DWORD dwReadBytes = 0;
    DWORD dwWriteBytes = 0;
    ULONGLONG nTotalBytesRead = 0;
    ULONGLONG nFileSize = 0;
    int nProgress = -1;
    bool bRunning = true;

    pContext->bError = false;
    pContext->bFinished = false;

    // open existing source file with read-only flag
    hFile = ::CreateFile(pContext->szFileName,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return(1);
    }

    nFileSize = ::GetFileSize64(hFile);
    if (nFileSize == 0)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        ::CloseHandle(hFile);
        return(1);
    }

    // read/write loop
    do
    {
        // read data from source file
        bRes = ::ReadFile(hFile, pReadBuff, 4096, &dwReadBytes, 0);
        if ((bRes == FALSE) || (dwReadBytes == 0))
            break;

        // NOTE: Sleep(0) solves problem writing to pipe errors
        ::Sleep(0);

        // write data to write pipe
        bRes = ::WriteFile(pContext->hPipe, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        // count read/write bytes
        nTotalBytesRead += dwReadBytes;
        nProgress = (int)((nTotalBytesRead * 100) / nFileSize);
        bRunning = pContext->pDlg->WorkerCallback(nProgress, false);
        if (bRunning == false)
            break;
    } while (bRes != FALSE);

    // clean up memory
    ::CloseHandle(hFile);

    // check if all data was processed
    if (nTotalBytesRead != nFileSize)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return(1);
    }

    pContext->bError = false;
    pContext->bFinished = true;
    return(0);
}
