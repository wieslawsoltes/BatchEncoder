// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

class CPresetsList
{
private:
    CList<CPreset, CPreset&> m_Presets;
private:
    void SetData(CPreset listData, int idx)
    {
        m_Presets.SetAt(m_Presets.FindIndex(idx), listData);
    }
    CPreset GetData(int idx)
    {
        return m_Presets.GetAt(m_Presets.FindIndex(idx));
    }
public:
    CPresetsList()
    {

    }
    virtual ~CPresetsList()
    {
        if (m_Presets.GetCount() != 0)
            m_Presets.RemoveAll();
    }
public:
    bool IsEmpty()
    {
        return (m_Presets.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)m_Presets.GetCount();
    }
public:
    void InsertNode(CString szName)
    {
        CPreset tmp;
        tmp.szName = szName;
        m_Presets.AddTail(tmp);
    }
    void RemoveNode(int pstn = -1)
    {
        m_Presets.RemoveAt(m_Presets.FindIndex(pstn));
    }

    void RemoveAllNodes(void)
    {
        if (m_Presets.GetCount() != 0)
            m_Presets.RemoveAll();
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
