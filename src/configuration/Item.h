// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "PathsList.h"

namespace config
{
    class CItem
    {
    public:
        int nId;
        CPathsList m_Paths;
        std::wstring szSize;
        std::wstring szName;
        std::wstring szExtension;
        std::wstring szFormatId;
        int nPreset;
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
    };
}
