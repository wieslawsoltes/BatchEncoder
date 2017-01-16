// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>
#include <afxwin.h>
#include <afxcmn.h>

#define NUM_INPUT_EXT 19
#define NUM_OUTPUT_EXT 14
#define NUM_FORMAT_NAMES (NUM_OUTPUT_EXT + (NUM_INPUT_EXT - 1))
#define NUM_PRESET_FILES 14

extern const TCHAR *g_szAllInExt[];
extern const TCHAR *g_szAllOutExt[];
extern const TCHAR *g_szPresetNames[];
extern const char *g_szPresetTags[];

class COptions
{
public:
    CString szSelectedPresets;
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
    void Copy(COptions &other)
    {
        other.szSelectedPresets = this->szSelectedPresets;
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
        this->szSelectedPresets = _T("");
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
    void SetData(CItem item, int idx)
    {
        m_Items.SetAt(m_Items.FindIndex(idx), item);
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
        CItem item1 = this->GetData(idx1);
        CItem item2 = this->GetData(idx2);
        if ((idx1 < 0) || (idx2 < 0) ||
            (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;

        this->SetData(item1, idx2);
        this->SetData(item2, idx1);
    }
public:
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
    int InsertNode(CString szPath, const TCHAR *szName, const ULONGLONG nSize, const int nOutFormat, const int nOutPreset)
    {
        CItem item;
        item.szPath = szPath;
        item.nSize = nSize;

        if ((szName == NULL) || (_tcslen(szName) == 0))
            item.szName = this->GetOnlyFileName(szPath);
        else
            item.szName = szName;

        item.szInExt = this->GetFileExtUpperCase(szPath);
        item.nInFormat = this->GetInFormatIndex(item.szInExt);

        item.szOutExt = this->GetOutFormatExt(nOutFormat);
        item.nOutFormat = nOutFormat;
        item.nOutPreset = nOutPreset;

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
    void SetItemFilePath(CString szFilePath, int idx)
    {
        CItem item = this->GetData(idx);
        item.szPath = szFilePath;
        this->SetData(item, idx);
    }
    CString GetItemFilePath(int idx)
    {
        return this->GetData(idx).szPath;
    }
public:
    void SetItemFileSize(ULONGLONG nFileSize, int idx)
    {
        CItem item = this->GetData(idx);
        item.nSize = nFileSize;
        this->SetData(item, idx);
    }
    ULONGLONG GetItemFileSize(int idx)
    {
        return this->GetData(idx).nSize;
    }
public:
    void SetItemFileName(CString szFileName, int idx)
    {
        CItem item = this->GetData(idx);
        item.szName = szFileName;
        this->SetData(item, idx);
    }
    CString GetItemFileName(int idx)
    {
        return this->GetData(idx).szName;
    }
public:
    void SetItemInExt(CString szInExt, int idx)
    {
        CItem item = this->GetData(idx);
        item.szInExt = szInExt;
        this->SetData(item, idx);
    }
    CString GetItemInExt(int idx)
    {
        return this->GetData(idx).szInExt;
    }
public:
    void SetItemInFormat(int nInFormat, int idx)
    {
        CItem item = this->GetData(idx);
        item.nInFormat = nInFormat;
        this->SetData(item, idx);
    }
    int GetItemInFormat(int idx)
    {
        return this->GetData(idx).nInFormat;
    }
public:
    void SetItemOutExt(CString szOutExt, int idx)
    {
        CItem item = this->GetData(idx);
        item.szOutExt = szOutExt;
        this->SetData(item, idx);
    }
    CString GetItemOutExt(int idx)
    {
        return this->GetData(idx).szOutExt;
    }
public:
    void SetItemOutFormat(int nOutFormat, int idx)
    {
        CItem item = this->GetData(idx);
        item.nOutFormat = nOutFormat;
        this->SetData(item, idx);
    }
    int GetItemOutFormat(int idx)
    {
        return this->GetData(idx).nOutFormat;
    }
public:
    void SetItemOutPreset(int nOutPreset, int idx)
    {
        CItem item = this->GetData(idx);
        item.nOutPreset = nOutPreset;
        this->SetData(item, idx);
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
    void SetData(CPreset preset, int idx)
    {
        m_Presets.SetAt(m_Presets.FindIndex(idx), preset);
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
    void SetPresetName(CString szName, int idx)
    {
        CPreset preset = this->GetData(idx);
        preset.szName = szName;
        this->SetData(preset, idx);
    }
    CString GetPresetName(int idx)
    {
        CPreset preset = this->GetData(idx);
        return preset.szName;
    }
public:
    void SetPresetOptions(CString szOptions, int idx)
    {
        CPreset preset = this->GetData(idx);
        preset.szOptions = szOptions;
        this->SetData(preset, idx);
    }
    CString GetPresetOptions(int idx)
    {
        CPreset preset = this->GetData(idx);
        return preset.szOptions;
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
