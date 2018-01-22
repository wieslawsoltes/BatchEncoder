// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <map>

namespace app
{
    const std::map<int, const TCHAR*> m_Strings = {

        // Strings Work Thread

        { 0x00110001, _T("Error: can not load progress script.") },
        { 0x00110002, _T("Error: can not find progress function.") },
        { 0x00110003, _T("Error: console line is too large for read buffer.") },

        { 0x00120001, _T("Error: invalid format pipe configuration.") },
        { 0x00120002, _T("Error: can not create pipes for stderr.") },
        { 0x00120003, _T("Error: can not duplicate stderr pipe to prevent child process from closing the pipe.") },
        { 0x00120004, _T("Error: can not create command-line process (%d).") },
        { 0x00120005, _T("Error: progress did not reach 100%.") },
        { 0x00120006, _T("Done") },

        { 0x00130001, _T("Error: invalid format pipe configuration.") },
        { 0x00130002, _T("Error: can not create pipes for stdin.") },
        { 0x00130003, _T("Error: can not set stdin pipe inherit flag.") },
        { 0x00130004, _T("Error: can not create pipes for stdout.") },
        { 0x00130005, _T("Error: can not set stdout pipe inherit flag.") },
        { 0x00130006, _T("Error: can not create command-line process (%d).") },
        { 0x00130007, _T("Error: can not create read thread.") },
        { 0x00130008, _T("Error: can not create write thread.") },
        { 0x00130009, _T("Error: progress did not reach 100%.") },
        { 0x0013000A, _T("Error: can not create pipes for process connection.") },
        { 0x0013000B, _T("Done") },
        { 0x0013000C, _T("Error: can not create pipes for stderr.") },
        { 0x0013000D, _T("Error: can not set stderr pipe inherit flag.") },
        { 0x0013000E, _T("Error: can not create output thread.") },

        { 0x00140001, _T("Error: can not find input file.") },
        { 0x00140002, _T("Error: can not find valid encoder by id.") },
        { 0x00140003, _T("Error: can not find encoder format preset.") },
        { 0x00140004, _T("Error: can not find valid decoder by extension.") },
        { 0x00140005, _T("Error: can not find decoder format preset.") },
        { 0x00140006, _T("Error: decoder output not supported by encoder.") },
        { 0x00140007, _T("Decoding...") },
        { 0x00140008, _T("Error: can not find decoded file.") },
        { 0x00140009, _T("Error: exception thrown while converting file.") },
        { 0x0014000A, _T("Encoding...") },
        { 0x0014000B, _T("Decoding...") },
        { 0x0014000C, _T("Transcoding...") },
        { 0x0014000D, _T("Error: can not find encoded file.") },
        { 0x0014000E, _T("Error: exception thrown while converting file.") },
        { 0x0014000F, _T("Unable to create output path!") },
        { 0x00140010, _T("Output file already exists.") },

        { 0x00150001, _T("--:--") },

        // Strings Worker Context

        { 0x00190001, _T("Error") },
        { 0x00190002, _T("Errors") },
        { 0x00190003, _T("Processing item %d of %d (%d Done, %d %s)") },
        { 0x00190004, _T("Processed %d of %d (%d Done, %d %s) in %s") },

        // Strings Main Dialog

        { 0x00210001, _T("Not Done") },
        { 0x00210002, _T("Item") },
        { 0x00210003, _T("Items") },
        { 0x00210004, _T("No Items") },
        { 0x00210005, _T("Recurse subdirectories") },
        { 0x00210006, _T("Output path:") },
        { 0x00210007, _T("Failed to load file!") },
        { 0x00210008, _T("Failed to save file!") },
        { 0x00210009, _T("Failed to allocate memory for filenames buffer!") },
        { 0x0021000A, _T("Select folder:") },
        { 0x0021000B, _T("Error while searching for language files!") },
        { 0x0021000C, _T("Error while searching for item files!") },
        { 0x0021000D, _T("Unable to create output path!") },
        { 0x0021000E, _T("Fatal error when creating thread!") },
        { 0x0021000F, _T("Invalid output path format!") },

        // Strings Presets Dialog

        { 0x00220001, _T("ERROR") },
        { 0x00220002, _T("Failed to load file!") },
        { 0x00220003, _T("Failed to save file!") },
        { 0x00220004, _T("Default") },

        // Strings Formats Dialog

        { 0x00230001, _T("ERROR") },
        { 0x00230002, _T("Failed to load file!") },
        { 0x00230003, _T("Failed to save file!") },
        { 0x00230004, _T("Format") },
        { 0x00230005, _T("Default") },

        // Strings Tools Dialog

        { 0x00240001, _T("ERROR") },
        { 0x00240002, _T("Failed to load file!") },
        { 0x00240003, _T("Failed to save file!") },
        { 0x00240004, _T("Tool") },

        // Strings File Dialogs

        { 0x00310001, _T("All Files") },
        { 0x00310002, _T("Xml Files") },
        { 0x00310003, _T("Items Files") },
        { 0x00310004, _T("Presets Files") },
        { 0x00310005, _T("Formats Files") },
        { 0x00310006, _T("Exe Files") },
        { 0x00310007, _T("Progress Files") },
        { 0x00310008, _T("Format Files") },
        { 0x00310009, _T("Tools Files") },
        { 0x00310010, _T("Tool Files") },

        // Strings Download Status

        { 0x00400001, _T("Finding resource...") },
        { 0x00400002, _T("Connecting...") },
        { 0x00400003, _T("Sending request...") },
        { 0x00400004, _T("Mime type available") },
        { 0x00400005, _T("Cache filename available") },
        { 0x00400006, _T("Begin download") },
        { 0x00400007, _T("End download") },
        { 0x00400008, _T("Status code : ") },

        // Strings Extract Status

        { 0x00410001, _T("Failed to create unzip folder.") },
        { 0x00410002, _T("Unziped downloaded file.") },
        { 0x00410003, _T("Failed to unzip downloaded file.") }
    };
}

namespace app
{
    extern const TCHAR* pszDefaulTime;
    extern const TCHAR* pszProgresssLoop[];
    extern const TCHAR* pszConvertConsole[];
    extern const TCHAR* pszConvertPipes[];
    extern const TCHAR* pszConvertItem[];
    extern const TCHAR* pszWorkerContext[];
    extern const TCHAR* pszMainDialog[];
    extern const TCHAR* pszPresetsDialog[];
    extern const TCHAR* pszFormatsDialog[];
    extern const TCHAR* pszToolsDialog[];
    extern const TCHAR* pszFileDialogs[];
    extern const TCHAR* pszDownloadStatus[];
    extern const TCHAR* pszExtractStatus[];
}
