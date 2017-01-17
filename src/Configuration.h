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
    COptions() 
    {
    }
    COptions(const COptions &other)
    {
        Copy(other);
    }
    COptions& operator=(const COptions &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~COptions() 
    {
    }
public:
    void Copy(const COptions &other)
    {
        this->nSelectedFormat = other.nSelectedFormat;
        this->szOutputPath = other.szOutputPath;
        this->bOutputPathChecked = other.bOutputPathChecked;
        this->bLogConsoleOutput = other.bLogConsoleOutput;
        this->bDeleteSourceFiles = other.bDeleteSourceFiles;
        this->bStayOnTop = other.bStayOnTop;
        this->bRecurseChecked = other.bRecurseChecked;
        this->szMainWindowResize = other.szMainWindowResize;
        this->szFileListColumns = other.szFileListColumns;
        this->bShowGridLines = other.bShowGridLines;
        this->bShowTrayIcon = other.bShowTrayIcon;
        this->bDoNotSaveConfiguration = other.bDoNotSaveConfiguration;
        this->szPresetsDialogResize = other.szPresetsDialogResize;
        this->szPresetsListColumns = other.szPresetsListColumns;
        this->szFormatsDialogResize = other.szFormatsDialogResize;
        this->szFormatsListColumns = other.szFormatsListColumns;
        this->bDeleteOnError = other.bDeleteOnError;
        this->bStopOnErrors = other.bStopOnErrors;
        this->szLogFileName = other.szLogFileName;
        this->nLogEncoding = other.nLogEncoding;
        this->bForceConsoleWindow = other.bForceConsoleWindow;
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
    CPreset() 
    {
    }
    CPreset(const CPreset &other)
    {
        Copy(other);
    }
    CPreset& operator=(const CPreset &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CPreset() 
    {
    }
public:
    void Copy(const CPreset &other)
    {
        this->szName = other.szName;
        this->szOptions = other.szOptions;
    }
};

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
    CPresetsList m_Presets;
public:
    CFormat() 
    {
    }
    CFormat(const CFormat &other)
    {
        Copy(other);
    }
    CFormat& operator=(const CFormat &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CFormat() 
    {
    }
public:
    void Copy(const CFormat &other)
    {
        this->szId = other.szId;
        this->szName = other.szName;
        this->szTemplate = other.szTemplate;
        this->bInput = other.bInput;
        this->bOutput = other.bOutput;
        this->szFunction = other.szFunction;
        this->szPath = other.szPath;
        this->nType = other.nType;
        this->szExtension = other.szExtension;
        this->nDefaultPreset = other.nDefaultPreset;
        this->m_Presets = other.m_Presets;
    }
};

class CFormatsList
{
public:
    CList<CFormat, const CFormat&> m_Formats;
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
    CFormatsList(const CFormatsList &other)
    {
        Copy(other);
    }
    CFormatsList& operator=(const CFormatsList &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CFormatsList() 
    {
        if (m_Formats.GetCount() != 0)
            m_Formats.RemoveAll();
    }
public:
    void Copy(const CFormatsList &other)
    {
        int nFormats = (int)other.m_Formats.GetCount();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat format = other.m_Formats.GetAt(other.m_Formats.FindIndex(i));
            this->InsertNode(format);
        }
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
    CItem() 
    {
    }
    CItem(const CItem &other)
    {
        Copy(other);
    }
    CItem& operator=(const CItem &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CItem() 
    {
    }
public:
    void Copy(const CItem &other)
    {
        this->szPath = other.szPath;
        this->szSize = other.szSize;
        this->szName = other.szName;
        this->szExtension = other.szExtension;
        this->szFormatId = other.szFormatId;
        this->nPreset = other.nPreset;
        this->bChecked = other.bChecked;
        this->szTime = other.szTime;
        this->szStatus = other.szStatus;
    }
};

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

class CConfiguration
{
public:
    COptions m_Options;
    CFormatsList m_Formats;
    CItemsList m_Items;
public:
    CConfiguration() 
    {
    }
    virtual ~CConfiguration() 
    { 
    }
};
