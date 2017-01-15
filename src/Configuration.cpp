// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "Configuration.h"
#include "BatchEncoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR *g_szAllInExt[NUM_INPUT_EXT] =
{
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
{
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
