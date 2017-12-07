// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>

class CPath
{
public:
    CString szPath;
    CString szSize;
public:
    CPath()
    {
    }
    CPath(const CPath &other)
    {
        Copy(other);
    }
    CPath& operator=(const CPath &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CPath()
    {
    }
public:
    void Copy(const CPath &other)
    {
        this->szPath = other.szPath;
        this->szSize = other.szSize;
    }
};
