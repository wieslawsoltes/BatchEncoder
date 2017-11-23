// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "..\collections\StringsMap.h"

class CLanguage
{
public:
    CString szId;
    CString szOriginalName;
    CString szTranslatedName;
    CStringsMap m_Strings;
public:
    CLanguage()
    {
    }
    CLanguage(const CLanguage &other)
    {
        Copy(other);
    }
    CLanguage& operator=(const CLanguage &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CLanguage()
    {
    }
public:
    void Copy(const CLanguage &other)
    {
        this->szId = other.szId;
        this->szOriginalName = other.szOriginalName;
        this->szTranslatedName = other.szTranslatedName;
        this->m_Strings = other.m_Strings;
    }
};
