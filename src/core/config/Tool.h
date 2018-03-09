// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "utilities\String.h"
#include "utilities\Utilities.h"

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
            return util::string::ContainsNoCase(this->szFormats, szFormat, token);
        }
    public:
        static bool IsUniqueName(const std::vector<CTool>& tools, const std::wstring& name)
        {
            auto predicate = [&name](const CTool& tool) { return tool.szName == name; };
            return std::count_if(tools.begin(), tools.end(), predicate) == 0;
        }
        static size_t GetToolByName(const std::vector<CTool>& tools, const std::wstring& szName)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                CTool& tool = tools[i];
                if (util::string::CompareNoCase(tool.szName, szName) == true)
                    return i;
            }
            return -1;
        }
        static size_t GetToolByPath(const std::vector<CTool>& tools, const std::wstring& szPath)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                CTool& tool = tools[i];
                if (util::string::CompareNoCase(tool.szPath, szPath) == true)
                    return i;
            }
            return -1;
        }
        static size_t GetToolByFormat(const std::vector<CTool>& tools, const std::wstring& szFormat)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                CTool& tool = tools[i];
                if (tool.IsValidFormat(szFormat) == true)
                    return i;
            }
            return -1;
        }
        static size_t GetToolByFormatAndPlatform(const std::vector<CTool>& tools, const std::wstring& szFormat, const std::wstring& szPlatform)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                CTool& tool = tools[i];
                if ((tool.IsValidFormat(szFormat) == true) && (util::string::CompareNoCase(tool.szPlatform, szPlatform) == true))
                    return i;
            }
            return -1;
        }
        static bool IsValidFormat(const std::vector<CTool>& tools, const std::wstring& szFormat)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                CTool& tool = tools[i];
                if (tool.IsValidFormat(szFormat) == true)
                    return true;
            }
            return false;
        }
    public:
        static bool CompareName(const CTool& a, const CTool& b)
        {
            return a.szName < b.szName;
        };
        static bool ComparePriority(const CTool& a, const CTool& b)
        {
            return a.nPriority > b.nPriority;
        };
        static void Sort(std::vector<CTool>& tools)
        {
            std::stable_sort(tools.begin(), tools.end(), &CompareName);
            std::stable_sort(tools.begin(), tools.end(), &ComparePriority);
        }
    };
}
