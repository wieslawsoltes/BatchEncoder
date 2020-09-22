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
