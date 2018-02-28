// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>

namespace config
{
    class CTool
    {
    private:
        static const wchar_t token = ',';
    public:
        std::wstring szName;
        std::wstring szPlatform;
        int nPriority;
        std::wstring szFormats;
        std::wstring szUrl;
        std::wstring szFile;
        std::wstring szExtract;
        std::wstring szPath;
        std::wstring szStatus;
    public:
        bool IsValidFormat(const std::wstring& szFormat)
        {
            return util::StringHelper::ContainsNoCase(this->szFormats, szFormat, token);
        }
    public:
        static bool CompareName(const CTool& a, const CTool& b)
        {
            return a.szName < b.szName;
        };
        static bool ComparePriority(const CTool& a, const CTool& b)
        {
            return a.nPriority < b.nPriority;
        };
        static void Sort(std::vector<CTool>& tools)
        {
            std::stable_sort(tools.begin(), tools.end(), &CompareName);
            std::stable_sort(tools.begin(), tools.end(), &ComparePriority);
        }
    };
}
