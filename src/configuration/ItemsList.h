// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "Item.h"

// TODO: Remove dependencies.
#include "..\utilities\Utilities.h"
#include <afxwin.h>

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
public:
    int InsertNode(CString szPath, const TCHAR *szName, const ULONGLONG nSize, const CString szFormatId, const int nPreset)
    {
        CItem item;

        item.szPath = szPath;

        CString szBuff;
        szBuff.Format(_T("%I64d"), nSize);
        item.szSize = szBuff;

        if ((szName == NULL) || (_tcslen(szName) == 0))
            item.szName = this->GetOnlyFileName(szPath);
        else
            item.szName = szName;

        item.szExtension = this->GetFileExtUpperCase(szPath);
        item.szFormatId = szFormatId;
        item.nPreset = nPreset;

        m_Items.AddTail(item);
        return (int)m_Items.GetCount() - 1;
    }
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
        int nItems = this->GetSize();
        for (int i = 0; i < nItems; i++)
        {
            CItem& item = this->GetData(i);
            CItem copy;
            item.Copy(copy);
            other.InsertNode(copy);
        }
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        CItem& item1 = this->GetData(idx1);
        CItem& item2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        this->SetData(item1, idx2);
        this->SetData(item2, idx1);
    }
public:
    UINT MyGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
    {
        LPTSTR lpszTemp = ::PathFindFileName(lpszPathName);
        if (lpszTitle == NULL)
            return lstrlen(lpszTemp) + 1;
        lstrcpyn(lpszTitle, lpszTemp, nMax);
        return(0);
    }
    CString GetFileName(CString szFilePath)
    {
        CString szFileName;
        MyGetFileName(szFilePath, szFileName.GetBuffer(_MAX_FNAME), _MAX_FNAME);
        szFileName.ReleaseBuffer();
        return szFileName;
    }
    CString GetOnlyFileName(CString szFilePath)
    {
        CString szFileName = this->GetFileName(szFilePath);
        szFileName.TrimRight(GetFileExt(szFilePath));
        szFileName.TrimRight(_T("."));
        return szFileName;
    }
public:
    CString GetFileExt(CString szFilePath)
    {
        CString szExt = ::PathFindExtension(szFilePath);
        szExt.Remove('.');
        return szExt;
    }
    CString GetFileExtUpperCase(CString szFilePath)
    {
        CString szExt = ::PathFindExtension(szFilePath);
        szExt.MakeUpper();
        szExt.Remove('.');
        return szExt;
    }
    CString GetFileExtLowerCase(CString szFilePath)
    {
        CString szExt = ::PathFindExtension(szFilePath);
        szExt.MakeLower();
        szExt.Remove('.');
        return szExt;
    }
};
