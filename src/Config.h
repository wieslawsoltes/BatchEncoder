//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
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

#define MAIN_APP_NAME           _T("BatchEncoder")
#define MAIN_APP_NAME_VER       _T("BatchEncoder Version 1.0.0.0 (C) 2005-2008 Wieslaw Soltes")
#define MAIN_APP_WEBSITE        _T("http://www.BatchEncoder.com/")
#define MAIN_APP_EMAIL          _T("wisodev@users.sourceforge.net")
#define MAIN_APP_HELP           _T("BatchEncoder.pdf")
#define MAIN_APP_CONFIG         _T("BatchEncoder.config")
#define MAIN_APP_LOG            _T("BatchEncoder.log")

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

// number of commandline tools:
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
#define NUM_FILE_ATTRIBUTES 9

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

// number of browse paths to load/save:
//
// CString szBrowsePath[n];
// 
// n   Dialog           Action
// --  ---------------  ------------------
//  0  Main             Load list...
//  1  Main             Save list...
//  -  Main             Create batch-file...
//  2  Main             Add Files
//  3  Main             Add Directory
//  4  Main             ... (browse for outpath)
//  5  Advanced         ... (browse for logfile)
//  6  Formats          Load
//  7  Formats          Save As
//
//  8  Formats          ... (browse for tool #00)
//  9  Formats          ... (browse for tool #01)
// 10  Formats          ... (browse for tool #02)
// 11  Formats          ... (browse for tool #03)
// 12  Formats          ... (browse for tool #04)
// 13  Formats          ... (browse for tool #05)
// 14  Formats          ... (browse for tool #06)
// 15  Formats          ... (browse for tool #07)
// 16  Formats          ... (browse for tool #08)
// 17  Formats          ... (browse for tool #09)
// 18  Formats          ... (browse for tool #10)
// 19  Formats          ... (browse for tool #11)
// 20  Formats          ... (browse for tool #12)
// 21  Formats          ... (browse for tool #13)
// 22  Formats          ... (browse for tool #14)
// 23  Formats          ... (browse for tool #15)
// 24  Formats          ... (browse for tool #16)
// 25  Formats          ... (browse for tool #17)
// 26  Formats          ... (browse for tool #18)
// 27  Formats          ... (browse for tool #19)
// 28  Formats          ... (browse for tool #20)
// 29  Formats          ... (browse for tool #21)
// 30  Formats          ... (browse for tool #22)
// 31  Formats          ... (browse for tool #23)
// 32  Formats          ... (browse for tool #24)
// 33  Formats          ... (browse for tool #25)
// 34  Formats          ... (browse for tool #26)
// 35  Formats          ... (browse for tool #27)
//
// 36  Progress         ... (browse for function #00)
// 37  Progress         ... (browse for function #01)
// 38  Progress         ... (browse for function #02)
// 39  Progress         ... (browse for function #03)
// 40  Progress         ... (browse for function #04)
// 41  Progress         ... (browse for function #05)
// 42  Progress         ... (browse for function #06)
// 43  Progress         ... (browse for function #07)
// 44  Progress         ... (browse for function #08)
// 45  Progress         ... (browse for function #09)
// 46  Progress         ... (browse for function #10)
// 47  Progress         ... (browse for function #11)
// 48  Progress         ... (browse for function #12)
// 49  Progress         ... (browse for function #13)
// 50  Progress         ... (browse for function #14)
// 51  Progress         ... (browse for function #15)
// 52  Progress         ... (browse for function #16)
// 53  Progress         ... (browse for function #17)
// 54  Progress         ... (browse for function #18)
// 55  Progress         ... (browse for function #19)
// 56  Progress         ... (browse for function #20)
// 57  Progress         ... (browse for function #21)
// 58  Progress         ... (browse for function #22)
// 59  Progress         ... (browse for function #23)
// 60  Progress         ... (browse for function #24)
// 61  Progress         ... (browse for function #25)
// 62  Progress         ... (browse for function #26)
// 63  Progress         ... (browse for function #27)
//
// 64  Presets          Load #00
// 65  Presets          Save As #00
// 66  Presets          Load #01
// 67  Presets          Save As #01
// 68  Presets          Load #02
// 69  Presets          Save As #02
// 70  Presets          Load #03
// 71  Presets          Save As #03
// 72  Presets          Load #04
// 73  Presets          Save As #04
// 74  Presets          Load #05
// 75  Presets          Save As #05
// 76  Presets          Load #06
// 77  Presets          Save As #06
// 78  Presets          Load #07
// 79  Presets          Save As #07
// 80  Presets          Load #08
// 81  Presets          Save As #08
// 82  Presets          Load #09
// 83  Presets          Save As #09
// 84  Presets          Load #10
// 85  Presets          Save As #10
// 86  Presets          Load #11
// 87  Presets          Save As #11
#define NUM_BROWSE_PATH 88

#define NUM_BROWSE_PATH_ADVANCED 1
#define START_BROWSE_PATH_ADVANCED 5

#define NUM_BROWSE_PATH_FORMATS 30
#define START_BROWSE_PATH_FORMATS 6

#define NUM_BROWSE_PATH_PROGRESS 30
#define START_BROWSE_PATH_PROGRESS 36

#define NUM_BROWSE_PATH_PRESETS 24
#define START_BROWSE_PATH_PRESETS 64

// number of supported thread priorities
#define NUM_THREAD_PRIORITY 7

// number of supported process priorities
#define NUM_PROCESS_PRIORITY 4

// number of progress icons
#define NUM_PROGRESS_ICONS 12

// global configuration constants
extern const int g_nProgressIconResources[NUM_PROGRESS_ICONS];
extern const int g_nThreadPriority[];
extern const DWORD g_dwProcessPriority[];
extern const TCHAR *g_szThreadPriority[];
extern const TCHAR *g_szProcessPriority[];
extern const TCHAR *g_szAllInExt[];
extern const TCHAR *g_szAllOutExt[];
extern const TCHAR *g_szFormatNames[];
extern const TCHAR *g_szPresetNames[];
extern const char *g_szPresetTags[];
extern const TCHAR *g_szPresetFiles[];
extern const int g_nPresetResources[NUM_PRESET_FILES];
extern const char *g_szFileAttributes[];
extern const char *g_szSettingsTags[];
extern const char *g_szColorsTags[];

// default configuration constants
extern const TCHAR *g_szDefaultTemplate[];
extern const TCHAR *g_szDefaultPath[];
extern const bool g_bDefaultInPipes[NUM_FORMAT_NAMES];
extern const bool g_bDefaultOutPipes[NUM_FORMAT_NAMES];
extern const TCHAR *g_bDefaultFunction[];
