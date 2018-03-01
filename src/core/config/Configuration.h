// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <utility>
#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"
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
        int AddItem(const std::wstring& szPath, int nFormat, int nPreset)
        {
            std::wstring szFormatId = L"";
            int nFormatId = nFormat;
            int nPresetId = nPreset;
            std::wstring szExt = util::Utilities::GetFileExtension(szPath);
            std::wstring szName = util::Utilities::GetOnlyFileName(szPath);
            unsigned __int64 nFileSize = util::Utilities::GetFileSize64(szPath);

            if ((nFormatId >= 0) && (nFormatId < (int)this->m_Formats.size()))
            {
                auto& format = this->m_Formats[nFormatId];
                szFormatId = format.szId;
            }

            if (this->m_Options.bTryToFindDecoder == true)
            {
                int nDecoder = CFormat::GetDecoderByExtension(this->m_Formats, szExt);
                if ((nDecoder >= 0) && (nDecoder < (int)this->m_Formats.size()))
                {
                    auto& format = this->m_Formats[nDecoder];
                    szFormatId = format.szId;
                    nPresetId = format.nDefaultPreset;
                }
            }

            CItem item;

            CPath path;
            path.szPath = szPath;
            path.nSize = nFileSize;
            item.m_Paths.emplace_back(std::move(path));

            item.nSize = nFileSize;
            item.szName = std::move(szName);
            item.szExtension = util::StringHelper::ToUpper(szExt);
            item.szFormatId = std::move(szFormatId);
            item.nPreset = nPresetId;
            item.bChecked = true;

            this->m_Items.emplace_back(std::move(item));

            return (int)this->m_Items.size() - 1;
        }
        void RemoveItems(std::vector<int>& keep)
        {
            std::vector<CItem> items;

            for (size_t i = 0; i < keep.size(); i++)
            {
                int nIndex = keep[i];
                auto& item = this->m_Items[nIndex];
                items.emplace_back(std::move(item));
            }

            this->m_Items = std::move(items);
        }
    public:
        inline bool LookupString(const int nKey, std::wstring& rValue)
        {
            if (this->nLangId >= 0 && this->nLangId < this->m_Languages.size())
            {
                auto& language = this->m_Languages[this->nLangId];
                if (language.m_Strings.count(nKey) == 1)
                {
                    rValue = language.m_Strings[nKey];
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
