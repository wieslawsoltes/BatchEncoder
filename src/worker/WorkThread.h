// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "..\Configuration.h"

class CWorkerContext
{
public:
    volatile bool bRunning;
    CConfiguration* pConfig;
public:
    HANDLE hThread;
    DWORD dwThreadID;
public:
    volatile int nTotalFiles;
    volatile int nProcessedFiles;
    volatile int nDoneWithoutError;
    volatile int nErrors;
public:
    volatile int nThreadCount;
    HANDLE* hConvertThread;
    DWORD* dwConvertThreadID;
    CObList* pQueue;
public:
    CWorkerContext(CConfiguration* pConfig)
    {
        this->pConfig = pConfig;
    }
    virtual ~CWorkerContext()
    {
    }
public:
    virtual void Init() = 0;
    virtual void Next(int nIndex) = 0;
    virtual void Done() = 0;
    virtual bool Callback(int nIndex, int nProgress, bool bFinished, bool bError = false, double fTime = 0.0) = 0;
    virtual void Status(int nIndex, CString szTime, CString szStatus) = 0;
};

class CPipeContext
{
public:
    CWorkerContext* pWorkerContext;
    CString szFileName;
    HANDLE hPipe;
    int nIndex;
    volatile bool bError;
    volatile bool bFinished;
};

class CFileContext
{
public:
    CWorkerContext* pWorkerContext;
    CString szInputFile;
    CString szOutputFile;
    TCHAR *szCommandLine;
    int nIndex;
    bool bDecode;
    int nTool;
    bool bUseReadPipes;
    bool bUseWritePipes;
};

class CItemContext : public CObject
{
public:
    CWorkerContext *pWorkerContext;
    CItem* item;
public:
    CItemContext()
    {
    }
    CItemContext(CWorkerContext *pWorkerContext, CItem* item)
    {
        this->pWorkerContext = pWorkerContext;
        this->item = item;
    }
    CItemContext(const CItemContext& context)
    {
        pWorkerContext = context.pWorkerContext;
        item = context.item;
    }
    virtual ~CItemContext()
    {
    }
    const CItemContext& operator=(const CItemContext& context)
    {
        pWorkerContext = context.pWorkerContext;
        item = context.item;
        return *this;
    }
    BOOL operator==(CItemContext context)
    {
        return pWorkerContext == context.pWorkerContext && item == context.item;
    }
};

DWORD WINAPI ReadThread(LPVOID lpParam);

DWORD WINAPI WriteThread(LPVOID lpParam);

bool ConvertFile(CFileContext* pContext);

bool ConvertItem(CItemContext* pContext);

DWORD WINAPI ConvertThread(LPVOID lpParam);

DWORD WINAPI WorkThread(LPVOID lpParam);
