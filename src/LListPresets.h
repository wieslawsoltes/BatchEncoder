// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

class CLListPresets
{
private:
    CList<CPreset, CPreset&> myList;
private:
    void SetData(CPreset listData, int idx)
    {
        myList.SetAt(myList.FindIndex(idx), listData);
    }
    CPreset GetData(int idx)
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
    void InsertNode(CString szName)
    {
        CPreset tmp;
        tmp.szName = szName;
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
    void SetPresetName(CString szName, int idx)
    {
        CPreset tmpList;
        tmpList = this->GetData(idx);
        tmpList.szName = szName;
        this->SetData(tmpList, idx);
    }
    CString GetPresetName(int idx)
    {
        CPreset tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szName;
    }
public:
    void SetPresetOptions(CString szOptions, int idx)
    {
        CPreset tmpList;
        tmpList = this->GetData(idx);
        tmpList.szOptions = szOptions;
        this->SetData(tmpList, idx);
    }
    CString GetPresetOptions(int idx)
    {
        CPreset tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szOptions;
    }
};
