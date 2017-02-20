// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

template <class K, class V>
class CMapT
{
public:
    CMap<K, K, V, V&> m_Map;
public:
    CMapT()
    {
    }
    CMapT(const CMapT &other)
    {
        Copy(other);
    }
    CMapT& operator=(const CMapT &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CMapT()
    {
        if (m_Map.GetCount() != 0)
            m_Map.RemoveAll();
    }
public:
    void Copy(const CMapT &other)
    {
        this->RemoveAll();
        POSITION pos = other.m_Map.GetStartPosition();
        int nKey;
        while (pos != NULL)
        {
            CString rValue;
            other.m_Map.GetNextAssoc(pos, nKey, rValue);
            this->Insert(nKey, rValue);
        }
    }
    void Copy(CMapT& other)
    {
        POSITION pos = this->m_Map.GetStartPosition();
        int nKey;
        while (pos != NULL)
        {
            CString rValue;
            this->m_Map.GetNextAssoc(pos, nKey, rValue);
            other.Insert(nKey, rValue);
        }
    }
    bool IsEmpty()
    {
        return (m_Map.GetCount() == 0) ? true : false;
    }
    int Count()
    {
        return (int)m_Map.GetCount();
    }
    void Set(K key, CString szValue)
    {
        m_Map[nKey] = szValue;
    }
    CString Get(K key)
    {
        return m_Map[key];
    }
    bool TryGet(K key, V& rValue)
    {
        if (m_Map.Lookup(key, rValue) == TRUE)
            return true;
        return false;
    }
    void Insert(K key, CString szValue)
    {
        m_Map[key] = szValue;
    }
    void Remove(K key)
    {
        m_Map.RemoveKey(key);
    }
    void RemoveAll(void)
    {
        if (m_Map.GetCount() != 0)
            m_Map.RemoveAll();
    }
    void Swap(K key1, K key2)
    {
        V value1;
        V value2;
        BOOL haveValue1 = this->m_Map.Lookup(key1, value1);
        BOOL haveValue2 = this->m_Map.Lookup(key2, value2);
        if (haveValue1 == FALSE || haveValue2 == FALSE)
            return;
        this->Set(key2, value1);
        this->Set(key1, value2);
    }
};
