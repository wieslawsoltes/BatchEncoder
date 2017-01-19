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

class ItemContext : public CObject
{
public:
    CBatchEncoderDlg *pDlg;
    CItem* item;
public:
    ItemContext()
    {
    }
    ItemContext(CBatchEncoderDlg *pDlg, CItem* item)
    {  
        this->pDlg = pDlg;
        this->item = item;
    }
    ItemContext(const ItemContext& context) 
    { 
        pDlg = context.pDlg; 
        item = context.item;
    }
    const ItemContext& operator=(const ItemContext& context)
    {
        pDlg = context.pDlg;
        item = context.item;
        return *this;
    }
    BOOL operator==(ItemContext context)
    {
        return pDlg == context.pDlg && item == context.item;
    }
};

DWORD WINAPI ReadThread(LPVOID lpParam);

DWORD WINAPI WriteThread(LPVOID lpParam);

bool ConvertFile(FileContext* pContext);

bool ConvertItem(ItemContext* pContext);

DWORD WINAPI ConvertThread(LPVOID lpParam);

DWORD WINAPI WorkThread(LPVOID lpParam);
