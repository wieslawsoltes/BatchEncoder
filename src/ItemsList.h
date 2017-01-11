// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

class CItemsList
{
    CList<CItem, CItem&> m_Items;
private:
    void SetData(CItem listData, int idx)
    {
        m_Items.SetAt(m_Items.FindIndex(idx), listData);
    }
    CItem GetData(int idx)
    {
        return m_Items.GetAt(m_Items.FindIndex(idx));
    }
private:
    UINT MyGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
    {
        LPTSTR lpszTemp = ::PathFindFileName(lpszPathName);
        if (lpszTitle == NULL)
            return lstrlen(lpszTemp) + 1;

        lstrcpyn(lpszTitle, lpszTemp, nMax);
        return(0);
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        CItem tmpList1 = this->GetData(idx1);
        CItem tmpList2 = this->GetData(idx2);

        if ((idx1 < 0) || (idx2 < 0) ||
            (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;

        this->SetData(tmpList1, idx2);
        this->SetData(tmpList2, idx1);
    }
public:
    CString GetFileName(CString szFilePath)
    {
        CString strResult;
        MyGetFileName(szFilePath, strResult.GetBuffer(_MAX_FNAME), _MAX_FNAME);
        strResult.ReleaseBuffer();
        return strResult;
    }
    CString GetOnlyFileName(CString szFilePath)
    {
        CString strResult;

        strResult = this->GetFileName(szFilePath);
        strResult.TrimRight(GetFileExt(szFilePath));
        strResult.TrimRight(_T("."));

        return strResult;
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
public:
    CItemsList()
    {

    }
    virtual ~CItemsList()
    {
        if (m_Items.GetCount() != 0)
            m_Items.RemoveAll();
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
    static int GetInFormatIndex(CString szInExt)
    {
        szInExt.MakeUpper();
        for (int i = 0; i < NUM_INPUT_EXT; i++)
        {
            if (szInExt.Compare(g_szAllInExt[i]) == 0)
                return i;
        }
        return -1;
    }
    static int GetOutFormatIndex(CString szOutExt)
    {
        szOutExt.MakeUpper();
        for (int i = 0; i < NUM_OUTPUT_EXT; i++)
        {
            if (szOutExt.Compare(g_szAllOutExt[i]) == 0)
                return i;
        }
        return -1;
    }
public:
    static CString GetInFormatExt(int nInFormat)
    {
        if ((nInFormat >= 0) && (nInFormat < NUM_INPUT_EXT))
            return g_szAllInExt[nInFormat];
        return NULL;
    }
    static CString GetOutFormatExt(int nOutFormat)
    {
        if ((nOutFormat >= 0) && (nOutFormat < NUM_OUTPUT_EXT))
            return g_szAllOutExt[nOutFormat];
        return NULL;
    }
public:
    static bool IsValidInExtension(CString szInExt)
    {
        for (int i = 0; i < NUM_INPUT_EXT; i++)
        {
            if (szInExt.Compare(g_szAllInExt[i]) == 0)
                return true;
        }
        return false;
    }
    static bool IsValidOutExtension(CString szOutExt)
    {
        for (int i = 0; i < NUM_OUTPUT_EXT; i++)
        {
            if (szOutExt.Compare(g_szAllOutExt[i]) == 0)
                return true;
        }
        return false;
    }
public:
    static bool IsValidInFileExtension(CString szInFilePath)
    {
        CString szInExt = ::PathFindExtension(szInFilePath);
        szInExt.MakeUpper();
        szInExt.Remove('.');
        return IsValidInExtension(szInExt);
    }
    static bool IsValidOutFileExtension(CString szOutFilePath)
    {
        CString szOutExt = ::PathFindExtension(szOutFilePath);
        szOutExt.MakeUpper();
        szOutExt.Remove('.');
        return IsValidOutExtension(szOutExt);
    }
public:
    int InsertNode(CString szFilePath,
        const TCHAR *szFileName,
        const ULONGLONG nFileSize,
        const int nOutFormat,
        const int nOutPreset)
    {
        CItem tmp;

        tmp.szFilePath = szFilePath;
        tmp.nFileSize = nFileSize;

        if ((szFileName == NULL) || (_tcslen(szFileName) == 0))
            tmp.szFileName = this->GetOnlyFileName(szFilePath);
        else
            tmp.szFileName = szFileName;

        tmp.szInExt = this->GetFileExtUpperCase(szFilePath);
        tmp.nInFormat = this->GetInFormatIndex(tmp.szInExt);
        tmp.szOutExt = this->GetOutFormatExt(nOutFormat);

        tmp.nOutFormat = nOutFormat;
        tmp.nOutPreset = nOutPreset;

        m_Items.AddTail(tmp);

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
    void SetItemFilePath(CString szFilePath, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.szFilePath = szFilePath;
        this->SetData(tmpList, idx);
    }
    CString GetItemFilePath(int idx)
    {
        return this->GetData(idx).szFilePath;
    }
public:
    void SetItemFileSize(ULONGLONG nFileSize, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.nFileSize = nFileSize;
        this->SetData(tmpList, idx);
    }
    ULONGLONG GetItemFileSize(int idx)
    {
        return this->GetData(idx).nFileSize;
    }
public:
    void SetItemFileName(CString szFileName, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.szFileName = szFileName;
        this->SetData(tmpList, idx);
    }
    CString GetItemFileName(int idx)
    {
        return this->GetData(idx).szFileName;
    }
public:
    void SetItemInExt(CString szInExt, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.szInExt = szInExt;
        this->SetData(tmpList, idx);
    }
    CString GetItemInExt(int idx)
    {
        return this->GetData(idx).szInExt;
    }
public:
    void SetItemInFormat(int nInFormat, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.nInFormat = nInFormat;
        this->SetData(tmpList, idx);
    }
    int GetItemInFormat(int idx)
    {
        return this->GetData(idx).nInFormat;
    }
public:
    void SetItemOutExt(CString szOutExt, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.szOutExt = szOutExt;
        this->SetData(tmpList, idx);
    }
    CString GetItemOutExt(int idx)
    {
        return this->GetData(idx).szOutExt;
    }
public:
    void SetItemOutFormat(int nOutFormat, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.nOutFormat = nOutFormat;
        this->SetData(tmpList, idx);
    }
    int GetItemOutFormat(int idx)
    {
        return this->GetData(idx).nOutFormat;
    }
public:
    void SetItemOutPreset(int nOutPreset, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.nOutPreset = nOutPreset;
        this->SetData(tmpList, idx);
    }
    int GetItemOutPreset(int idx)
    {
        return this->GetData(idx).nOutPreset;
    }
};
