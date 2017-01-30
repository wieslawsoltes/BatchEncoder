// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "WorkThread.h"

DWORD WINAPI ConvertThread(LPVOID lpParam)
{
    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    while (!pWorkerContext->pQueue->IsEmpty())
    {
        CItemContext* pContext = NULL;
        try
        {
            DWORD dwWaitResult = ::WaitForSingleObject(pWorkerContext->hMutex, INFINITE);
            switch (dwWaitResult)
            {
            case WAIT_OBJECT_0:
                {
                    pContext = (CItemContext*)pWorkerContext->pQueue->RemoveHead();
                    if (!::ReleaseMutex(pWorkerContext->hMutex))
                        return(0);
                }
                break;
            case WAIT_ABANDONED:
                return(0);
            }

            if (pContext != NULL)
            {
                if (pContext->pWorkerContext->nThreadCount > 1)
                    pContext->pWorkerContext->Next(pContext->item->nId);

                bool bSuccess = ConvertItem(pContext);
                if (bSuccess == true)
                {
                    pContext->pWorkerContext->nDoneWithoutError++;
                }
                else
                {
                    if (pContext->pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                    {
                        delete pContext;
                        return(0);
                    }
                }

                if (pContext->pWorkerContext->bRunning == false)
                {
                    delete pContext;
                    return(0);
                }

                delete pContext;
            }
        }
        catch (...)
        {
            if (pContext != NULL)
                delete pContext;
        }
    }
    return(0);
}
