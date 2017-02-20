// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

template <class T>
class CListT
{
public:
    CList<T, T&> m_Items;
public:
    void SetData(T& item, int idx)
    {
        m_Items.SetAt(m_Items.FindIndex(idx), item);
    }
    T& GetData(int idx)
    {
        return m_Items.GetAt(m_Items.FindIndex(idx));
    }
public:
    CListT()
    {
    }
    CListT(const CListT &other)
    {
        Copy(other);
    }
    CListT& operator=(const CListT &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CListT()
    {
        if (m_Items.GetCount() != 0)
            m_Items.RemoveAll();
    }
public:
    void Copy(const CListT &other)
    {
        int nItems = (int)other.m_Items.GetCount();
        for (int i = 0; i < nItems; i++)
        {
            T item = other.m_Items.GetAt(other.m_Items.FindIndex(i));
            this->InsertNode(item);
        }
    }
public:
    bool IsEmpty()
    {
        return (m_Items.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)m_Items.GetCount();
    }
public:
    void InsertNode(T& item)
    {
        m_Items.AddTail(item);
    }
    void InsertBefore(T& item, int nIndex)
    {
        POSITION pos = m_Items.FindIndex(nIndex);
        if (pos != NULL)
            m_Items.InsertBefore(pos, item);
    }
    void InsertAfter(T& item, int nIndex)
    {
        POSITION pos = m_Items.FindIndex(nIndex);
        if (pos != NULL)
            m_Items.InsertAfter(pos, item);
    }
public:
    void RemoveNode(int pstn = -1)
    {
        m_Items.RemoveAt(m_Items.FindIndex(pstn));
    }
    void RemoveAllNodes(void)
    {
        if (m_Items.GetCount() != 0)
            m_Items.RemoveAll();
    }
public:
    void Copy(CListT& other)
    {
        int nItems = (int)other.m_Items.GetCount();
        for (int i = 0; i < nItems; i++)
        {
            T item = other.m_Items.GetAt(other.m_Items.FindIndex(i));
            this->InsertNode(item);
        }
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        T item1 = this->GetData(idx1);
        T item2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        this->SetData(item1, idx2);
        this->SetData(item2, idx1);
    }
};
