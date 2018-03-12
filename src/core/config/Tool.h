// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "utilities\String.h"

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
    public:
        std::wstring szUrl;
        std::wstring szFile;
        std::wstring szExtract;
        std::wstring szPath;
        std::wstring szStatus;
    public:
        static inline int CountUniqueNames(const std::vector<CTool>& tools, const std::wstring& name)
        {
            auto predicate = [&name](const CTool& tool)
            {
                return util::string::CompareNoCase(tool.szName, name);
            };
            return std::count_if(tools.begin(), tools.end(), predicate);
        }
        static inline bool IsUniqueName(const std::vector<CTool>& tools, const std::wstring& name)
        {
            return CountUniqueNames(tools, name) == 0;
        }
        static inline bool AreNamesUnique(const std::vector<CTool>& tools)
        {
            for (const auto& tool : tools)
            {
                if (CountUniqueNames(tools, tool.szName) != 1)
                    return false;
            }
            return true;
        }
        static inline size_t GetToolByName(const std::vector<CTool>& tools, const std::wstring& szName)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                const CTool& tool = tools[i];
                if (util::string::CompareNoCase(tool.szName, szName) == true)
                    return i;
            }
            return -1;
        }
        static inline bool IsValidFormat(const std::wstring& szFormats, const std::wstring& szFormat)
        {
            return util::string::ContainsNoCase(szFormats, szFormat, token);
        }
        static inline bool IsValidFormat(const std::vector<CTool>& tools, const std::wstring& szFormat)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                const CTool& tool = tools[i];
                if (IsValidFormat(tool.szFormats, szFormat) == true)
                    return true;
            }
            return false;
        }
        static inline size_t GetToolByPath(const std::vector<CTool>& tools, const std::wstring& szPath)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                const CTool& tool = tools[i];
                if (util::string::CompareNoCase(tool.szPath, szPath) == true)
                    return i;
            }
            return -1;
        }
        static inline size_t GetToolByFormat(const std::vector<CTool>& tools, const std::wstring& szFormat)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                const CTool& tool = tools[i];
                if (IsValidFormat(tool.szFormats, szFormat) == true)
                    return i;
            }
            return -1;
        }
        static inline size_t GetToolByFormatAndPlatform(const std::vector<CTool>& tools, const std::wstring& szFormat, const std::wstring& szPlatform)
        {
            size_t nTools = tools.size();
            for (size_t i = 0; i < nTools; i++)
            {
                const CTool& tool = tools[i];
                if ((IsValidFormat(tool.szFormats, szFormat) == true) && (util::string::CompareNoCase(tool.szPlatform, szPlatform) == true))
                    return i;
            }
            return -1;
        }
    public:
        static inline bool CompareName(const CTool& a, const CTool& b)
        {
            return a.szName < b.szName;
        };
        static inline bool ComparePriority(const CTool& a, const CTool& b)
        {
            return a.nPriority > b.nPriority;
        };
        static inline void Sort(std::vector<CTool>& tools)
        {
            std::stable_sort(tools.begin(), tools.end(), &CompareName);
            std::stable_sort(tools.begin(), tools.end(), &ComparePriority);
        }
    };
}
