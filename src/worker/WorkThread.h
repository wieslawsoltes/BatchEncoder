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

typedef struct tagFileContext
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
} FileContext;

typedef struct tagItemContext
{
    CBatchEncoderDlg *pDlg;
    CItem* item;
} ItemContext;

DWORD WINAPI ReadThread(LPVOID lpParam);

DWORD WINAPI WriteThread(LPVOID lpParam);

bool ConvertFile(FileContext* pContext);

bool ConvertItem(ItemContext* pContext);

DWORD WINAPI WorkThread(LPVOID lpParam);
