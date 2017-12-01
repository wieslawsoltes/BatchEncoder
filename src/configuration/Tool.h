// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>

class CTool
{
public:
    const CString szFormatsTokens = _T(",");
public:
    CString szName;
    CString szPlatform;
    CString szFormats;
    CString szUrl;
    CString szFile;
    CString szExtract;
    CString szPath;
public:
    CTool()
    {
    }
    CTool(const CTool &other)
    {
        Copy(other);
    }
    CTool& operator=(const CTool &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CTool()
    {
    }
public:
    void Copy(const CTool &other)
    {
        this->szName = other.szName;
        this->szPlatform = other.szPlatform;
        this->szFormats = other.szFormats;
        this->szUrl = other.szUrl;
        this->szFile = other.szFile;
        this->szExtract = other.szExtract;
        this->szPath = other.szPath;
    }
public:
    bool IsValidFormat(CString szFormat)
    {
        int nTokenPos = 0;
        CString strToken = this->szFormats.Tokenize(szFormatsTokens, nTokenPos);
        while (!strToken.IsEmpty())
        {
            if (strToken.CompareNoCase(szFormat) == 0)
                return true;

            strToken = this->szFormats.Tokenize(szFormatsTokens, nTokenPos);
        }
        return false;
    }
};
