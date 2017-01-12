// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "afxwin.h"
#include "afxcmn.h"

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
#define NUM_PROGRAM_SETTINGS 24

// number of tags used to store program colors
#define NUM_PROGRAM_COLORS 9

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

// number of progress icons
#define NUM_PROGRESS_ICONS 12

extern const int g_nProgressIconResources[NUM_PROGRESS_ICONS];
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
extern const int g_nPresetResources[NUM_PRESET_FILES];
extern const char *g_szSettingsTags[];
extern const char *g_szColorsTags[];

extern const TCHAR *g_szDefaultTemplate[];
extern const TCHAR *g_szDefaultPath[];
extern const bool g_bDefaultInPipes[NUM_FORMAT_NAMES];
extern const bool g_bDefaultOutPipes[NUM_FORMAT_NAMES];
extern const TCHAR *g_bDefaultFunction[];

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

class CColor
{
public:
    unsigned char nRed;
    unsigned char nGreen;
    unsigned char nBlue;
public:
    void Copy(CColor &other)
    {
        other.nRed = this->nRed;
        other.nGreen = this->nGreen;
        other.nBlue = this->nBlue;
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

#include "ItemsList.h"
#include "PresetsList.h"

class CConfiguration
{
public:
    COptions m_Options;
    CColor m_Colors[NUM_PROGRAM_COLORS];
    CFormat m_Formats[NUM_FORMAT_NAMES];
    CPresetsList m_Presets[NUM_PRESET_FILES];
    CString szPresetsFile[NUM_PRESET_FILES];
    CItemsList m_Items;
};

// nAction
#define ADD_ITEM_NONE              -1
#define ADD_ITEM_MEMORY             0
#define ADD_ITEM_CONTROL            1
#define ADD_ITEM_MEMORY_AND_CONTROL 2

typedef struct TNewItemData
{
    int nAction;
    int nItem;
    CString szPath;
    CString szName;
    CString szOutExt;
    int nOutPreset;
    bool bChecked;
    CString szTime;
    CString szStatus;

} NewItemData, *PNewItemData;

void InitNewItemData(NewItemData &nid);
