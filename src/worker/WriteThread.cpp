// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "WorkThread.h"

DWORD WINAPI WriteThread(LPVOID lpParam)
{
    PipeContext* pContext = (PipeContext*)lpParam;
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

    // open existing source file with read-only flag
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
        return(1);
    }

    // read/write loop
    do
    {
        // read data from source pipe
        bRes = ::ReadFile(pContext->hPipe, pReadBuff, 4096, &dwReadBytes, 0);
        if ((bRes == FALSE) || (dwReadBytes == 0))
            break;

        // write data to file
        bRes = ::WriteFile(hFile, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        // count read/write bytes
        nTotalBytesWrite += dwReadBytes;

        // handle user Stop
        if (pContext->pDlg->bRunning == false)
            break;
    } 
    while (bRes != FALSE);

    // clean up memory
    ::CloseHandle(hFile);

    pContext->bError = false;
    pContext->bFinished = true;
    return(0);
}
