// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "Item.h"

class CItemsList
{
public:
    CList<CItem, CItem&> m_Items;
public:
    void SetData(CItem& item, int idx)
    {
        m_Items.SetAt(m_Items.FindIndex(idx), item);
    }
    CItem& GetData(int idx)
    {
        return m_Items.GetAt(m_Items.FindIndex(idx));
    }
public:
    CItemsList()
    {
    }
    CItemsList(const CItemsList &other)
    {
        Copy(other);
    }
    CItemsList& operator=(const CItemsList &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CItemsList()
    {
        if (m_Items.GetCount() != 0)
            m_Items.RemoveAll();
    }
public:
    void Copy(const CItemsList &other)
    {
        int nItems = (int)other.m_Items.GetCount();
        for (int i = 0; i < nItems; i++)
        {
            CItem item = other.m_Items.GetAt(other.m_Items.FindIndex(i));
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
    void InsertNode(CItem& item)
    {
        m_Items.AddTail(item);
    }
    void InsertBefore(CItem& item, int nIndex)
    {
        POSITION pos = m_Items.FindIndex(nIndex);
        if (pos != NULL)
            m_Items.InsertBefore(pos, item);
    }
    void InsertAfter(CItem& item, int nIndex)
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
    void Copy(CItemsList& other)
    {
        int nItems = (int)other.m_Items.GetCount();
        for (int i = 0; i < nItems; i++)
        {
            CItem item = other.m_Items.GetAt(other.m_Items.FindIndex(i));
            this->InsertNode(item);
        }
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        CItem item1 = this->GetData(idx1);
        CItem item2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        this->SetData(item1, idx2);
        this->SetData(item2, idx1);
    }
};
