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

    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    if (pWorkerContext == NULL)
        return (DWORD)(-1);

    pWorkerContext->hMutex = ::CreateMutex(NULL, FALSE, NULL);

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

    pWorkerContext->hConvertThread = new HANDLE[pWorkerContext->nThreadCount];
    pWorkerContext->dwConvertThreadID = new DWORD[pWorkerContext->nThreadCount];
    pWorkerContext->pQueue = new CObList();

    pWorkerContext->Init();

    int nItems = pWorkerContext->pConfig->m_Items.GetSize();

    pWorkerContext->nProgess = new int[nItems];
    pWorkerContext->nPreviousProgess = new int[nItems];

    for (int i = 0; i < nItems; i++)
    {
        if (pWorkerContext->pConfig->m_Items.GetData(i).bChecked == TRUE)
        {
            pWorkerContext->nTotalFiles++;
            pWorkerContext->nProgess[i] = 0;
            pWorkerContext->nPreviousProgess[i] = 0;
        }
        else
        {
            pWorkerContext->nProgess[i] = 100;
            pWorkerContext->nPreviousProgess[i] = 100;
        }
    }

    pWorkerContext->nLastItemId = -1;

    for (int i = 0; i < nItems; i++)
    {
        // get next file name and check if we need to encode/decode/trans-code
        CItem& item = pWorkerContext->pConfig->m_Items.GetData(i);
        if (item.bChecked == true)
        {
            if (pWorkerContext->nThreadCount > 1)
            {
                // insert work item to queue
                CItemContext* context = new CItemContext(pWorkerContext, &item);
                pWorkerContext->pQueue->AddTail(context);
            }
            else
            {
                pWorkerContext->Next(i);

                CItemContext context(pWorkerContext, &item);
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
            pWorkerContext->dwConvertThreadID[i] = i;
            pWorkerContext->hConvertThread[i] = ::CreateThread(NULL, 0, ConvertThread, pWorkerContext, CREATE_SUSPENDED, &pWorkerContext->dwConvertThreadID[i]);
            if (pWorkerContext->hConvertThread[i] == NULL)
            {
                break;
            }
            ::ResumeThread(pWorkerContext->hConvertThread[i]);
        }

        // wait for all workers to finish
        ::WaitForMultipleObjects(pWorkerContext->nThreadCount, pWorkerContext->hConvertThread, TRUE, INFINITE);

        // close convert thread handles
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
            ::CloseHandle(pWorkerContext->hConvertThread[i]);

        delete pWorkerContext->hConvertThread;
        delete pWorkerContext->dwConvertThreadID;
        delete pWorkerContext->pQueue;
        delete pWorkerContext->nProgess;
        delete pWorkerContext->nPreviousProgess;
    }

    pWorkerContext->Done();

    ::CloseHandle(pWorkerContext->hMutex);

    pWorkerContext->bDone = true;

    return ::CloseHandle(pWorkerContext->hThread);
}
