// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "utilities\TimeCount.h"
#include "Configuration.h"
#include "worker\WorkerContext.h"

#ifdef DEBUG
class CTraceWorkerContext : public CWorkerContext
{
private:
    CTimeCount timer;
public:
    CTraceWorkerContext(CConfiguration* pConfig)
        : CWorkerContext(pConfig)
    {
        this->bDone = true;
    }
    CTraceWorkerContext::~CTraceWorkerContext()
    {

    }
public:
    void Init()
    {
        this->timer.Start();
    }
    void Next(int nItemId)
    {
        this->nProcessedFiles++;
        this->nErrors = (this->nProcessedFiles - 1) - this->nDoneWithoutError;
        this->nLastItemId = nItemId;
    }
    void Done()
    {
        this->timer.Stop();
        this->nErrors = this->nProcessedFiles - this->nDoneWithoutError;
    }
    bool Callback(int nItemId, int nProgress, bool bFinished, bool bError)
    {
        if (bError == true)
        {
            if (pConfig->m_Options.bStopOnErrors == true)
            {
                this->bRunning = false;
            }
            return this->bRunning;
        }

        if (bFinished == false && this->bRunning == true)
        {
            nProgess[nItemId] = nProgress;
            if (nPreviousProgess[nItemId] > nProgress)
                nPreviousProgess[nItemId] = nProgress;

            if (nItemId > this->nLastItemId)
            {
                this->nLastItemId = nItemId;
            }

            int nTotalProgress = 0;
            int nItems = pConfig->m_Items.GetSize();
            for (int i = 0; i < nItems; i++)
            {
                if (pConfig->m_Items.GetData(i).bChecked == TRUE)
                {
                    int nItemProgress = nProgess[i];
                    int nItemPreviousProgress = nPreviousProgess[i];

                    nTotalProgress += nItemProgress;

                    if (nItemProgress > 0 && nItemProgress < 100 && nItemProgress > nItemPreviousProgress)
                    {
                        CString szOutput;
                        szOutput.Format(_T("Progress: [%d] %d%%\n"), i, nItemProgress);
                        OutputDebugString(szOutput);
                        nPreviousProgess[i] = nItemProgress;
                    }
                }
            }

            int nPos = nTotalProgress / nTotalFiles;
            CString szOutput;
            szOutput.Format(_T("Total: %d%%\n"), nPos);
            OutputDebugString(szOutput);
        }

        return this->bRunning;
    }
    void Status(int nItemId, CString szTime, CString szStatus)
    {
        CString szOutput;
        szOutput.Format(_T("Status: [%d] %s : %s\n"), nItemId, szTime, szStatus);
        OutputDebugString(szOutput);
    };
};
#endif
