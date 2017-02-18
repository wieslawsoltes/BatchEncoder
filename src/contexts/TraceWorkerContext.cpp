// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\MainApp.h"
#include "TraceWorkerContext.h"

#ifdef DEBUG

CTraceWorkerContext::CTraceWorkerContext(CConfiguration* pConfig)
    : CWorkerContext(pConfig)
{
    this->bDone = true;
}

CTraceWorkerContext::~CTraceWorkerContext()
{

}

void CTraceWorkerContext::Init()
{
    this->timer.Start();
}

void CTraceWorkerContext::Next(int nItemId)
{
    this->nProcessedFiles++;
    this->nErrors = (this->nProcessedFiles - 1) - this->nDoneWithoutError;
    this->nLastItemId = nItemId;
}

void CTraceWorkerContext::Done()
{
    this->timer.Stop();
    this->nErrors = this->nProcessedFiles - this->nDoneWithoutError;
}

bool CTraceWorkerContext::Callback(int nItemId, int nProgress, bool bFinished, bool bError)
{
    if (bError == true)
    {
        CItem &item = pConfig->m_Items.GetData(nItemId);
        item.bFinished = true;

        if (pConfig->m_Options.bStopOnErrors == true)
        {
            this->bRunning = false;
        }
        return this->bRunning;
    }

    if (bFinished == true)
    {
        CItem &item = pConfig->m_Items.GetData(nItemId);
        item.bFinished = true;
    }

    if ((bFinished == false) && (this->bRunning == true))
    {
        CItem &current = pConfig->m_Items.GetData(nItemId);
        current.nProgress = nProgress;
        if (current.nPreviousProgress > nProgress)
            current.nPreviousProgress = nProgress;

        if (nItemId > this->nLastItemId)
        {
            this->nLastItemId = nItemId;
        }

        int nTotalProgress = 0;
        int nItems = pConfig->m_Items.GetSize();
        for (int i = 0; i < nItems; i++)
        {
            CItem &item = pConfig->m_Items.GetData(i);
            if (item.bChecked == true)
            {
                int nItemProgress = item.nProgress;
                int nItemPreviousProgress = item.nPreviousProgress;

                nTotalProgress += nItemProgress;

                if (item.bFinished == false)
                {
                    if (nItemProgress > 0 && nItemProgress < 100 && nItemProgress > nItemPreviousProgress)
                    {
                        CString szOutput;
                        szOutput.Format(_T("Progress: [%d] %d%%\n"), i, nItemProgress);
                        OutputDebugString(szOutput);
                        item.nPreviousProgress = nItemProgress;
                    }
                    else if (nItemProgress == 100 && nItemProgress > nItemPreviousProgress)
                    {
                        item.nPreviousProgress = nItemProgress;
                    }
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

void CTraceWorkerContext::Status(int nItemId, CString szTime, CString szStatus)
{
    CString szOutput;
    szOutput.Format(_T("Status: [%d] %s : %s\n"), nItemId, szTime, szStatus);
    OutputDebugString(szOutput);
};

#endif
