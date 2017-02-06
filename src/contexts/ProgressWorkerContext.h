// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "..\utilities\TimeCount.h"
#include "..\worker\WorkerContext.h"
#include "..\dialogs\MainDlg.h"

class CProgressWorkerContext : public CWorkerContext
{
private:
    CTimeCount timer;
    CMainDlg *pDlg;
public:
    CProgressWorkerContext(CConfiguration* pConfig, CMainDlg* pDlg);
    virtual ~CProgressWorkerContext();
public:
    void Init();
    void Next(int nItemId);
    void Done();
    bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false);
    void Status(int nItemId, CString szTime, CString szStatus);
};
