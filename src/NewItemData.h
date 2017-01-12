// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

// nAction:
// 0 - Add new item to memory ItemsList.
// 1 - Add new item to control ItemsList.
// 2 - Add new item to memory and control ItemsList.
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
