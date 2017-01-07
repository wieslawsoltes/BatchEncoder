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

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// the progress icons IDs
// items: NUM_PROGRESS_ICONS
const int g_nProgressIconResources[NUM_PROGRESS_ICONS] = 
{
    IDI_ICON_PROGRESS_01,   // 0
    IDI_ICON_PROGRESS_02,   // 1
    IDI_ICON_PROGRESS_03,   // 2
    IDI_ICON_PROGRESS_04,   // 3
    IDI_ICON_PROGRESS_05,   // 4
    IDI_ICON_PROGRESS_06,   // 5
    IDI_ICON_PROGRESS_07,   // 6
    IDI_ICON_PROGRESS_08,   // 7
    IDI_ICON_PROGRESS_09,   // 8
    IDI_ICON_PROGRESS_10,   // 9
    IDI_ICON_PROGRESS_11,   // 10
    IDI_ICON_PROGRESS_12    // 11
};

// supported thread priorities
// items: NUM_THREAD_PRIORITY
const int g_nThreadPriority[NUM_THREAD_PRIORITY] = 
{
    THREAD_PRIORITY_IDLE,           // 0
    THREAD_PRIORITY_LOWEST,         // 1
    THREAD_PRIORITY_BELOW_NORMAL,   // 2
    THREAD_PRIORITY_NORMAL,         // 3
    THREAD_PRIORITY_ABOVE_NORMAL,   // 4
    THREAD_PRIORITY_HIGHEST,        // 5
    THREAD_PRIORITY_TIME_CRITICAL   // 6
};

// supported process priorities
// items: NUM_PROCESS_PRIORITY
const DWORD g_dwProcessPriority[NUM_PROCESS_PRIORITY] = 
{
    IDLE_PRIORITY_CLASS,            // 0
    NORMAL_PRIORITY_CLASS,          // 1
    HIGH_PRIORITY_CLASS,            // 2
    REALTIME_PRIORITY_CLASS         // 3
};

// supported thread priority names
// items: NUM_THREAD_PRIORITY
const TCHAR *g_szThreadPriority[] = 
{
    _T("Idle"),                   // 0
    _T("Lowest"),                 // 1
    _T("Below Normal"),           // 2
    _T("Normal"),                 // 3
    _T("Above Normal"),           // 4
    _T("Highest"),                // 5
    _T("Time Critical")           // 6
};

// supported process priority names
// items: NUM_PROCESS_PRIORITY
const TCHAR *g_szProcessPriority[] = 
{
    _T("Idle"),                   // 0
    _T("Normal"),                 // 1
    _T("High"),                   // 2
    _T("RealTime")                // 3
};

// file tag attributes
// items: NUM_FILE_ATTRIBUTES
const char *g_szFileAttributes[] = 
{
    "path",     // 0
    "checked",  // 1
    "name",     // 2
    "type",     // 3
    "size",     // 4
    "output",   // 5
    "preset",   // 6
    "time",     // 7
    "status"    // 8
};

// tags used to store program settings
// items: NUM_PROGRAM_SETTINGS
const char *g_szSettingsTags[] = 
{
    "SelPresetIndex",       // 0
    "SelFormatIndex",       // 1
    "BrowsePath",           // 2
    "DebugOutput",          // 3
    "DeleteSourceFiles",    // 4
    "StayOnTop",            // 5
    "RecurseChecked",       // 6
    "MainWndResize",        // 7
    "FileListColumns",      // 8
    "ShowGridLines",        // 9
    "ShowTrayIcon",         // 10
    "DoNotSaveSettings",    // 11
    "PresetsWndResize",     // 12
    "PresetsListColumns",   // 13
    "FormatsWndResize",     // 14
    "FormatsListColumns",   // 15
    "ThreadPriority",       // 16
    "ProcessPriority",      // 17
    "DeleteOnError",        // 18
    "StopOnErrors",         // 19
    "LogFileName",          // 20
    "LogFileEncoding",      // 21
    "ProgressWndStart",     // 22
    "ForceConsoleWnd"       // 23
};

// tags used to store conversion progress and histogram colors
// items: NUM_PROGRAM_COLORS
const char *g_szColorsTags[] = 
{
    "CnvStatusText",        // 0
    "CnvStatusTextError",   // 1
    "CnvStatusProgress",    // 2
    "CnvStatusBorder",      // 3
    "CnvStatusBack",        // 4
    "HistogramLR",          // 5
    "HistogramMS",          // 6
    "HistogramBorder",      // 7
    "HistogramBack"         // 8
};

// supported input file extensions
// items: NUM_INPUT_EXT
const TCHAR *g_szAllInExt[] = 
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

// supported output file extensions
// items: NUM_OUTPUT_EXT
const TCHAR *g_szAllOutExt[] = 
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

// supported format tool names
// items: NUM_FORMAT_NAMES
const TCHAR *g_szFormatNames[] =
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
    _T("DEC_TAK")     // 31 (NOTE: not working, TAK decoding using pipes&console output does not work)
};

// default presets names for each file format
// items: NUM_PRESET_FILES
const TCHAR *g_szPresetNames[] = 
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

// tags used to store presets filenames
// items: NUM_PRESET_FILES
const char *g_szPresetTags[] = 
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

// default presets config filenames
// items: NUM_PRESET_FILES
const TCHAR *g_szPresetFiles[] = 
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

// default presets resources IDs
// items: NUM_PRESET_FILES
const int g_nPresetResources[NUM_PRESET_FILES] = 
{
    IDR_PRESET_WAV_SSRC,    // 0
    IDR_PRESET_MP3_LAME,    // 1
    IDR_PRESET_MPC_MPCENC,  // 2
    IDR_PRESET_AAC_FAAC,    // 3
    IDR_PRESET_M4A_NERO,    // 4
    IDR_PRESET_MP4_NERO,    // 5
    IDR_PRESET_OGG_OGGENC,  // 6
    IDR_PRESET_AC3_AFTEN,   // 7
    IDR_PRESET_FLAC_FLAC,   // 8
    IDR_PRESET_WV_WAVPACK,  // 9
    IDR_PRESET_APE_MAC,     // 10
    IDR_PRESET_OFR_OFR,     // 11
    IDR_PRESET_TTA_TTAENC,  // 12
    IDR_PRESET_TAK_TAKC     // 13
};

// default commandline templates
// items: NUM_FORMAT_NAMES
const TCHAR *g_szDefaultTemplate[] =
{
    // ENCODERS
    _T("$EXE $OPTIONS $INFILE $OUTFILE"),                                 // 0
    _T("$EXE $OPTIONS $INFILE $OUTFILE"),                                 // 1
    _T("$EXE $OPTIONS --overwrite $INFILE $OUTFILE"),                     // 2
    _T("$EXE $OPTIONS -o $OUTFILE $INFILE"),                              // 3
    _T("$EXE $OPTIONS -o $OUTFILE $INFILE"),                              // 4
    _T("$EXE $OPTIONS -o $OUTFILE $INFILE"),                              // 5
    _T("$EXE $OPTIONS $INFILE -o $OUTFILE"),                              // 6
    _T("$EXE $OPTIONS $INFILE $OUTFILE"),                                 // 7
    _T("$EXE $OPTIONS -f -o $OUTFILE $INFILE"),                           // 8
    _T("$EXE $OPTIONS -y $INFILE $OUTFILE"),                              // 9
    _T("$EXE $INFILE $OUTFILE $OPTIONS"),                                 // 10
    _T("$EXE $OPTIONS --overwrite $INFILE --output $OUTFILE"),            // 11
    _T("$EXE $OPTIONS -o $OUTFILE $INFILE"),                              // 12
    _T("$EXE -e $OPTIONS $INFILE $OUTFILE "),                             // 13
    // DECODERS
    _T("$EXE --decode --mp1input $INFILE $OUTFILE"),             // 14
    _T("$EXE --decode --mp2input $INFILE $OUTFILE"),             // 15
    _T("$EXE --decode --mp3input $INFILE $OUTFILE"),             // 16
    _T("$EXE $INFILE $OUTFILE"),                                 // 17
    _T("$EXE $INFILE $OUTFILE"),                                 // 18
    _T("$EXE $INFILE $OUTFILE"),                                 // 19
    _T("$EXE -o $OUTFILE $INFILE"),                              // 20
    _T("$EXE -o $OUTFILE $INFILE"),                              // 21
    _T("$EXE -o $OUTFILE $INFILE"),                              // 22
    _T("$EXE -o $OUTFILE $INFILE"),                              // 23
    _T("$EXE $OPTIONS $INFILE $OUTFILE"),                        // 24
    _T("$EXE -d -f -o $OUTFILE $INFILE"),                        // 25
    _T("$EXE -y $INFILE $OUTFILE"),                              // 26
    _T("$EXE $INFILE $OUTFILE -d"),                              // 27
    _T("$EXE $INFILE $OUTFILE -d"),                              // 28
    _T("$EXE --decode --overwrite $INFILE --output $OUTFILE"),   // 29
    _T("$EXE -d -o $OUTFILE $INFILE"),                           // 30
    _T("$EXE -d $INFILE $OUTFILE")                               // 31
};

// default names for commandline executables
// items: NUM_FORMAT_NAMES
const TCHAR *g_szDefaultPath[] = 
{
    // ENCODERS
    _T("ssrc.exe"),           // 0
    _T("lame.exe"),           // 1
    _T("mppenc.exe"),         // 2
    _T("faac.exe"),           // 3
    _T("faac.exe"),           // 4
    _T("faac.exe"),           // 5
    _T("oggenc2.exe"),        // 6
    _T("aften.exe"),          // 7
    _T("flac.exe"),           // 8
    _T("wavpack.exe"),        // 9
    _T("mac.exe"),            // 10
    _T("ofr.exe"),            // 11
    _T("ttaenc.exe"),         // 12
    _T("takc.exe"),           // 13
    // DECODERS
    _T("lame.exe"),           // 14
    _T("lame.exe"),           // 15
    _T("lame.exe"),           // 16
    _T("mppdec.exe"),         // 17
    _T("mppdec.exe"),         // 18
    _T("mppdec.exe"),         // 19
    _T("faad.exe"),           // 20
    _T("faad.exe"),           // 21
    _T("faad.exe"),           // 22
    _T("oggdec.exe"),         // 23
    _T("aften.exe"),          // 24
    _T("flac.exe"),           // 25
    _T("wvunpack.exe"),       // 26
    _T("mac.exe"),            // 27
    _T("mac.exe"),            // 28
    _T("ofr.exe"),            // 29
    _T("ttaenc.exe"),         // 30
    _T("takc.exe")            // 31
};

// default input pipes flags for commandline tools
// items: NUM_FORMAT_NAMES
const bool g_bDefaultInPipes[NUM_FORMAT_NAMES] = 
{
    // ENCODERS
    false,  // 0
    false,  // 1
    true,   // 2
    true,   // 3
    true,   // 4
    true,   // 5
    true,   // 6
    true,   // 7
    true,   // 8
    false,  // 9
    false,  // 10
    true,   // 11
    false,  // 12
    true,   // 13
    // DECODERS
    false,  // 14
    false,  // 15
    false,  // 16
    true,   // 17
    true,   // 18
    true,   // 19
    false,  // 20
    false,  // 21
    false,  // 22
    false,  // 23
    true,   // 24
    true,   // 25
    false,  // 26
    false,  // 27
    false,  // 28
    true,   // 29
    false,  // 30
    true    // 31
};

// default output pipes flags for commandline tools
const bool g_bDefaultOutPipes[NUM_FORMAT_NAMES] = 
{
    // ENCODERS
    false,  // 0
    false,  // 1
    false,  // 2
    false,  // 3
    false,  // 4
    false,  // 5
    false,  // 6
    false,  // 7
    false,  // 8
    false,  // 9
    false,  // 10
    false,  // 11
    false,  // 12
    false,  // 13
    // DECODERS
    false,  // 14
    false,  // 15
    false,  // 16
    false,  // 17
    false,  // 18
    false,  // 19
    false,  // 20
    false,  // 21
    false,  // 22
    false,  // 23
    false,  // 24
    false,  // 25
    false,  // 26
    false,  // 27
    false,  // 28
    false,  // 29
    false,  // 30
    false   // 31
};

// default GetProgress functions
// items: NUM_FORMAT_NAMES
const TCHAR *g_bDefaultFunction[] =
{
    // ENCODERS
    _T("SsrcEnc.progress"),        // 0
    _T("LameEnc.progress"),        // 1
    _T("MppEnc.progress"),         // 2
    _T("FaacEnc.progress"),        // 3
    _T("- none -"),                // 4
    _T("- none -"),                // 5
    _T("OggEnc.progress"),         // 6
    _T("- none -"),                // 7
    _T("FlacEnc.progress"),        // 8
    _T("WavPackEnc.progress"),     // 9
    _T("MacEnc.progress"),         // 10
    _T("OfrEnc.progress"),         // 11
    _T("TtaEnc.progress"),         // 12
    _T("- none -"),                // 13
    // DECODERS
    _T("LameDec.progress"),        // 14
    _T("LameDec.progress"),        // 15
    _T("LameDec.progress"),        // 16
    _T("MppDec.progress"),         // 17
    _T("MppDec.progress"),         // 18
    _T("MppDec.progress"),         // 19
    _T("FaadDec.progress"),        // 20
    _T("FaadDec.progress"),        // 21
    _T("FaadDec.progress"),        // 22
    _T("OggDec.progress"),         // 23
    _T("- none -"),                // 24
    _T("FlacDec.progress"),        // 25
    _T("WvUnpackDec.progress"),    // 26
    _T("MacDec.progress"),         // 27
    _T("MacDec.progress"),         // 28
    _T("OfrDec.progress"),         // 29
    _T("TtaDec.progress"),         // 30
    _T("- none -")                 // 31
};
