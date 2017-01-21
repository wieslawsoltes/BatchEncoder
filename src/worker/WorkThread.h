// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "..\Configuration.h"

class WorkerContext
{
public:
    volatile bool bRunning;
    CConfiguration* pConfig;
public:
    HANDLE hThread;
    DWORD dwThreadID;
    volatile int nProgressCurrent;
public:
    int nTotalFiles;
    int nProcessedFiles;
    int nDoneWithoutError;
    int nErrors;
public:
    int nThreadCount;
public:
    WorkerContext(CConfiguration* pConfig)
    {
        this->pConfig = pConfig;
    }
    virtual ~WorkerContext()
    {
    }
public:
    virtual void Init() = 0;
    virtual void Next(int nIndex) = 0;
    virtual void Done() = 0;
    virtual bool Callback(int nProgress, bool bFinished, bool bError = false, double fTime = 0.0, int nIndex = -1) = 0;
    virtual void Status(int nIndex, CString szTime, CString szStatus) = 0;
};

class PipeContext
{
public:
    WorkerContext* pWorkerContext;
    CString szFileName;
    HANDLE hPipe;
    volatile bool bError;
    volatile bool bFinished;
};

class FileContext
{
public:
    WorkerContext* pWorkerContext;
    CString szInputFile;
    CString szOutputFile;
    TCHAR *szCommandLine;
    int nIndex;
    bool bDecode;
    int nTool;
    bool bUseReadPipes;
    bool bUseWritePipes;
};

class ItemContext : public CObject
{
public:
    WorkerContext *pWorkerContext;
    CItem* item;
public:
    ItemContext()
    {
    }
    ItemContext(WorkerContext *pWorkerContext, CItem* item)
    {
        this->pWorkerContext = pWorkerContext;
        this->item = item;
    }
    ItemContext(const ItemContext& context)
    {
        pWorkerContext = context.pWorkerContext;
        item = context.item;
    }
    virtual ~ItemContext()
    {
    }
    const ItemContext& operator=(const ItemContext& context)
    {
        pWorkerContext = context.pWorkerContext;
        item = context.item;
        return *this;
    }
    BOOL operator==(ItemContext context)
    {
        return pWorkerContext == context.pWorkerContext && item == context.item;
    }
};

DWORD WINAPI ReadThread(LPVOID lpParam);

DWORD WINAPI WriteThread(LPVOID lpParam);

bool ConvertFile(FileContext* pContext);

bool ConvertItem(ItemContext* pContext);

DWORD WINAPI ConvertThread(LPVOID lpParam);

DWORD WINAPI WorkThread(LPVOID lpParam);
