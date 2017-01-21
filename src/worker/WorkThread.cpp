// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    ::UpdatePath();

    WorkerContext* pWorkerContext = (WorkerContext*)lpParam;
    if (pWorkerContext == NULL)
        return (DWORD)(-1);

    pWorkerContext->nTotalFiles = 0;
    pWorkerContext->nProcessedFiles = 0;
    pWorkerContext->nDoneWithoutError = 0;
    pWorkerContext->nErrors = 0;

    pWorkerContext->nThreadCount = pWorkerContext->pConfig->m_Options.nThreadCount;
    if (pWorkerContext->nThreadCount < 1)
    {
        // auto-detect number of available threads
        LogicalProcessorInformation info;
        if (GetLogicalProcessorInformation(&info) == 0)
            pWorkerContext->nThreadCount = info.processorCoreCount;
        else
            pWorkerContext->nThreadCount = 1;
    }

    HANDLE* hConvertThread = new HANDLE[pWorkerContext->nThreadCount];
    DWORD* dwThreadID = new DWORD[pWorkerContext->nThreadCount];

    CObList queue;

    pWorkerContext->Init();

    int nItems = pWorkerContext->pConfig->m_Items.GetSize();
    for (int i = 0; i < nItems; i++)
    {
        if (pWorkerContext->pConfig->m_Items.GetData(i).bChecked == TRUE)
            pWorkerContext->nTotalFiles++;
    }

    for (int i = 0; i < nItems; i++)
    {
        // get next file name and check if we need to encode/decode/trans-code
        CItem& item = pWorkerContext->pConfig->m_Items.GetData(i);
        if (item.bChecked == true)
        {
            pWorkerContext->Next(i);

            if (pWorkerContext->nThreadCount > 1)
            {
                // insert work item to queue
                ItemContext* context = new ItemContext(pWorkerContext, &item);
                queue.AddTail(context);
            }
            else
            {
                ItemContext context(pWorkerContext, &item);
                bool bSuccess = ConvertItem(&context);
                if (bSuccess == true)
                {
                    pWorkerContext->nDoneWithoutError++;
                }
                else
                {
                    if (pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                        break;
                }

                if (pWorkerContext->bRunning == false)
                    break;
            }
        }
    }

    if (pWorkerContext->nThreadCount > 1)
    {
        // create worker threads
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
        {
            dwThreadID[i] = i;
            hConvertThread[i] = ::CreateThread(NULL, 0, ConvertThread, &queue, CREATE_SUSPENDED, &dwThreadID[i]);
            if (hConvertThread[i] == NULL)
            {
                break;
            }
            ::ResumeThread(hConvertThread[i]);
        }

        // wait for all workers to finish
        ::WaitForMultipleObjects(pWorkerContext->nThreadCount, hConvertThread, TRUE, INFINITE);

        // close convert thread handles
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
        {
            ::CloseHandle(hConvertThread[i]);
        }

        delete hConvertThread;
        delete dwThreadID;
    }

    pWorkerContext->Done();

    return ::CloseHandle(pWorkerContext->hThread);
}
