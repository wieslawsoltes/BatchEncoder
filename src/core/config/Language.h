#pragma once

#include <string>
#include <map>
#include <vector>
#include "utilities\String.h"

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
        static inline size_t GetLanguageById(const std::vector<CLanguage>& languages, const std::wstring& szLanguageId)
        {
            size_t nCount = languages.size();
            for (size_t i = 0; i < nCount; i++)
            {
                const CLanguage& language = languages[i];
                if (util::string::CompareNoCase(szLanguageId, language.szId))
                    return i;
            }
            return -1;
        }
    };
}
