// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include "utilities\OutputPath.h"
#include "utilities\Synchronize.h"
#include "utilities\Thread.h"
#include "configuration\Configuration.h"

class CWorkerContext
{
public:
    volatile bool bRunning;
    volatile bool bDone;
    CConfiguration* pConfig;
    COutputPath m_Output;
public:
    CThread m_Thread;
    CSynchronize *pSync;
    CSynchronize *pSyncDir;
    volatile int nThreadCount;
public:
    volatile int nTotalFiles;
    volatile int nProcessedFiles;
    volatile int nDoneWithoutError;
    volatile int nErrors;
public:
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
    CString GetString(int nKey, const TCHAR* szDefault)
    {
        CString rValue;
        if (this->pConfig->LookupString(nKey, rValue))
            return rValue;
        return szDefault;
    }
public:
    virtual void Init() = 0;
    virtual void Next(int nItemId) = 0;
    virtual void Done() = 0;
    virtual bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
    virtual void Status(int nItemId, CString szTime, CString szStatus) = 0;
};
