// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"

const TCHAR* pszDefaulTime = _T("--:--");

const TCHAR* pszProgresssLoop[] = {
    /* 00 */ _T("Error: can not load GetProgress function library dll."),
    /* 01 */ _T("Error: can not get GetProgress function address."),
    /* 02 */ _T("Error: console line is too large for read buffer.")
};

const TCHAR* pszConvertConsole[] = {
    /* 00 */ _T("Error: invalid format pipe configuration."),
    /* 01 */ _T("Error: can not create pipes for stderr."),
    /* 02 */ _T("Error: can not duplicate stderr pipe to prevent child process from closing the pipe."),
    /* 03 */ _T("Error: can not create command-line process (%d)."),
    /* 04 */ _T("Error: progress did not reach 100%."),
    /* 05 */ _T("Done")
};

const TCHAR* pszConvertPipes[] = {
    /* 00 */ _T("Error: invalid format pipe configuration."),
    /* 01 */ _T("Error: can not create pipes for stdin."),
    /* 02 */ _T("Error: can not set stdin pipe inherit flag."),
    /* 03 */ _T("Error: can not create pipes for stdout."),
    /* 04 */ _T("Error: can not set stdout pipe inherit flag."),
    /* 05 */ _T("Error: can not create command-line process (%d)."),
    /* 06 */ _T("Error: can not create read thread."),
    /* 07 */ _T("Error: can not create write thread."),
    /* 08 */ _T("Error: progress did not reach 100%."),
    /* 09 */ _T("Done")
};

const TCHAR* pszConvertItem[] = {
    /* 00 */ _T("Error: can not find input file."),
    /* 01 */ _T("Error: can not find valid encoder by id."),
    /* 02 */ _T("Error: can not find encoder format preset."),
    /* 03 */ _T("Error: can not find valid decoder by extension."),
    /* 04 */ _T("Error: can not find decoder format preset."),
    /* 05 */ _T("Error: decoder output not supported by encoder."),
    /* 06 */ _T("Decoding..."),
    /* 07 */ _T("Error: can not find decoded file."),
    /* 08 */ _T("Error: exception thrown while converting file."),
    /* 09 */ _T("Encoding..."),
    /* 10 */ _T("Decoding..."),
    /* 11 */ _T("Processing..."),
    /* 12 */ _T("Error: can not find encoded file."),
    /* 13 */ _T("Error: exception thrown while converting file.")
};

const TCHAR* pszMainDialog[] = {
    /* 00 */ _T("Not Done"),
    /* 01 */ _T("Item"),
    /* 02 */ _T("Items"),
    /* 03 */ _T("No Items"),
    /* 04 */ _T("Recurse subdirectories"),
    /* 05 */ _T("<< same as source file >>"),
    /* 06 */ _T("Output path:"),
    /* 07 */ _T("Failed to load file!"),
    /* 08 */ _T("Failed to save file!"),
    /* 09 */ _T("Failed to allocate memory for filenames buffer!"),
    /* 10 */ _T("Select folder:"),
    /* 11 */ _T("Error while searching for language files!"),
    /* 12 */ _T("Error while searching for item files!"),
    /* 13 */ _T("Unable to create output path!"),
    /* 14 */ _T("Fatal error when creating thread!")
};

const TCHAR* pszPresetsDialog[] = {
    /* 00 */ _T("ERROR"),
    /* 01 */ _T("Failed to load file!"),
    /* 02 */ _T("Failed to save file!"),
    /* 03 */ _T("Default")
}; 

const TCHAR* pszFormatsDialog[] = {
    /* 00 */ _T("ERROR"),
    /* 01 */ _T("Failed to load file!"),
    /* 02 */ _T("Failed to save file!"),
    /* 03 */ _T("Format"),
    /* 04 */ _T("Default")
};

const TCHAR* pszFileDialogs[] = {
    /* 00 */ _T("All Files"),
    /* 01 */ _T("Xml Files"),
    /* 02 */ _T("Items Files"),
    /* 03 */ _T("Presets Files"),
    /* 04 */ _T("Formats Files"),
    /* 05 */ _T("Exe Files"),
    /* 06 */ _T("Progress Files")
};
