// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

DWORD WINAPI ConvertThread(LPVOID lpParam)
{
    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    while (!pWorkerContext->pQueue->IsEmpty())
    {
        try
        {
            CItemContext* pContext = NULL;
            DWORD dwWaitResult = ::WaitForSingleObject(pWorkerContext->hMutex, INFINITE);
            switch (dwWaitResult)
            {
            case WAIT_OBJECT_0:
            {
                pContext = (CItemContext*)pWorkerContext->pQueue->RemoveHead();
                if (!::ReleaseMutex(pWorkerContext->hMutex))
                    return FALSE;
            }
            break;
            case WAIT_ABANDONED:
                return FALSE;
            }

            if (pContext != NULL)
            {
                if (pContext->pWorkerContext->nThreadCount > 1)
                    pContext->pWorkerContext->Next(pContext->item->nId);

                if (ConvertItem(pContext) == true)
                {
                    pContext->pWorkerContext->nDoneWithoutError++;
                }
                else
                {
                    if (pContext->pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                        return FALSE;
                }

                if (pContext->pWorkerContext->bRunning == false)
                    return FALSE;
            }
        }
        catch (...) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    ::SetCurrentDirectory(::GetExeFilePath());

    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    if (pWorkerContext == NULL)
        return (DWORD)(-1);

    int nItems = pWorkerContext->pConfig->m_Items.GetSize();
    CItemContext *context = new CItemContext[nItems];

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
    
    pWorkerContext->hMutex = ::CreateMutex(NULL, FALSE, NULL);
    pWorkerContext->nTotalFiles = 0;
    pWorkerContext->nProcessedFiles = 0;
    pWorkerContext->nDoneWithoutError = 0;
    pWorkerContext->nErrors = 0;
    pWorkerContext->hConvertThread = new HANDLE[pWorkerContext->nThreadCount];
    pWorkerContext->dwConvertThreadID = new DWORD[pWorkerContext->nThreadCount];
    pWorkerContext->pQueue = new CObList();
    pWorkerContext->nProgess = new int[nItems];
    pWorkerContext->nPreviousProgess = new int[nItems];
    pWorkerContext->nLastItemId = -1;

    for (int i = 0; i < nItems; i++)
    {
        CItem& item = pWorkerContext->pConfig->m_Items.GetData(i);
        if (item.bChecked == true)
        {
            pWorkerContext->nTotalFiles++;
            pWorkerContext->nProgess[i] = 0;
            pWorkerContext->nPreviousProgess[i] = 0;
            
            context[i] = new CItemContext(pWorkerContext, &item);
            pWorkerContext->pQueue->AddTail(context[i]);
        }
        else
        {
            pWorkerContext->nProgess[i] = 100;
            pWorkerContext->nPreviousProgess[i] = 100;
        }
    }

    pWorkerContext->Init();
    
    // single-threaded
    if (pWorkerContext->nThreadCount == 1)
    {
        while (!pWorkerContext->pQueue->IsEmpty())
        {
            try
            {
                CItemContext *pContext = (CItemContext*)pWorkerContext->pQueue->RemoveHead();
                if (pContext != NULL)
                {
                    pWorkerContext->Next(pContext->item->nId);
                    if (ConvertItem(pContext) == true)
                    {
                        pWorkerContext->nDoneWithoutError++;
                    }
                    else
                    {
                        if (pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                            break;
                    }
                }
            }
            catch (...) 
            {
                if (pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                    break;
            }

            if (pWorkerContext->bRunning == false)
                break;  
        }
    }
    
    // multi-threaded
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
    }

    delete pWorkerContext->hConvertThread;
    delete pWorkerContext->dwConvertThreadID;
    delete pWorkerContext->pQueue;
    delete pWorkerContext->nProgess;
    delete pWorkerContext->nPreviousProgess;
    
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = pWorkerContext->pConfig->m_Items.GetData(i);
        if (item.bChecked == true)
            delete context[i];
    }
    delete context;

    ::CloseHandle(pWorkerContext->hMutex);

    pWorkerContext->Done();
    pWorkerContext->bDone = true;

    return ::CloseHandle(pWorkerContext->hThread);
}
