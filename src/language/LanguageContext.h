// Copyright (c) Wies³aw Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "LanguagesList.h"
#include "Language.h"

class CLanguageContext
{
public:
    CLanguagesList m_Languages;
    CLanguage* pLanguage;
public:
    bool LookupString(const int nKey, CString& rValue)
    {
        if (this->pLanguage != NULL)
        {
            if (this->pLanguage->m_Strings.TryGet(nKey, rValue) == TRUE)
                return true;
        }
        return false;
    }
public:
    CString GetString(int nKey, const TCHAR* szDefault)
    {
        CString rValue;
        if (this->LookupString(nKey, rValue))
            return rValue;
        return szDefault;
    }
};
