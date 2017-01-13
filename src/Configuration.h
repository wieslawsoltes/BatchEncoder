// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>
#include <afxwin.h>
#include <afxcmn.h>

// number of supported input file extension
#define NUM_INPUT_EXT 19

// number of supported output file extension
#define NUM_OUTPUT_EXT 14

// number of supported format tool names
#define NUM_FORMAT_NAMES (NUM_OUTPUT_EXT + (NUM_INPUT_EXT - 1))

// encoder/decoder tool ID (based of g_szDefaultPath)
enum TOOL_ID
{
    // ENCODERS
    TOOL_ID_ENC_SSRC,
    TOOL_ID_ENC_LAME,
    TOOL_ID_ENC_MPPENC,
    TOOL_ID_ENC_FAAC_01,
    TOOL_ID_ENC_FAAC_02,
    TOOL_ID_ENC_FAAC_03,
    TOOL_ID_ENC_OGGENC2,
    TOOL_ID_ENC_AFTEN,
    TOOL_ID_ENC_FLAC,
    TOOL_ID_ENC_WAVPACK,
    TOOL_ID_ENC_MAC,
    TOOL_ID_ENC_OFR,
    TOOL_ID_ENC_TTAENC,
    TOOL_ID_ENC_TAKC,
    // DECODERS
    TOOL_ID_DEC_LAME_01,
    TOOL_ID_DEC_LAME_02,
    TOOL_ID_DEC_LAME_03,
    TOOL_ID_DEC_MPPDEC_01,
    TOOL_ID_DEC_MPPDEC_02,
    TOOL_ID_DEC_MPPDEC_03,
    TOOL_ID_DEC_FAAD_01,
    TOOL_ID_DEC_FAAD_02,
    TOOL_ID_DEC_FAAD_03,
    TOOL_ID_DEC_OGGDEC,
    TOOL_ID_DEC_AFTEN,
    TOOL_ID_DEC_FLAC,
    TOOL_ID_DEC_WVUNPACK,
    TOOL_ID_DEC_MAC_01,
    TOOL_ID_DEC_MAC_02,
    TOOL_ID_DEC_OFR,
    TOOL_ID_DEC_TTAENC,
    TOOL_ID_DEC_TAKC
};

// number of command-line tools:
// 00 - SSRC
// 01 - LAME
// 02 - MPPENC
// 03 - MPPDEC
// 04 - FAAC
// 05 - FAAD
// 06 - OGGENC
// 07 - OGGDEC
// 08 - AFTEN
// 08 - FLAC
// 09 - WAVPACK
// 10 - WVUNPACK
// 11 - MAC
// 12 - OFR
// 13 - TTAENC
// 14 - TAK
#define NUM_COMMADLINE_TOOLS 15

// number of presets files:
// 00 - WAV (Standard)
// 01 - MP3 (MPEG Layer-3)
// 02 - MPC (Musepack)
// 03 - AAC (MPEG-2/AAC)
// 04 - M4A (MPEG-4/AAC)
// 05 - MP4 (MPEG-4/AAC)
// 06 - OGG (Ogg Vorbis)
// 07 - AC3 (ATSC A/52)
// 08 - FLAC (Free Lossless Audio Codec)
// 09 - WV (WavPack)
// 10 - APE (Monkey's Audio)
// 11 - OFR (OptimFROG Lossless Audio)
// 12 - TTA (TTA1 Lossless Audio)
// 13 - TAK (Tom's Lossless Audio Kompressor)
#define NUM_PRESET_FILES 14

// number of file tag attributes
#define NUM_ITEM_ATTRIBUTES 9

// number of tags used to store program settings
#define NUM_PROGRAM_SETTINGS 23

// filter for input audio files
#define AUDIO_FILES_FILTER \
    _T("Supported Audio Files|") \
    _T("*.wav;*.mp1;*.mp2;*.mp3;*.mpc;*.mpp;*.mp+;*.mp4;*.m4a;*.aac;*.ogg;*.ac3;*.flac;*.wv;*.mac;*.ape;*.ofr;*.tta;*.tak|") \
    _T("WAV (*.wav)|*.wav|") \
    _T("MP1 (*.mp1)|*.mp1|") \
    _T("MP2 (*.mp2)|*.mp2|") \
    _T("MP3 (*.mp3)|*.mp3|") \
    _T("MPC (*.mpc)|*.mpc|") \
    _T("MPP (*.mpp)|*.mpp|") \
    _T("MP+ (*.mp+)|*.mp+|") \
    _T("MP4 (*.mp4)|*.mp4|") \
    _T("M4A (*.m4a)|*.m4a|") \
    _T("AAC (*.aac)|*.aac|") \
    _T("OGG (*.ogg)|*.ogg|") \
    _T("AC3 (*.ac3)|*.ac3|") \
    _T("FLAC (*.flac)|*.flac|") \
    _T("WV (*.wv)|*.wv|") \
    _T("MAC (*.mac)|*.mac|") \
    _T("APE (*.ape)|*.ape|") \
    _T("OFR (*.ofr)|*.ofr|") \
    _T("TTA (*.tta)|*.tta|") \
    _T("TAK (*.tak)|*.tak||")

// number of supported thread priorities
#define NUM_THREAD_PRIORITY 7

// number of supported process priorities
#define NUM_PROCESS_PRIORITY 4

extern const int g_nThreadPriority[];
extern const DWORD g_dwProcessPriority[];
extern const TCHAR *g_szThreadPriority[];
extern const TCHAR *g_szProcessPriority[];
extern const TCHAR *g_szAllInExt[];
extern const TCHAR *g_szAllOutExt[];
extern const TCHAR *g_szFormatNames[];
extern const TCHAR *g_szNames[];
extern const char *g_szPresetTags[];
extern const TCHAR *g_szPresetFiles[];
extern const char *g_szSettingsTags[];

int GetFormatId(CString szBuff);

class COptions
{
public:
    int nThreadPriorityIndex;
    int nProcessPriorityIndex;
    bool bDeleteOnError;
    bool bStopOnErrors;
    CString szLogFileName;
    int nLogEncoding; // 0 - ANSI, 1 - UNICODE, 2 - UTF-8
public:
    void Copy(COptions &other)
    {
        other.nThreadPriorityIndex = this->nThreadPriorityIndex;
        other.nProcessPriorityIndex = this->nProcessPriorityIndex;
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
