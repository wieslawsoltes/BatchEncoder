// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>

namespace config
{
    class CPreset
    {
    public:
        std::wstring szName;
        std::wstring szOptions;
    public:
        static bool CompareName(const CPreset& a, const CPreset& b)
        {
            return a.szName < b.szName;
        };
        static void Sort(std::vector<CPreset>& presets)
        {
            std::stable_sort(presets.begin(), presets.end(), &CompareName);
        }
    };
}
