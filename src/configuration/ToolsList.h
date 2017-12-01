// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include "collections\ListT.h"
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
