// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CPipe
{
public:
    SECURITY_ATTRIBUTES secattr;
public:
    HANDLE hRead;
    HANDLE hWrite;
public:
    CPipe(bool bInherit)
    {
        ZeroMemory(&this->secattr, sizeof(this->secattr));
        this->secattr.nLength = sizeof(this->secattr);
        this->secattr.bInheritHandle = bInherit;
        this->hRead = INVALID_HANDLE_VALUE;
        this->hWrite = INVALID_HANDLE_VALUE;
    }
    virtual ~CPipe()
    {
    }
public:
    bool Create()
    {
        BOOL bResult = ::CreatePipe(&this->hRead, &this->hWrite, &this->secattr, 0);
        if (bResult == FALSE)
            return false;
        return true;
    }
    void CloseRead()
    {
        ::CloseHandle(this->hRead);
    }
    void CloseWrite()
    {
        ::CloseHandle(this->hWrite);
    }
    bool InheritRead()
    {
        BOOL bResult = ::SetHandleInformation(this->hRead, HANDLE_FLAG_INHERIT, 0);
        if (bResult == FALSE)
        {
            this->CloseRead();
            this->CloseWrite();
            return false;
        }
        return true;
    }
    bool InheritWrite()
    {
        BOOL bResult = ::SetHandleInformation(this->hWrite, HANDLE_FLAG_INHERIT, 0);
        if (bResult == FALSE)
        {
            this->CloseRead();
            this->CloseWrite();
            return false;
        }
        return true;
    }
    bool DuplicateRead()
    {
        BOOL bResult = ::DuplicateHandle(::GetCurrentProcess(),
            this->hRead,
            ::GetCurrentProcess(),
            &this->hRead,
            0,
            TRUE,
            DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS);
        if (bResult == FALSE)
        {
            this->CloseRead();
            this->CloseWrite();
            return false;
        }
        return true;
    }
    bool DuplicateWrite()
    {
        BOOL bResult = ::DuplicateHandle(::GetCurrentProcess(),
            this->hWrite,
            ::GetCurrentProcess(),
            &this->hWrite,
            0,
            TRUE,
            DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS);
        if (bResult == FALSE)
        {
            this->CloseRead();
            this->CloseWrite();
            return false;
        }
        return true;
    }
};
