// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CWorkerContext
{
public:
    volatile bool bRunning;
    volatile bool bDone;
    CConfiguration* pConfig;
public:
    HANDLE hThread;
    DWORD dwThreadID;
public:
    volatile int nThreadCount;
    HANDLE hMutex;
    HANDLE* hConvertThread;
    DWORD* dwConvertThreadID;
public:
    volatile int nTotalFiles;
    volatile int nProcessedFiles;
    volatile int nDoneWithoutError;
    volatile int nErrors;
public:
    CObList* pQueue;
    int* nProgess;
    int* nPreviousProgess;
    volatile int nLastItemId;
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
    virtual void Next(int nItemId) = 0;
    virtual void Done() = 0;
    virtual bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
    virtual void Status(int nItemId, CString szTime, CString szStatus) = 0;
};
