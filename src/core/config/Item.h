// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "Path.h"

namespace config
{
    class CItem
    {
    public:
        int nId;
        std::vector<CPath> m_Paths;
        unsigned __int64 nSize;
        std::wstring szName;
        std::wstring szExtension;
        std::wstring szFormatId;
        size_t nPreset;
        std::wstring szOptions;
        bool bChecked;
        std::wstring szTime;
        std::wstring szStatus;
        bool bFinished;
        int nProgress;
        int nPreviousProgress;
    public:
        void ResetProgress()
        {
            this->bFinished = false;
            this->nProgress = 0;
            this->nPreviousProgress = 0;
        }
    public:
        static inline bool CompareName(const CItem& a, const CItem& b)
        {
            return a.szName < b.szName;
        };
        static inline void Sort(std::vector<CItem>& items)
        {
            std::stable_sort(items.begin(), items.end(), &CompareName);
        }
    };
}
