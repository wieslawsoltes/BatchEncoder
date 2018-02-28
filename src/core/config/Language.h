// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <map>
#include <vector>
#include "utilities\StringHelper.h"

namespace config
{
    class CLanguage
    {
    public:
        std::wstring szId;
        std::wstring szOriginalName;
        std::wstring szTranslatedName;
        std::map<int, std::wstring> m_Strings;
    public:
        static size_t GetLanguageById(std::vector<CLanguage>& languages, const std::wstring& szLanguageId)
        {
            size_t nCount = languages.size();
            for (size_t i = 0; i < nCount; i++)
            {
                CLanguage& language = languages[i];
                if (util::StringHelper::CompareNoCase(szLanguageId, language.szId))
                    return i;
            }
            return -1;
        }
    };
}
