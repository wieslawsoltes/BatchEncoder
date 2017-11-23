// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "utilities\TimeCount.h"
#include "worker\WorkerContext.h"

#ifdef DEBUG

class CTraceWorkerContext : public CWorkerContext
{
private:
    CTimeCount timer;
public:
    CTraceWorkerContext(CConfiguration* pConfig);
    CTraceWorkerContext::~CTraceWorkerContext();
public:
    void Init();
    void Next(int nItemId);
    void Done();
    bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false);
    void Status(int nItemId, CString szTime, CString szStatus);
};

#endif
