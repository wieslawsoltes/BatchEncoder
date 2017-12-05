// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include "utilities\ListT.h"
#include "Tool.h"

class CToolsList : public CListT<CTool>
{
public:
    int GetToolByFormat(CString szFormat)
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
    int GetToolByFormatAndPlatform(CString szFormat, CString szPlatform)
    {
        int nTools = this->Count();
        for (int i = 0; i < nTools; i++)
        {
            CTool& tool = this->Get(i);
            if ((tool.IsValidFormat(szFormat) == true) && (tool.szPlatform.CompareNoCase(szPlatform) == 0))
            {
                return i;
            }
        }
        return -1;
    }
    bool IsValidFormat(CString szFormat)
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
