﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace config
{
    class StringHelper
    {
    public:
        static std::vector<std::wstring> Split(const wchar_t *str, wchar_t c)
        {
            std::vector<std::wstring> result;
            do
            {
                const wchar_t *begin = str;
                while(*str != c && *str)
                    str++;
                result.push_back(std::wstring(begin, str));
            } while (0 != *str++);
            return result;
        }
    public:
        static bool CompareNoCase(const std::wstring& str1, const std::wstring& str2)
        {
            std::wstring s1 = str1;
            std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
            std::wstring s2 = str2;
            std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
            return s1 == s2;
        }
    public:
        static bool ContainsNoCase(const std::wstring& str, const std::wstring& value, wchar_t token)
        {
            std::wstring v = value;
            std::transform(v.begin(), v.end(), v.begin(), ::toupper);
            auto tokens = Split(str.c_str(), token);
            for (auto& t : tokens) 
            {
                std::transform(t.begin(), t.end(), t.begin(), ::toupper);
                if (t == v)
                    return true;
            }
            return false;
        }
    };
}