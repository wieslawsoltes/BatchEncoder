// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <map>
#include <tchar.h>

namespace config
{
    const std::map<int, const TCHAR*> m_Strings =
    {
        // File Menu

        { 0x00010001, _T("&File") },
        { 0x00010002, _T("&Load List...\tCtrl+L") },
        { 0x00010003, _T("&Save List...\tCtrl+S") },
        { 0x00010004, _T("Cl&ear List\tCtrl+E") },
        { 0x00010005, _T("E&xit\tAlt+F4") },

        // Edit Menu

        { 0x00020001, _T("&Edit") },
        { 0x00020002, _T("&Add Files...\tF5") },
        { 0x00020003, _T("Add Dire&ctory...\tF6") },
        { 0x00020004, _T("Rena&me\tF2") },
        { 0x00020005, _T("Rese&t Time/Status\tF3") },
        { 0x00020006, _T("Reset Output&/Preset\tF4") },
        { 0x00020007, _T("&Remove\tDEL") },
        { 0x00020008, _T("Cro&p\tINS") },
        { 0x00020009, _T("Remove Chec&ked\tCtrl++") },
        { 0x0002000A, _T("Remove Unc&hecked\tCtrl+-") },
        { 0x0002000B, _T("Check S&elected\tShift++") },
        { 0x0002000C, _T("&Uncheck Selected\tShift+-") },
        { 0x0002000D, _T("&Select All\tCtrl+A") },
        { 0x0002000E, _T("Select &None\tCtrl+N") },
        { 0x0002000F, _T("&Invert Selection\tCtrl+I") },
        { 0x00020010, _T("&Open File...") },
        { 0x00020011, _T("Open &Directory...") },

        // Action Menu

        { 0x00030001, _T("&Action") },
        { 0x00030002, _T("Conve&rt\tF9") },
        { 0x00030003, _T("S&top\tF9") },

        // Options Menu

        { 0x00040001, _T("&Options") },
        { 0x00040002, _T("Configure &Presets...\tF7") },
        { 0x00040003, _T("Configure &Formats...\tF8") },
        { 0x00040004, _T("&Delete Source Files") },
        { 0x00040005, _T("&Shutdown When Finished") },
        { 0x00040006, _T("Do Not Save Configuration On E&xit") },
        { 0x00040007, _T("Delete &Output File On Error") },
        { 0x00040008, _T("Stop &Conversion On Error") },
        { 0x00040009, _T("&Hide Console Window") },
        { 0x0004000A, _T("&Ensure Item Is Visible") },
        { 0x0004000B, _T("&Try To Find Decoder") },
        { 0x0004000C, _T("&Validate Input Files") },
        { 0x0004000D, _T("Ove&rwrite Existing Files") },
        { 0x0004000E, _T("Configure Too&ls...\tF10") },
        { 0x0004000F, _T("Tr&y To Download Tools") },

        // Language Menu

        { 0x00050001, _T("&Language") },
        { 0x00050002, _T("&Default (English)") },

        // Help Menu

        { 0x00060001, _T("&Help") },
        { 0x00060002, _T("&Website...") },
        { 0x00060003, _T("&About...") },

        // Main Dialog

        { 0x000A0001, _T("Name") },
        { 0x000A0002, _T("Input") },
        { 0x000A0003, _T("Size (bytes)") },
        { 0x000A0004, _T("Output") },
        { 0x000A0005, _T("Preset#") },
        { 0x000A0006, _T("Time") },
        { 0x000A0007, _T("Status") },

        { 0x000A0010, _T("BatchEncoder") },
        { 0x000A0011, _T("Output") },
        { 0x000A0012, _T("format:") },
        { 0x000A0013, _T("preset:") },
        { 0x000A0014, _T("Output path:") },
        { 0x000A0015, _T("&...") },
        { 0x000A0016, _T("threads (0=auto):") },
        { 0x000A0017, _T("Conve&rt") },
        { 0x000A0018, _T("S&top") },

        // Presets Dialog

        { 0x000B0001, _T("Name") },
        { 0x000B0002, _T("Options") },

        { 0x000B0010, _T("Configure Presets") },
        { 0x000B0011, _T("&Cancel") },
        { 0x000B0012, _T("Name:") },
        { 0x000B0013, _T("Command-line options:") },
        { 0x000B0014, _T("move &Up") },
        { 0x000B0015, _T("move Dow&n") },
        { 0x000B0016, _T("Dele&te All") },
        { 0x000B0017, _T("&Delete") },
        { 0x000B0018, _T("&Add") },
        { 0x000B0019, _T("&Load") },
        { 0x000B001A, _T("Sa&ve As") },
        { 0x000B001B, _T("C&hange") },
        { 0x000B001C, _T("&OK") },
        { 0x000B001D, _T("Du&plicate") },

        // Formats Dialog

        { 0x000C0001, _T("Name") },
        { 0x000C0002, _T("Template") },

        { 0x000C0010, _T("Configure Formats") },
        { 0x000C0011, _T("&Cancel") },
        { 0x000C0012, _T("Encoder") },
        { 0x000C0013, _T("Decoder") },
        { 0x000C0014, _T("Type:") },
        { 0x000C0015, _T("Input") },
        { 0x000C0016, _T("Output") },
        { 0x000C0017, _T("Pipes:") },
        { 0x000C0018, _T("Id:") },
        { 0x000C0019, _T("Name:") },
        { 0x000C001A, _T("Output extension:") },
        { 0x000C001B, _T("Input extensions (separated by comma):") },
        { 0x000C001C, _T("Default preset:") },
        { 0x000C001D, _T("Command-line executable path:") },
        { 0x000C001E, _T("&...") },
        { 0x000C001F, _T("Command-line template:") },
        { 0x000C0020, _T("Progress function:") },
        { 0x000C0021, _T("...") },
        { 0x000C0022, _T("move &Up") },
        { 0x000C0023, _T("move Dow&n") },
        { 0x000C0024, _T("Dele&te All") },
        { 0x000C0025, _T("&Delete") },
        { 0x000C0026, _T("&Add") },
        { 0x000C0027, _T("&Load") },
        { 0x000C0028, _T("&Save As") },
        { 0x000C0029, _T("C&hange") },
        { 0x000C002A, _T("&OK") },
        { 0x000C002B, _T("Exit code on success:") },
        { 0x000C002C, _T("&Edit Presets") },
        { 0x000C002D, _T("&Import") },
        { 0x000C002E, _T("E&xport") },
        { 0x000C002F, _T("Du&plicate") },

        // Tools Dialog

        { 0x000E0001, _T("Name") },
        { 0x000E0002, _T("Url") },
        { 0x000E0003, _T("Status") },

        { 0x000E0010, _T("Configure Tools") },
        { 0x000E0011, _T("&Cancel") },
        { 0x000E0012, _T("Name:") },
        { 0x000E0013, _T("Platform:") },
        { 0x000E0014, _T("Supported formats (separated by comma):") },
        { 0x000E0015, _T("Download URL:") },
        { 0x000E0016, _T("Download file name:") },
        { 0x000E0017, _T("Extract method:") },
        { 0x000E0018, _T("Extract path:") },
        { 0x000E0019, _T("move &Up") },
        { 0x000E001A, _T("move Dow&n") },
        { 0x000E001B, _T("&Import") },
        { 0x000E001C, _T("E&xport") },
        { 0x000E001D, _T("Du&plicate") },
        { 0x000E001E, _T("Dele&te All") },
        { 0x000E001F, _T("&Delete") },
        { 0x000E0020, _T("&Add") },
        { 0x000E0021, _T("&Load") },
        { 0x000E0022, _T("&Save As") },
        { 0x000E0023, _T("Do&wnload Selected") },
        { 0x000E0024, _T("Stop Do&wnload") },
        { 0x000E0025, _T("S&et Selected Paths") },
        { 0x000E0026, _T("Set x&86 Paths") },
        { 0x000E0027, _T("Set x&64 Paths") },
        { 0x000E0028, _T("C&hange") },
        { 0x000E0029, _T("&OK") },

        // About Dialog

        { 0x000D0010, _T("About BatchEncoder") },
        { 0x000D0011, _T("&OK") },
        { 0x000D0012, _T("Licensed under the MIT license. See LICENSE file in the application root for full license information.") },

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
