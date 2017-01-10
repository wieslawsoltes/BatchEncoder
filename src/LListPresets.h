// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

class CLListPresets
{
private:
    typedef struct _EDIT_DATA_PRESET_
    {
        CString szPresetName;
        CString szPresetOptions;
    } LIST_DATA_PRESET, *PLIST_DATA_PRESET;
private:
    CList<LIST_DATA_PRESET, LIST_DATA_PRESET&> myList;
private:
    void SetData(LIST_DATA_PRESET listData, int idx)
    {
        myList.SetAt(myList.FindIndex(idx), listData);
    }

    LIST_DATA_PRESET GetData(int idx)
    {
        return myList.GetAt(myList.FindIndex(idx));
    }
public:
    CLListPresets()
    {

    }
    virtual ~CLListPresets()
    {
        if (myList.GetCount() != 0)
            myList.RemoveAll();
    }
public:
    bool IsEmpty()
    {
        return (myList.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)myList.GetCount();
    }
public:
    void InsertNode(CString szPresetName)
    {
        LIST_DATA_PRESET tmp;
        tmp.szPresetName = szPresetName;
        myList.AddTail(tmp);
    }
    void RemoveNode(int pstn = -1)
    {
        myList.RemoveAt(myList.FindIndex(pstn));
    }

    void RemoveAllNodes(void)
    {
        if (myList.GetCount() != 0)
            myList.RemoveAll();
    }
public:
    void SetPresetName(CString szPresetName, int idx)
    {
        LIST_DATA_PRESET tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetName = szPresetName;
        this->SetData(tmpList, idx);
    }
    CString GetPresetName(int idx)
    {
        LIST_DATA_PRESET tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szPresetName;
    }
public:
    void SetPresetOptions(CString szPresetOptions, int idx)
    {
        LIST_DATA_PRESET tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetOptions = szPresetOptions;
        this->SetData(tmpList, idx);
    }
    CString GetPresetOptions(int idx)
    {
        LIST_DATA_PRESET tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szPresetOptions;
    }
};
