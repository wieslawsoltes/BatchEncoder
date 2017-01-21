// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

typedef struct tagLogicalProcessorInformation
{
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
} LogicalProcessorInformation;

DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}

int GetLogicalProcessorInformation(LogicalProcessorInformation* info)
{
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;

    glpi = (LPFN_GLPI)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation");
    if (NULL == glpi)
    {
#ifdef _DEBUG
        _tprintf(_T("\nGetLogicalProcessorInformation is not supported.\n"));
#endif
        return (1);
    }

    while (!done)
    {
        DWORD rc = glpi(buffer, &returnLength);
        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
                if (NULL == buffer)
                {
#ifdef _DEBUG
                    _tprintf(_T("\nError: Allocation failure\n"));
#endif
                    return (2);
                }
            }
            else
            {
#ifdef _DEBUG
                _tprintf(_T("\nError %d\n"), GetLastError());
#endif
                return (3);
            }
        }
        else
        {
            done = TRUE;
        }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        switch (ptr->Relationship)
        {
        case RelationNumaNode:
            // Non-NUMA systems report a single record of this type.
            info->numaNodeCount++;
            break;
        case RelationProcessorCore:
            info->processorCoreCount++;
            // A hyperthreaded core supplies more than one logical processor.
            info->logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;
        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
            Cache = &ptr->Cache;
            if (Cache->Level == 1)
            {
                info->processorL1CacheCount++;
            }
            else if (Cache->Level == 2)
            {
                info->processorL2CacheCount++;
            }
            else if (Cache->Level == 3)
            {
                info->processorL3CacheCount++;
            }
            break;
        case RelationProcessorPackage:
            // Logical processors share a physical package.
            info->processorPackageCount++;
            break;
        default:
#ifdef _DEBUG
            _tprintf(_T("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
#endif
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }

#ifdef _DEBUG
    _tprintf(_T("\nGetLogicalProcessorInformation results:\n"));
    _tprintf(_T("Number of NUMA nodes: %d\n"), info->numaNodeCount);
    _tprintf(_T("Number of physical processor packages: %d\n"), info->processorPackageCount);
    _tprintf(_T("Number of processor cores: %d\n"), info->processorCoreCount);
    _tprintf(_T("Number of logical processors: %d\n"), info->logicalProcessorCount);
    _tprintf(_T("Number of processor L1/L2/L3 caches: %d/%d/%d\n"),
        info->processorL1CacheCount,
        info->processorL2CacheCount,
        info->processorL3CacheCount);
#endif

    free(buffer);
    return (0);
}

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
