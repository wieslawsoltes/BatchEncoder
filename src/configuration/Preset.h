// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>

class CPreset
{
public:
    CString szName;
    CString szOptions;
public:
    CPreset()
    {
    }
    CPreset(const CPreset &other)
    {
        Copy(other);
    }
    CPreset& operator=(const CPreset &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CPreset()
    {
    }
public:
    void Copy(const CPreset &other)
    {
        this->szName = other.szName;
        this->szOptions = other.szOptions;
    }
};
