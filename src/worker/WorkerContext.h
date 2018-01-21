// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "configuration\Configuration.h"

class IWorkerContext
{
public:
    volatile bool bRunning;
    volatile bool bDone;
    volatile int nThreadCount;
    volatile int nTotalFiles;
    volatile int nProcessedFiles;
    volatile int nDoneWithoutError;
    volatile int nErrors;
    volatile int nLastItemId;
    config::CConfiguration* pConfig;
public:
    IWorkerContext(config::CConfiguration* pConfig)
        : pConfig(pConfig) { }
    virtual ~IWorkerContext() { }
public:
    virtual void Init() = 0;
    virtual void Next(int nItemId) = 0;
    virtual void Done() = 0;
    virtual bool IsRunning() = 0;
    virtual bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
    virtual void Status(int nItemId, CString szTime, CString szStatus) = 0;
    virtual CString GetString(int nKey, const TCHAR* szDefault) = 0;
};
