// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

typedef struct TREAD_DATA
{
    CString szFileName;
    HANDLE hPipe;
    volatile bool bError;
    CBatchEncoderDlg *pDlg;
    volatile bool bFinished;
} READ_DATA, *PREAD_DATA;

ULONGLONG GetFileSize64(HANDLE hFile);
DWORD WINAPI ReadThread(LPVOID lpParam);
DWORD WINAPI WriteThread(LPVOID lpParam);
bool ConvertFile(CBatchEncoderDlg *pDlg,
    CString szInputFile,
    CString szOutputFile,
    TCHAR *szCommandLine,
    int nIndex,
    bool bDecode,
    int nTool,
    bool bUseReadPipes,
    bool bUseWritePipes);
DWORD WINAPI WorkThread(LPVOID lpParam);
