// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "Preset.h"

class CPresetsList
{
public:
    CList<CPreset, const CPreset&> m_Presets;
public:
    void SetData(CPreset& preset, int idx)
    {
        m_Presets.SetAt(m_Presets.FindIndex(idx), preset);
    }
    CPreset& GetData(int idx)
    {
        return m_Presets.GetAt(m_Presets.FindIndex(idx));
    }
public:
    CPresetsList()
    {
    }
    CPresetsList(const CPresetsList &other)
    {
        Copy(other);
    }
    CPresetsList& operator=(const CPresetsList &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CPresetsList()
    {
        if (m_Presets.GetCount() != 0)
            m_Presets.RemoveAll();
    }
public:
    void Copy(const CPresetsList &other)
    {
        int nPresets = (int)other.m_Presets.GetCount();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset preset = other.m_Presets.GetAt(other.m_Presets.FindIndex(i));
            this->InsertNode(preset);
        }
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
    void InsertNode(CPreset &preset)
    {
        m_Presets.AddTail(preset);
    }
    void InsertNode(CString szName)
    {
        CPreset preset;
        preset.szName = szName;
        m_Presets.AddTail(preset);
    }
public:
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
    void Copy(CPresetsList& other)
    {
        int nPresets = this->GetSize();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset& preset = this->GetData(i);
            CPreset copy;
            preset.Copy(copy);
            other.InsertNode(copy);
        }
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        CPreset preset1 = this->GetData(idx1);
        CPreset preset2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        this->SetData(preset1, idx2);
        this->SetData(preset2, idx1);
    }
};
