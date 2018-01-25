// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>

namespace config
{
    class StringHelper
    {
    public:
        template<typename T>
        static std::wstring ToHex(T i)
        {
            std::wostringstream stream;
            stream << "0x" 
                   << std::setfill('0') << std::setw(sizeof(T)*2) 
                   << std::hex << i;
          return stream.str();
        }
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
        static std::wstring TowLower(const std::wstring& str)
        {
            std::wstring s = str;
            std::transform(s.begin(), s.end(), s.begin(), ::towlower);
            return s;
        }
        static std::wstring ToUpper(const std::wstring& str)
        {
            std::wstring s = str;
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        }
        static bool CompareNoCase(const std::wstring& str1, const std::wstring& str2)
        {
            std::wstring s1 = ToUpper(str1);
            std::wstring s2 = ToUpper(str2);
            return s1 == s2;
        }
        static bool ContainsNoCase(const std::wstring& str, const std::wstring& value, wchar_t token)
        {
            std::wstring v = ToUpper(value);
            auto tokens = Split(str.c_str(), token);
            for (auto& t : tokens) 
            {
                if (ToUpper(t) == v)
                    return true;
            }
            return false;
        }
        static bool ReplaceNoCase(std::wstring& str, const std::wstring& from, const std::wstring& to)
        {
            std::wstring s = ToUpper(str);
            std::wstring f = ToUpper(from);
            auto pos = s.find(f);
            if (pos != std::string::npos)
            {
                str.replace(pos, f.length(), to);
                return true;
            }
            return false;
        }
    };
}
