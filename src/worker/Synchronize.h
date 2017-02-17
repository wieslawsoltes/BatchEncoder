// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CSynchronize
{
    HANDLE hMutex;
public:
    CSynchronize()
    {
        hMutex = ::CreateMutex(NULL, FALSE, NULL);
    }
    virtual ~CSynchronize()
    {
        ::CloseHandle(hMutex);
    }
public:
    bool Wait()
    {
        DWORD dwWaitResult = ::WaitForSingleObject(hMutex, INFINITE);
        switch (dwWaitResult)
        {
        case WAIT_OBJECT_0:
            return true;
        case WAIT_ABANDONED:
            return false;
        }
        return false;
    }
    bool Release()
    {
        if (!::ReleaseMutex(hMutex))
            return false;
        return true;
    }
};
