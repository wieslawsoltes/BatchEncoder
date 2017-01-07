//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#pragma once

#include <afxtempl.h>

#include "Config.h"

class CLListFiles 
{
private:
    typedef struct _EDIT_DATA_FILES_
    {
        CString szFilePath;
        ULONGLONG nFileSize;
        CString szFileName;
        CString szInExt;
        int nInFormat;
        CString szOutExt;
        int nOutFormat;
        int nOutPreset;
    } LIST_DATA_FILES, *PLIST_DATA_FILES;
private:
    CList<LIST_DATA_FILES,LIST_DATA_FILES&> myList;
private:
    void SetData(LIST_DATA_FILES listData, int idx)
    {
        myList.SetAt(myList.FindIndex(idx), listData);
    }
    LIST_DATA_FILES GetData(int idx)
    {
        return myList.GetAt(myList.FindIndex(idx));
    }
private:
    UINT MyGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
    {
        LPTSTR lpszTemp = ::PathFindFileName(lpszPathName);
        if(lpszTitle == NULL)
            return lstrlen(lpszTemp) + 1;

        lstrcpyn(lpszTitle, lpszTemp, nMax);
        return(0);
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        LIST_DATA_FILES tmpList1 = this->GetData(idx1);
        LIST_DATA_FILES tmpList2 = this->GetData(idx2);

        if((idx1 < 0) || (idx2 < 0) || 
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
    CLListFiles()
    {

    }
    virtual ~CLListFiles()
    {
        if(myList.GetCount() != 0)
            myList.RemoveAll();
    }
public:
    bool IsEmpty()
    {
        return (myList.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int) myList.GetCount();	
    }
public:
    static int GetInFormatIndex(CString szInExt)
    {
        szInExt.MakeUpper();
        for(int i = 0; i < NUM_INPUT_EXT; i++)
        {
            if(szInExt.Compare(g_szAllInExt[i]) == 0)
                return i;
        }
        return -1;
    }
    static int GetOutFormatIndex(CString szOutExt)
    {
        szOutExt.MakeUpper();
        for(int i = 0; i < NUM_OUTPUT_EXT; i++)
        {
            if(szOutExt.Compare(g_szAllOutExt[i]) == 0)
                return i;
        }
        return -1;
    }
public:
    static CString GetInFormatExt(int nInFormat)
    {
        if((nInFormat >= 0) && (nInFormat < NUM_INPUT_EXT))
            return g_szAllInExt[nInFormat];
        return NULL;
    }
    static CString GetOutFormatExt(int nOutFormat)
    {
        if((nOutFormat >= 0) && (nOutFormat < NUM_OUTPUT_EXT))
            return g_szAllOutExt[nOutFormat];
        return NULL;
    }
public:
    static bool IsValidInExtension(CString szInExt)
    {
        for(int i = 0; i < NUM_INPUT_EXT; i++)
        {
            if(szInExt.Compare(g_szAllInExt[i]) == 0)
                return true;
        }
        return false;
    }
    static bool IsValidOutExtension(CString szOutExt)
    {
        for(int i = 0; i < NUM_OUTPUT_EXT; i++)
        {
            if(szOutExt.Compare(g_szAllOutExt[i]) == 0)
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
        LIST_DATA_FILES tmp;

        tmp.szFilePath = szFilePath;
        tmp.nFileSize = nFileSize;

        if((szFileName == NULL) || (_tcslen(szFileName) == 0))
            tmp.szFileName = this->GetOnlyFileName(szFilePath);
        else
            tmp.szFileName = szFileName;

        tmp.szInExt = this->GetFileExtUpperCase(szFilePath);
        tmp.nInFormat = this->GetInFormatIndex(tmp.szInExt);
        tmp.szOutExt = this->GetOutFormatExt(nOutFormat);

        tmp.nOutFormat = nOutFormat;
        tmp.nOutPreset = nOutPreset;

        myList.AddTail(tmp);

        return (int) myList.GetCount() - 1;
    }
    void RemoveNode(int pstn = -1)
    {
        myList.RemoveAt(myList.FindIndex(pstn));
    }
    void RemoveAllNodes(void)
    {
        if(myList.GetCount() != 0)
            myList.RemoveAll();
    }
public:
    void SetItemFilePath(CString szFilePath, int idx)
    {
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
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
        LIST_DATA_FILES tmpList = this->GetData(idx);
        tmpList.nOutPreset = nOutPreset;
        this->SetData(tmpList, idx);
    }
    int GetItemOutPreset(int idx)
    {
        return this->GetData(idx).nOutPreset;
    }
};
