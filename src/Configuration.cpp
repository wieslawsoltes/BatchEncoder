// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "Configuration.h"
#include "BatchEncoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char *g_szOptionsTags[NUM_PROGRAM_OPTIONS] =
{
    "SelPresetIndex",           // 0
    "SelFormatIndex",           // 1
    "BrowsePath",               // 2
    "DebugOutput",              // 3
    "DeleteSourceFiles",        // 4
    "StayOnTop",                // 5
    "RecurseChecked",           // 6
    "MainWndResize",            // 7
    "FileListColumns",          // 8
    "ShowGridLines",            // 9
    "ShowTrayIcon",             // 10
    "DoNotSaveConfiguration",   // 11
    "PresetsWndResize",         // 12
    "PresetsListColumns",       // 13
    "FormatsWndResize",         // 14
    "FormatsListColumns",       // 15
    "DeleteOnError",            // 16
    "StopOnErrors",             // 17
    "LogFileName",              // 18
    "LogFileEncoding",          // 19
    "ForceConsoleWnd"           // 20
};

const TCHAR *g_szAllInExt[NUM_INPUT_EXT] =
{                 // nInFormat
    _T("WAV"),  // 0
    _T("MP1"),  // 1
    _T("MP2"),  // 2
    _T("MP3"),  // 3
    _T("MPC"),  // 4
    _T("MPP"),  // 5
    _T("MP+"),  // 6
    _T("MP4"),  // 7
    _T("M4A"),  // 8
    _T("AAC"),  // 9
    _T("OGG"),  // 10
    _T("AC3"),  // 11
    _T("FLAC"), // 12
    _T("WV"),   // 13
    _T("APE"),  // 14
    _T("MAC"),  // 15
    _T("OFR"),  // 16
    _T("TTA"),  // 17
    _T("TAK")   // 18
};

const TCHAR *g_szAllOutExt[NUM_OUTPUT_EXT] =
{                 // nOutFormat
    _T("WAV"),  // 0
    _T("MP3"),  // 1
    _T("MPC"),  // 2
    _T("AAC"),  // 3
    _T("M4A"),  // 4
    _T("MP4"),  // 5
    _T("OGG"),  // 6
    _T("AC3"),  // 7
    _T("FLAC"), // 8
    _T("WV"),   // 9
    _T("APE"),  // 10
    _T("OFR"),  // 11
    _T("TTA"),  // 12
    _T("TAK")   // 13
};

const TCHAR *g_szFormatNames[NUM_FORMAT_NAMES] =
{
    // ENCODERS
    _T("ENC_WAV"),    // 0
    _T("ENC_MP3"),    // 1
    _T("ENC_MPC"),    // 2
    _T("ENC_AAC"),    // 3
    _T("ENC_M4A"),    // 4
    _T("ENC_MP4"),    // 5
    _T("ENC_OGG"),    // 6
    _T("ENC_AC3"),    // 7
    _T("ENC_FLAC"),   // 8
    _T("ENC_WV"),     // 9
    _T("ENC_APE"),    // 10
    _T("ENC_OFR"),    // 11
    _T("ENC_TTA"),    // 12
    _T("ENC_TAK"),    // 13
    // DECODERS (NUM_OUTPUT_EXT + IntputFormat - 1)
    _T("DEC_MP1"),    // 14
    _T("DEC_MP2"),    // 15
    _T("DEC_MP3"),    // 16
    _T("DEC_MPC"),    // 17
    _T("DEC_MPP"),    // 18
    _T("DEC_MP+"),    // 19
    _T("DEC_MP4"),    // 20
    _T("DEC_M4A"),    // 21
    _T("DEC_AAC"),    // 22
    _T("DEC_OGG"),    // 23
    _T("DEC_AC3"),    // 24 (NOTE: not working, no AC3 decoder included in Aften)
    _T("DEC_FLAC"),   // 25
    _T("DEC_WV"),     // 26
    _T("DEC_APE"),    // 27
    _T("DEC_MAC"),    // 28
    _T("DEC_OFR"),    // 29
    _T("DEC_TTA"),    // 30
    _T("DEC_TAK")     // 31 (NOTE: not working, TAK decoding using pipes & console output does not work)
};

const TCHAR *g_szPresetNames[NUM_PRESET_FILES] =
{
    _T("WAV (Standard)"),                         // 0
    _T("MP3 (MPEG Layer-3)"),                     // 1
    _T("MPC (Musepack)"),                         // 2
    _T("AAC (MPEG-2/AAC)"),                       // 3
    _T("M4A (MPEG-4/AAC)"),                       // 4
    _T("MP4 (MPEG-4/AAC)"),                       // 5
    _T("OGG (Ogg Vorbis)"),                       // 6
    _T("AC3 (ATSC A/52)"),                        // 7
    _T("FLAC (Free Lossless Audio Codec)"),       // 8
    _T("WV (WavPack)"),                           // 9
    _T("APE (Monkey's Audio)"),                   // 10
    _T("OFR (OptimFROG Lossless Audio)"),         // 11
    _T("TTA (TTAv1 Lossless Audio)"),             // 12
    _T("TAK (Tom's Lossless Audio Kompressor)")   // 13
};

const char *g_szPresetTags[NUM_PRESET_FILES] =
{
    "PRESETS_WAV",      // 0
    "PRESETS_MP3",      // 1
    "PRESETS_MPC",      // 2
    "PRESETS_AAC",      // 3
    "PRESETS_M4A",      // 4
    "PRESETS_MP4",      // 5
    "PRESETS_OGG",      // 6
    "PRESETS_AC3",      // 7
    "PRESETS_FLAC",     // 8
    "PRESETS_WV",       // 9
    "PRESETS_APE",      // 10
    "PRESETS_OFR",      // 11
    "PRESETS_TTA",      // 12
    "PRESETS_TAK"       // 13
};

const TCHAR *g_szPresetFiles[NUM_PRESET_FILES] =
{
    _T("Presets_WAV_Ssrc.presets"),       // 0
    _T("Presets_MP3_Lame.presets"),       // 1
    _T("Presets_MPC_MpcEnc.presets"),     // 2
    _T("Presets_AAC_Faac.presets"),       // 3
    _T("Presets_M4A_neroAacEnc.presets"), // 4
    _T("Presets_MP4_neroAacEnc.presets"), // 5
    _T("Presets_OGG_OggEnc.presets"),     // 6
    _T("Presets_AC3_Aften.presets"),      // 7
    _T("Presets_FLAC_Flac.presets"),      // 8
    _T("Presets_WV_WavPack.presets"),     // 9
    _T("Presets_APE_Mac.presets"),        // 10
    _T("Presets_OFR_Ofr.presets"),        // 11
    _T("Presets_TTA_TtaEnc.presets"),     // 12
    _T("Presets_TAK_Takc.presets")        // 13
};

int GetFormatId(CString szBuff)
{
    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        if (szBuff.Compare(g_szFormatNames[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}
