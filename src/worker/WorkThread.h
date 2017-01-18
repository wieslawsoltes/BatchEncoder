// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

typedef struct tagPipeContext
{
    CBatchEncoderDlg *pDlg;
    CString szFileName;
    HANDLE hPipe;
    volatile bool bError;
    volatile bool bFinished;
} PipeContext;

typedef struct tagConvertContext
{
    CBatchEncoderDlg *pDlg;
    CString szInputFile;
    CString szOutputFile;
    TCHAR *szCommandLine;
    int nIndex;
    bool bDecode;
    int nTool;
    bool bUseReadPipes;
    bool bUseWritePipes;
} ConvertContext;

DWORD WINAPI ReadThread(LPVOID lpParam);

DWORD WINAPI WriteThread(LPVOID lpParam);

bool ConvertFile(ConvertContext* lpContext);

DWORD WINAPI WorkThread(LPVOID lpParam);
