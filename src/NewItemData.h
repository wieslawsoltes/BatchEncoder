// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

// nAction
#define ADD_ITEM_NONE              -1
#define ADD_ITEM_MEMORY             0
#define ADD_ITEM_CONTROL            1
#define ADD_ITEM_MEMORY_AND_CONTROL 2

typedef struct TNewItemData
{
    int nAction;
    CString szFileName;
    int nItem;
    CString szName;
    CString szOutExt;
    int nPreset;
    BOOL bCheck;
    CString szTime;
    CString szStatus;
} NewItemData, *PNewItemData;

void InitNewItemData(NewItemData &nid);
