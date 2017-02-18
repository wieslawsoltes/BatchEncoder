// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

typedef int Entry(void *param);

typedef struct
{
    Entry *pEntry;
    void *param;
} Params;

inline DWORD WINAPI ThreadStartRoutine(LPVOID lpParam)
{
    Params *pParams = (Params *)lpParam;
    return (DWORD)pParams->pEntry(pParams->param);
}

class CThread
{
    DWORD dwThreadID = 0L;
    HANDLE hThread = NULL;
    Params params;
public:
    CThread()
    {
    }
    virtual ~CThread()
    {
    }
public:
    bool Start(Entry *pEntry, void *param = NULL, bool bSuspended = false)
    {
        params.pEntry = pEntry;
        params.param = param;
        hThread = ::CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE)ThreadStartRoutine,
            (LPVOID)&params,
            bSuspended == true ? CREATE_SUSPENDED : 0,
            &dwThreadID);
        if (hThread == NULL)
            return false;
        return true;
    }
    bool Wait()
    {
        DWORD dwRet = ::WaitForSingleObject(hThread, INFINITE);
        switch (dwRet)
        {
        case WAIT_FAILED:
            return false;
            break;
        case WAIT_ABANDONED:
            break;
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return false;
            break;
        };
        return true;
    }
    bool Wait(int milliseconds)
    {
        DWORD dwRet = ::WaitForSingleObject(hThread, (DWORD)milliseconds);
        switch (dwRet)
        {
        case WAIT_FAILED:
            return false;
            break;
        case WAIT_ABANDONED:
            break;
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return false;
            break;
        };
        return true;

    }
    bool Resume()
    {
        return ::ResumeThread(hThread) != (DWORD)-1;
    }
    bool Pause()
    {
        return ::SuspendThread(hThread) != (DWORD)-1;
    }
    bool Terminate(int code = 0)
    {
        return ::TerminateThread(hThread, (DWORD)code) == TRUE;
    }
    bool Close()
    {
        return ::CloseHandle(hThread) == TRUE;
    }
};
