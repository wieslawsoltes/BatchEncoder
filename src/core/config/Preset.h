// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "utilities\String.h"

namespace config
{
    class CPreset
    {
    public:
        std::wstring szName;
        std::wstring szOptions;
    public:
        static inline int CountUniqueNames(const std::vector<CPreset>& presets, const std::wstring& name)
        {
            auto predicate = [&name](const CPreset& preset)
            {
                return util::string::CompareNoCase(preset.szName, name);
            };
            return std::count_if(presets.begin(), presets.end(), predicate);
        }
        static inline bool IsUniqueName(const std::vector<CPreset>& presets, const std::wstring& name)
        {
            return CountUniqueNames(presets, name) == 0;
        }
        static inline bool AreNamesUnique(const std::vector<CPreset>& presets)
        {
            for (const auto& preset : presets)
            {
                if (CountUniqueNames(presets, preset.szName) != 1)
                    return false;
            }
            return true;
        }
    public:
        static inline bool CompareName(const CPreset& a, const CPreset& b)
        {
            return a.szName < b.szName;
        };
        static inline void Sort(std::vector<CPreset>& presets)
        {
            std::stable_sort(presets.begin(), presets.end(), &CompareName);
        }
    };
}
