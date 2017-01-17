// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>
#include <afxwin.h>
#include <afxcmn.h>

class COptions
{
public:
    int nSelectedFormat;
    CString szOutputPath;
    bool bOutputPathChecked;
    bool bLogConsoleOutput;
    bool bDeleteSourceFiles;
    bool bStayOnTop;
    bool bRecurseChecked;
    CString szMainWindowResize;
    CString szFileListColumns;
    bool bShowGridLines;
    bool bShowTrayIcon;
    bool bDoNotSaveConfiguration;
    CString szPresetsDialogResize;
    CString szPresetsListColumns;
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
    bool bDeleteOnError;
    bool bStopOnErrors;
    CString szLogFileName;
    int nLogEncoding; // 0 - ANSI, 1 - UNICODE, 2 - UTF-8
    bool bForceConsoleWindow;
public:
    COptions() { }
    virtual ~COptions() { }
public:
    void Copy(COptions &other)
    {
        other.nSelectedFormat = this->nSelectedFormat;
        other.szOutputPath = this->szOutputPath;
        other.bOutputPathChecked = this->bOutputPathChecked;
        other.bLogConsoleOutput = this->bLogConsoleOutput;
        other.bDeleteSourceFiles = this->bDeleteSourceFiles;
        other.bStayOnTop = this->bStayOnTop;
        other.bRecurseChecked = this->bRecurseChecked;
        other.szMainWindowResize = this->szMainWindowResize;
        other.szFileListColumns = this->szFileListColumns;
        other.bShowGridLines = this->bShowGridLines;
        other.bShowTrayIcon = this->bShowTrayIcon;
        other.bDoNotSaveConfiguration = this->bDoNotSaveConfiguration;
        other.szPresetsDialogResize = this->szPresetsDialogResize;
        other.szPresetsListColumns = this->szPresetsListColumns;
        other.szFormatsDialogResize = this->szFormatsDialogResize;
        other.szFormatsListColumns = this->szFormatsListColumns;
        other.bDeleteOnError = this->bDeleteOnError;
        other.bStopOnErrors = this->bStopOnErrors;
        other.szLogFileName = this->szLogFileName;
        other.nLogEncoding = this->nLogEncoding;
        other.bForceConsoleWindow = this->bForceConsoleWindow;
    }
public:
    void Defaults()
    {
        this->nSelectedFormat = 0;
        this->szOutputPath = _T("");
        this->bOutputPathChecked = false;
        this->bLogConsoleOutput = false;
        this->bDeleteSourceFiles = false;
        this->bStayOnTop = false;
        this->bRecurseChecked = true;
        this->szMainWindowResize = _T("");
        this->szFileListColumns = _T("");
        this->bShowGridLines = true;
        this->bShowTrayIcon = false;
        this->bDoNotSaveConfiguration = false;
        this->szPresetsDialogResize = _T("");
        this->szPresetsListColumns = _T("");
        this->szFormatsDialogResize = _T("");
        this->szFormatsListColumns = _T("");
        this->bDeleteOnError = true;
        this->bStopOnErrors = false;
        this->szLogFileName = MAIN_APP_LOG;
        this->nLogEncoding = 2;
        this->bForceConsoleWindow = false;
    }
};

class CPreset
{
public:
    CString szName;
    CString szOptions;
public:
    CPreset() { }
    virtual ~CPreset() { }
public:
    void Copy(CPreset &other)
    {
        other.szName = this->szName;
        other.szOptions = this->szOptions;
    }
};

class CPresetsList
{
private:
    CList<CPreset, CPreset&> m_Presets;
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
        CPreset& preset1 = this->GetData(idx1);
        CPreset& preset2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        this->SetData(preset1, idx2);
        this->SetData(preset2, idx1);
    }
};

class CFormat
{
public:
    CString szId;
    CString szName;
    CString szTemplate;
    bool bInput;
    bool bOutput;
    CString szFunction;
    CString szPath;
    int nType; // 0 - Encoder, 1 - Decoder
    CString szExtension;
    int nDefaultPreset;
    CPresetsList *m_Presets;
public:
    CFormat() { }
    virtual ~CFormat() { }
public:
    void Copy(CFormat &other)
    {
        other.szId = this->szId;
        other.szName = this->szName;
        other.szTemplate = this->szTemplate;
        other.bInput = this->bInput;
        other.bOutput = this->bOutput;
        other.szFunction = this->szFunction;
        other.szPath = this->szPath;
        other.nType = this->nType;
        other.szExtension = this->szExtension;
        other.nDefaultPreset = this->nDefaultPreset;

        int nPresets = m_Presets.GetSize();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset& preset = m_Presets->GetData(i);
            CPreset copy;
            preset.Copy(copy);
            other.m_Presets->InsertNode(copy);
        }
    }
};

class CFormatsList
{
private:
    CList<CFormat, CFormat&> m_Formats;
public:
    void SetData(CFormat& format, int idx)
    {
        m_Formats.SetAt(m_Formats.FindIndex(idx), format);
    }
    CFormat& GetData(int idx)
    {
        return m_Formats.GetAt(m_Formats.FindIndex(idx));
    }
public:
    CFormatsList()
    {
    }
    virtual ~CFormatsList()
    {
        if (m_Formats.GetCount() != 0)
            m_Formats.RemoveAll();
    }
public:
    bool IsEmpty()
    {
        return (m_Formats.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)m_Formats.GetCount();
    }
public:
    void InsertNode(CFormat &format)
    {
        m_Formats.AddTail(format);
    }
    void RemoveNode(int pstn = -1)
    {
        m_Formats.RemoveAt(m_Formats.FindIndex(pstn));
    }
    void RemoveAllNodes(void)
    {
        if (m_Formats.GetCount() != 0)
            m_Formats.RemoveAll();
    }
public:
    void Copy(CFormatsList& other)
    {
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            CFormat copy;
            format.Copy(copy);
            other.InsertNode(copy);
        }
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        CFormat& format1 = this->GetData(idx1);
        CFormat& format2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        this->SetData(format1, idx2);
        this->SetData(format2, idx1);
    }
public:
    int GetFormatById(CString szFormatId)
    {
        szFormatId.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (szFormatId.Compare(format.szId.MakeUpper()) == 0)
                return i;
        }
        return -1;
    }
public:
    int GetInFormatByExt(CString szInExt)
    {
        szInExt.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 1 && szInExt.Compare(format.szExtension.MakeUpper()) == 0)
                return i;
        }
        return -1;
    }
    int GetOutFormatByExt(CString szOutExt)
    {
        szOutExt.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 0 && szOutExt.Compare(format.szExtension.MakeUpper()) == 0)
                return i;
        }
        return -1;
    }
public:
    int GetInFormatById(CString szFormatId)
    {
        szFormatId.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 1 && szFormatId.Compare(format.szId.MakeUpper()) == 0)
                return i;
        }
        return -1;
    }
    int GetOutFormatById(CString szFormatId)
    {
        szFormatId.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 0 && szFormatId.Compare(format.szId.MakeUpper()) == 0)
                return i;
        }
        return -1;
    }
public:
    bool IsValidInExtension(CString szInExt)
    {
        szInExt.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 1 && szInExt.Compare(format.szExtension.MakeUpper()) == 0)
                return true;
        }
        return false;
    }
    bool IsValidOutExtension(CString szOutExt)
    {
        szOutExt.MakeUpper();
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 0 && szOutExt.Compare(format.szExtension.MakeUpper()) == 0)
                return true;
        }
        return false;
    }
public:
    bool IsValidInFileExtension(CString szInFilePath)
    {
        CString szInExt = ::PathFindExtension(szInFilePath);
        szInExt.MakeUpper();
        szInExt.Remove('.');
        return IsValidInExtension(szInExt);
    }
    bool IsValidOutFileExtension(CString szOutFilePath)
    {
        CString szOutExt = ::PathFindExtension(szOutFilePath);
        szOutExt.MakeUpper();
        szOutExt.Remove('.');
        return IsValidOutExtension(szOutExt);
    }
};

class CItem
{
public:
    CString szPath;
    CString szSize;
    CString szName;
    CString szExtension;
    CString szFormatId;
    int nPreset;
    bool bChecked;
    CString szTime;
    CString szStatus;
public:
    CItem() { }
    virtual ~CItem() { }
public:
    void Copy(CItem &other)
    {
        other.szPath = this->szPath;
        other.szSize = this->szSize;
        other.szName = this->szName;
        other.szExtension = this->szExtension;
        other.szFormatId = this->szFormatId;
        other.nPreset = this->nPreset;
        other.bChecked = this->bChecked;
        other.szTime = this->szTime;
        other.szStatus = this->szStatus;
    }
};

class CItemsList
{
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

class CConfiguration
{
public:
    COptions m_Options;
    CFormatsList m_Formats;
    CItemsList m_Items;
public:
    CConfiguration() { }
    virtual ~CConfiguration() { }
};
