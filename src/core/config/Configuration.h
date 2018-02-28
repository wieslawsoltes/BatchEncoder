// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "Format.h"
#include "Item.h"
#include "Language.h"
#include "Options.h"
#include "Path.h"
#include "Preset.h"
#include "Settings.h"
#include "Strings.h"
#include "Tool.h"

namespace config
{
    class CConfiguration
    {
    public:
        CSettings m_Settings;
        COptions m_Options;
        std::vector<CFormat> m_Formats;
        std::vector<CItem> m_Items;
        std::vector<CTool> m_Tools;
        size_t nLangId;
        std::vector<CLanguage> m_Languages;
    public:
        inline bool LookupString(const int nKey, std::wstring& rValue)
        {
            if (this->nLangId >= 0 && this->nLangId < this->m_Languages.size())
            {
                auto& language = this->m_Languages[this->nLangId];
                if (language.m_Strings.count(nKey) == 1)
                {
                    rValue = language.m_Strings.at[key];
                    return true;
                }
            }
            return false;
        }
        inline std::wstring GetString(int nKey, const std::wstring& szDefault)
        {
            std::wstring rValue;
            if (this->LookupString(nKey, rValue))
                return rValue;
            return szDefault;
        }
        std::wstring GetString(int nKey)
        {
            std::wstring rValue;
            if (this->LookupString(nKey, rValue))
                return rValue;

            if (config::m_Strings.count(nKey) == 1)
                return config::m_Strings.at(nKey);

            return L"??";
        }
    };
}
