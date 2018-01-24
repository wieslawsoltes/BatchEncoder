// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\ListT.h"
#include "Tool.h"

namespace config
{
    class CToolsList : public util::CListT<CTool>
    {
    public:
        int GetToolByFormat(const std::wstring& szFormat)
        {
            int nTools = this->Count();
            for (int i = 0; i < nTools; i++)
            {
                CTool& tool = this->Get(i);
                if (tool.IsValidFormat(szFormat) == true)
                {
                    return i;
                }
            }
            return -1;
        }
        int GetToolByFormatAndPlatform(const std::wstring& szFormat, const std::wstring& szPlatform)
        {
            int nTools = this->Count();
            for (int i = 0; i < nTools; i++)
            {
                CTool& tool = this->Get(i);
                if ((tool.IsValidFormat(szFormat) == true) && (config::StringHelper::CompareNoCase(tool.szPlatform, szPlatform) == true))
                {
                    return i;
                }
            }
            return -1;
        }
        bool IsValidFormat(const std::wstring& szFormat)
        {
            int nTools = this->Count();
            for (int i = 0; i < nTools; i++)
            {
                CTool& tool = this->Get(i);
                if (tool.IsValidFormat(szFormat) == true)
                    return true;
            }
            return false;
        }
    };
}
