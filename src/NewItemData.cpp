// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "NewItemData.h"

void InitNewItemData(NewItemData &nid)
{
    nid.nAction = ADD_ITEM_NONE;
    nid.szFileName = _T("");
    nid.nItem = -1;
    nid.szName = _T("");
    nid.szOutExt = _T("");
    nid.nPreset = -1;
    nid.bCheck = TRUE;
    nid.szTime = _T("");
    nid.szStatus = _T("");
}
