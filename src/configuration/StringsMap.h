// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>

class CStringsMap
{
public:
    CMap<int, int, CString, CString&> m_Map;
public:
    void SetData(int nKey, CString szValue)
    {
        m_Map[nKey] = szValue;
    }
    CString GetData(int nKey)
    {
        return m_Map[nKey];
    }
public:
    CStringsMap()
    {
    }
    CStringsMap(const CStringsMap &other)
    {
        Copy(other);
    }
    CStringsMap& operator=(const CStringsMap &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CStringsMap()
    {
        if (m_Map.GetCount() != 0)
            m_Map.RemoveAll();
    }
public:
    void Copy(const CStringsMap &other)
    {
        this->RemoveAllNodes();
        POSITION pos = other.m_Map.GetStartPosition();
        int nKey;
        while (pos != NULL)
        {
            CString rValue;
            other.m_Map.GetNextAssoc(pos, nKey, rValue);
            this->InsertNode(nKey, rValue);
        }
    }
public:
    bool IsEmpty()
    {
        return (m_Map.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)m_Map.GetCount();
    }
public:
    void InsertNode(int nKey, CString szValue)
    {
        m_Map[nKey] = szValue;
    }
public:
    void RemoveNode(int nKey)
    {
        m_Map.RemoveKey(nKey);
    }
    void RemoveAllNodes(void)
    {
        if (m_Map.GetCount() != 0)
            m_Map.RemoveAll();
    }
public:
    void Copy(CStringsMap& other)
    {
        POSITION pos = this->m_Map.GetStartPosition();
        int nKey;
        while (pos != NULL)
        {
            CString rValue;
            this->m_Map.GetNextAssoc(pos, nKey, rValue);
            other.InsertNode(nKey, rValue);
        }
    }
public:
    void SwapItems(int key1, int key2)
    {
        CString value1;
        CString value2;
        BOOL haveValue1 = this->m_Map.Lookup(key1, value1);
        BOOL haveValue2 = this->m_Map.Lookup(key2, value2);
        if (haveValue1 == FALSE || haveValue2 == FALSE)
            return;
        this->SetData(key2, value1);
        this->SetData(key1, value2);
    }
};
