// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>

namespace config
{
    class CPath
    {
    public:
        std::wstring szPath;
        unsigned __int64 nSize;
    public:
        static inline bool ComparePath(const CPath& a, const CPath& b)
        {
            return a.szPath < b.szPath;
        };
        static inline void Sort(std::vector<CPath>& paths)
        {
            std::stable_sort(paths.begin(), paths.end(), &ComparePath);
        }
    };
}
