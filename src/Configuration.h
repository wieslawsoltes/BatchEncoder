// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>
#include <afxwin.h>
#include <afxcmn.h>

#define NUM_PROGRAM_OPTIONS 21
#define NUM_INPUT_EXT 19
#define NUM_OUTPUT_EXT 14
#define NUM_FORMAT_NAMES (NUM_OUTPUT_EXT + (NUM_INPUT_EXT - 1))
#define NUM_PRESET_FILES 14

extern const char *g_szOptionsTags[];
extern const TCHAR *g_szAllInExt[];
extern const TCHAR *g_szAllOutExt[];
extern const TCHAR *g_szPresetNames[];
extern const char *g_szPresetTags[];
extern const TCHAR *g_szPresetFiles[];

class COptions
{
public:
    bool bDeleteOnError;
    bool bStopOnErrors;
    CString szLogFileName;
    int nLogEncoding; // 0 - ANSI, 1 - UNICODE, 2 - UTF-8
public:
    void Copy(COptions &other)
    {
        other.bDeleteOnError = this->bDeleteOnError;
        other.bStopOnErrors = this->bStopOnErrors;
        other.szLogFileName = this->szLogFileName;
        other.nLogEncoding = this->nLogEncoding;
    }
};

class CPreset
{
public:
    CString szName;
    CString szOptions;
public:
    void Copy(CPreset &other)
    {
        other.szName = this->szName;
        other.szOptions = this->szOptions;
    }
};

class CFormat
{
public:
    CString szName;
    CString szTemplate;
    CString szPath;
    bool bInput;
    bool bOutput;
    CString szFunction;
public:
    void Copy(CFormat &other)
    {
        other.szName = this->szName;
        other.szTemplate = this->szTemplate;
        other.szPath = this->szPath;
        other.bInput = this->bInput;
        other.bOutput = this->bOutput;
        other.szFunction = this->szFunction;
    }
};

class CItem
{
public:
    // File
    CString szPath;
    ULONGLONG nSize;
    // Input
    CString szName;
    CString szInExt;
    int nInFormat;
    // Output
    CString szOutExt;
    int nOutFormat;
    int nOutPreset;
    // Status
    bool bChecked;
    CString szTime;
    CString szStatus;
public:
    void Copy(CItem &other)
    {
        other.szPath = this->szPath;
        other.nSize = this->nSize;
        other.szName = this->szName;
        other.szInExt = this->szInExt;
        other.nInFormat = this->nInFormat;
        other.szOutExt = this->szOutExt;
        other.nOutFormat = this->nOutFormat;
        other.nOutPreset = this->nOutPreset;
        other.bChecked = this->bChecked;
        other.szTime = this->szTime;
        other.szStatus = this->szStatus;
    }
};

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
    int InsertNode(CString szPath,
        const TCHAR *szName,
        const ULONGLONG nSize,
        const int nOutFormat,
        const int nOutPreset)
    {
        CItem tmp;

        tmp.szPath = szPath;
        tmp.nSize = nSize;

        if ((szName == NULL) || (_tcslen(szName) == 0))
            tmp.szName = this->GetOnlyFileName(szPath);
        else
            tmp.szName = szName;

        tmp.szInExt = this->GetFileExtUpperCase(szPath);
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
        tmpList.szPath = szFilePath;
        this->SetData(tmpList, idx);
    }
    CString GetItemFilePath(int idx)
    {
        return this->GetData(idx).szPath;
    }
public:
    void SetItemFileSize(ULONGLONG nFileSize, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.nSize = nFileSize;
        this->SetData(tmpList, idx);
    }
    ULONGLONG GetItemFileSize(int idx)
    {
        return this->GetData(idx).nSize;
    }
public:
    void SetItemFileName(CString szFileName, int idx)
    {
        CItem tmpList = this->GetData(idx);
        tmpList.szName = szFileName;
        this->SetData(tmpList, idx);
    }
    CString GetItemFileName(int idx)
    {
        return this->GetData(idx).szName;
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

class CConfiguration
{
public:
    COptions m_Options;
    CFormat m_Formats[NUM_FORMAT_NAMES];
    CPresetsList m_Presets[NUM_PRESET_FILES];
    CString szPresetsFile[NUM_PRESET_FILES];
    CItemsList m_Items;
};
