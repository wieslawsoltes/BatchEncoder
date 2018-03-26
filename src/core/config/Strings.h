// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <map>
#include <vector>

namespace config
{
    const std::vector<std::wstring> m_OutpuPathsPresets =
    {
        L"$InputPath$",
        L"$InputPath$\\$Name$.$Ext$",
        L"$InputPath$\\$Name$_converted.$Ext$",
        L"$InputPath$\\$Name$$InputExt$.$Ext$",
        L"$InputPath$\\$InputName$.$Ext$",
        L"$InputPath$\\$InputName$_converted.$Ext$",
        L"$InputPath$\\$Ext$\\$Name$.$Ext$",
        L"$InputPath$\\$Ext$\\$Name$_converted.$Ext$",
        L"$InputPath$\\$Ext$\\$Name$$InputExt$.$Ext$",
        L"$InputPath$\\$Ext$\\$InputName$.$Ext$",
        L"$InputPath$\\$Ext$\\$InputName$_converted.$Ext$",
        L"$InputPath$\\Converted\\$Name$.$Ext$",
        L"$InputPath$\\Converted\\$Name$_converted.$Ext$",
        L"$InputPath$\\Converted\\$InputName$.$Ext$",
        L"$InputPath$\\Converted\\$InputName$_converted.$Ext$",
        L"$InputPath$\\Converted\\$Ext$\\$Name$.$Ext$",
        L"$InputPath$\\Converted\\$Ext$\\$Name$_converted.$Ext$",
        L"$InputPath$\\Converted\\$Ext$\\$InputName$.$Ext$",
        L"$InputPath$\\Converted\\$Ext$\\$InputName$_converted.$Ext$",
        L"C:\\Output",
        L"C:\\Output\\$Name$.$Ext$",
        L"C:\\Output\\$Name$_converted.$Ext$",
        L"C:\\Output\\$InputName$.$Ext$",
        L"C:\\Output\\$InputName$_converted.$Ext$",
        L"C:\\Output\\$InputFolder[0]$\\$Name$.$Ext$",
        L"C:\\Output\\$InputFolder[0]$\\$InputName$.$Ext$",
        L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$",
        L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$",
        L"C:\\Output\\$InputFolder[2]$\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$",
        L"C:\\Output\\$InputFolder[2]$\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$",
        L"Converted",
        L"Converted\\$Name$.$Ext$",
        L"Converted\\$Name$_converted.$Ext$",
        L"Converted\\$InputName$.$Ext$",
        L"Converted\\$InputName$_converted.$Ext$",
        L"Converted\\$InputFolder[0]$\\$Name$.$Ext$",
        L"Converted\\$InputFolder[0]$\\$InputName$.$Ext$",
        L"Converted\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$",
        L"Converted\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$",
        L"Converted\\$InputFolder[2]$\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$",
        L"Converted\\$InputFolder[2]$\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$",
        L"$InputDrive$\\$Name$.$Ext$",
        L"$InputDrive$\\$InputName$.$Ext$",
        L"$InputDrive$\\$InputDir$\\Converted\\$Name$.$Ext$",
        L"$InputDrive$\\$InputDir$\\Converted\\$InputName$.$Ext$",
        L"$InputDrive$\\Converted\\$Name$.$Ext$",
        L"$InputDrive$\\Converted\\$InputName$.$Ext$",
        L"C:\\$InputDir$\\$Name$.$Ext$",
        L"C:\\$InputDir$\\$InputName$.$Ext$",
        L"C:\\Converted\\$InputDir$\\$Name$.$Ext$",
        L"C:\\Converted\\$InputDir$\\$InputName$.$Ext$"
    };

    const std::map<int, const std::wstring> m_Strings =
    {
        // File Menu

        { 0x00010001, L"&File" },
        { 0x00010002, L"&Load List...\tCtrl+L" },
        { 0x00010003, L"&Save List...\tCtrl+S" },
        { 0x00010004, L"Cl&ear List\tCtrl+E" },
        { 0x00010005, L"E&xit\tAlt+F4" },

        // Edit Menu

        { 0x00020001, L"&Edit" },
        { 0x00020002, L"&Add Files...\tF5" },
        { 0x00020003, L"Add Dire&ctory...\tF6" },
        { 0x00020004, L"Rena&me\tF2" },
        { 0x00020005, L"Rese&t Time/Status\tF3" },
        { 0x00020006, L"Reset Output&/Preset\tF4" },
        { 0x00020007, L"&Remove\tDEL" },
        { 0x00020008, L"Cro&p\tINS" },
        { 0x00020009, L"Remove Chec&ked\tCtrl++" },
        { 0x0002000A, L"Remove Unc&hecked\tCtrl+-" },
        { 0x0002000B, L"Check S&elected\tShift++" },
        { 0x0002000C, L"&Uncheck Selected\tShift+-" },
        { 0x0002000D, L"&Select All\tCtrl+A" },
        { 0x0002000E, L"Select &None\tCtrl+N" },
        { 0x0002000F, L"&Invert Selection\tCtrl+I" },
        { 0x00020010, L"&Open File..." },
        { 0x00020011, L"Open &Directory..." },

        // Action Menu

        { 0x00030001, L"&Action" },
        { 0x00030002, L"Conve&rt\tF9" },
        { 0x00030003, L"S&top\tF9" },

        // Options Menu

        { 0x00040001, L"&Options" },
        { 0x00040002, L"Configure &Presets...\tF7" },
        { 0x00040003, L"Configure &Formats...\tF8" },
        { 0x00040004, L"&Delete Source Files" },
        { 0x00040005, L"&Shutdown When Finished" },
        { 0x00040006, L"Do Not Save Configuration On E&xit" },
        { 0x00040007, L"Delete &Output File On Error" },
        { 0x00040008, L"Stop &Conversion On Error" },
        { 0x00040009, L"&Hide Console Window" },
        { 0x0004000A, L"&Ensure Item Is Visible" },
        { 0x0004000B, L"&Try To Find Decoder" },
        { 0x0004000C, L"&Validate Input Files" },
        { 0x0004000D, L"Ove&rwrite Existing Files" },
        { 0x0004000E, L"Configure Too&ls...\tF10" },
        { 0x0004000F, L"Tr&y To Download Tools" },
        { 0x00040010, L"Rena&me Existing Files" },

        // Language Menu

        { 0x00050001, L"&Language" },
        { 0x00050002, L"&Default (English)" },

        // Help Menu

        { 0x00060001, L"&Help" },
        { 0x00060002, L"&Website..." },
        { 0x00060003, L"&About..." },

        // Main Dialog

        { 0x000A0001, L"Name" },
        { 0x000A0002, L"Input" },
        { 0x000A0003, L"Size (bytes)" },
        { 0x000A0004, L"Output" },
        { 0x000A0005, L"Preset#" },
        { 0x000A0006, L"Time" },
        { 0x000A0007, L"Status" },

        { 0x000A0010, L"BatchEncoder" },
        { 0x000A0011, L"Output" },
        { 0x000A0012, L"format:" },
        { 0x000A0013, L"preset:" },
        { 0x000A0014, L"Output path:" },
        { 0x000A0015, L"&..." },
        { 0x000A0016, L"threads (0=auto):" },
        { 0x000A0017, L"Conve&rt" },
        { 0x000A0018, L"S&top" },

        // Paths Dialog

        { 0x000F0001, L"Path" },
        { 0x000F0002, L"Size (bytes)" },

        { 0x000F0010, L"Edit Paths" },
        { 0x000F0011, L"&Cancel" },
        { 0x000F0012, L"Path:" },
        { 0x000F0013, L"Size:" },
        { 0x000F0014, L"move &Up" },
        { 0x000F0015, L"move Dow&n" },
        { 0x000F0016, L"Dele&te All" },
        { 0x000F0017, L"&Delete" },
        { 0x000F0018, L"&Add" },
        { 0x000F0019, L"&Import" },
        { 0x000F001A, L"E&xport" },
        { 0x000F001B, L"C&hange" },
        { 0x000F001C, L"&OK" },
        { 0x000F001D, L"Du&plicate" },
        { 0x000F001E, L"&..." },

        // Presets Dialog

        { 0x000B0001, L"Name" },
        { 0x000B0002, L"Options" },

        { 0x000B0010, L"Configure Presets" },
        { 0x000B0011, L"&Cancel" },
        { 0x000B0012, L"Name:" },
        { 0x000B0013, L"Command-line options:" },
        { 0x000B0014, L"move &Up" },
        { 0x000B0015, L"move Dow&n" },
        { 0x000B0016, L"Dele&te All" },
        { 0x000B0017, L"&Delete" },
        { 0x000B0018, L"&Add" },
        { 0x000B0019, L"&Import" },
        { 0x000B001A, L"E&xport" },
        { 0x000B001B, L"C&hange" },
        { 0x000B001C, L"&OK" },
        { 0x000B001D, L"Du&plicate" },

        // Formats Dialog

        { 0x000C0001, L"Name" },
        { 0x000C0002, L"Id" },
        { 0x000C0003, L"Priority" },
        { 0x000C0004, L"Template" },

        { 0x000C0010, L"Configure Formats" },
        { 0x000C0011, L"&Cancel" },
        { 0x000C0012, L"Encoder" },
        { 0x000C0013, L"Decoder" },
        { 0x000C0014, L"Type:" },
        { 0x000C0015, L"Input" },
        { 0x000C0016, L"Output" },
        { 0x000C0017, L"Pipes:" },
        { 0x000C0018, L"Id:" },
        { 0x000C0019, L"Name:" },
        { 0x000C001A, L"Output extension:" },
        { 0x000C001B, L"Input extensions (separated by comma):" },
        { 0x000C001C, L"Default preset:" },
        { 0x000C001D, L"Command-line executable path:" },
        { 0x000C001E, L"&..." },
        { 0x000C001F, L"Command-line template:" },
        { 0x000C0020, L"Progress function:" },
        { 0x000C0021, L"..." },
        { 0x000C0022, L"move &Up" },
        { 0x000C0023, L"move Dow&n" },
        { 0x000C0024, L"Dele&te All" },
        { 0x000C0025, L"&Delete" },
        { 0x000C0026, L"&Add" },
        { 0x000C0027, L"&Load" },
        { 0x000C0028, L"&Save As" },
        { 0x000C0029, L"C&hange" },
        { 0x000C002A, L"&OK" },
        { 0x000C002B, L"Exit code on success:" },
        { 0x000C002C, L"&Edit Presets" },
        { 0x000C002D, L"&Import" },
        { 0x000C002E, L"E&xport" },
        { 0x000C002F, L"Du&plicate" },
        { 0x000C0030, L"Priority:" },

        // Tools Dialog

        { 0x000E0001, L"Name" },
        { 0x000E0002, L"Priority" },
        { 0x000E0003, L"Url" },
        { 0x000E0004, L"Status" },

        { 0x000E0010, L"Configure Tools" },
        { 0x000E0011, L"&Cancel" },
        { 0x000E0012, L"Name:" },
        { 0x000E0013, L"Platform:" },
        { 0x000E0014, L"Supported formats (separated by comma):" },
        { 0x000E0015, L"Download URL:" },
        { 0x000E0016, L"Download file name:" },
        { 0x000E0017, L"Extract method:" },
        { 0x000E0018, L"Extract path:" },
        { 0x000E0019, L"move &Up" },
        { 0x000E001A, L"move Dow&n" },
        { 0x000E001B, L"&Import" },
        { 0x000E001C, L"E&xport" },
        { 0x000E001D, L"Du&plicate" },
        { 0x000E001E, L"Dele&te All" },
        { 0x000E001F, L"&Delete" },
        { 0x000E0020, L"&Add" },
        { 0x000E0021, L"&Load" },
        { 0x000E0022, L"&Save As" },
        { 0x000E0023, L"Do&wnload Selected" },
        { 0x000E0024, L"Stop Do&wnload" },
        { 0x000E0025, L"S&et Selected Paths" },
        { 0x000E0026, L"Set x&86 Paths" },
        { 0x000E0027, L"Set x&64 Paths" },
        { 0x000E0028, L"C&hange" },
        { 0x000E0029, L"&OK" },
        { 0x000E002A, L"Priority:" },

        // About Dialog

        { 0x000D0010, L"About BatchEncoder" },
        { 0x000D0011, L"&OK" },
        { 0x000D0012, L"Licensed under the MIT license. See LICENSE file in the application root for full license information." },

        // Strings Work Thread

        { 0x00110001, L"Error: can not load progress script." },
        { 0x00110002, L"Error: can not find progress function." },
        { 0x00110003, L"Error: console line is too large for read buffer." },

        { 0x00120001, L"Error: invalid format pipe configuration." },
        { 0x00120002, L"Error: can not create pipes for stderr." },
        { 0x00120003, L"Error: can not duplicate stderr pipe to prevent child process from closing the pipe." },
        { 0x00120004, L"Error: can not create command-line process." },
        { 0x00120005, L"Error: progress did not reach 100%." },
        { 0x00120006, L"Done" },

        { 0x00130001, L"Error: invalid format pipe configuration." },
        { 0x00130002, L"Error: can not create pipes for stdin." },
        { 0x00130003, L"Error: can not set stdin pipe inherit flag." },
        { 0x00130004, L"Error: can not create pipes for stdout." },
        { 0x00130005, L"Error: can not set stdout pipe inherit flag." },
        { 0x00130006, L"Error: can not create command-line process." },
        { 0x00130007, L"Error: can not create read thread." },
        { 0x00130008, L"Error: can not create write thread." },
        { 0x00130009, L"Error: progress did not reach 100%." },
        { 0x0013000A, L"Error: can not create pipes for process connection." },
        { 0x0013000B, L"Done" },
        { 0x0013000C, L"Error: can not create pipes for stderr." },
        { 0x0013000D, L"Error: can not set stderr pipe inherit flag." },
        { 0x0013000E, L"Error: can not create output thread." },

        { 0x00140001, L"Error: can not find input file." },
        { 0x00140002, L"Error: can not find valid encoder by id." },
        { 0x00140003, L"Error: can not find encoder format preset." },
        { 0x00140004, L"Error: can not find valid decoder by extension." },
        { 0x00140005, L"Error: can not find decoder format preset." },
        { 0x00140006, L"Error: decoder output not supported by encoder." },
        { 0x00140007, L"Decoding..." },
        { 0x00140008, L"Error: can not find decoded file." },
        { 0x00140009, L"Error: exception thrown while converting file." },
        { 0x0014000A, L"Encoding..." },
        { 0x0014000B, L"Decoding..." },
        { 0x0014000C, L"Transcoding..." },
        { 0x0014000D, L"Error: can not find encoded file." },
        { 0x0014000E, L"Error: exception thrown while converting file." },
        { 0x0014000F, L"Unable to create output path!" },
        { 0x00140010, L"Output file already exists." },

        { 0x00150001, L"--:--" },

        // Strings Worker Context

        { 0x00190001, L"Error" },
        { 0x00190002, L"Errors" },
        { 0x00190003, L"Processing item %d of %d (%d Done, %d %s)" },
        { 0x00190004, L"Processed %d of %d (%d Done, %d %s) in %s" },

        // Strings Main Dialog

        { 0x00210001, L"Not Done" },
        { 0x00210002, L"Item" },
        { 0x00210003, L"Items" },
        { 0x00210004, L"No Items" },
        { 0x00210005, L"Recurse subdirectories" },
        { 0x00210006, L"Output path:" },
        { 0x00210007, L"Failed to load file!" },
        { 0x00210008, L"Failed to save file!" },
        { 0x00210009, L"Failed to allocate memory for filenames buffer!" },
        { 0x0021000A, L"Select folder:" },
        { 0x0021000B, L"Error while searching for language files!" },
        { 0x0021000C, L"Error while searching for item files!" },
        { 0x0021000D, L"Unable to create output path!" },
        { 0x0021000E, L"Fatal error when creating thread!" },
        { 0x0021000F, L"Invalid output path format!" },

        // Strings Presets Dialog

        { 0x00220001, L"ERROR" },
        { 0x00220002, L"Failed to load file!" },
        { 0x00220003, L"Failed to save file!" },
        { 0x00220004, L"Default" },
        { 0x00220005, L"-copy" },

        // Strings Formats Dialog

        { 0x00230001, L"ERROR" },
        { 0x00230002, L"Failed to load file!" },
        { 0x00230003, L"Failed to save file!" },
        { 0x00230004, L"Format" },
        { 0x00230005, L"Default" },
        { 0x00230006, L"-copy" },

        // Strings Tools Dialog

        { 0x00240001, L"ERROR" },
        { 0x00240002, L"Failed to load file!" },
        { 0x00240003, L"Failed to save file!" },
        { 0x00240004, L"Tool" },
        { 0x00240005, L"-copy" },

        // Strings File Dialogs

        { 0x00310001, L"All Files" },
        { 0x00310002, L"Xml Files" },
        { 0x00310003, L"Items Files" },
        { 0x00310004, L"Presets Files" },
        { 0x00310005, L"Formats Files" },
        { 0x00310006, L"Exe Files" },
        { 0x00310007, L"Progress Files" },
        { 0x00310008, L"Format Files" },
        { 0x00310009, L"Tools Files" },
        { 0x0031000A, L"Tool Files" },
        { 0x0031000B, L"Paths Files" },

        // Strings Download Status

        { 0x00400001, L"Finding resource..." },
        { 0x00400002, L"Connecting..." },
        { 0x00400003, L"Sending request..." },
        { 0x00400004, L"Mime type available" },
        { 0x00400005, L"Cache filename available" },
        { 0x00400006, L"Begin download" },
        { 0x00400007, L"End download" },
        { 0x00400008, L"Status code : " },

        // Strings Extract Status

        { 0x00410001, L"Failed to create unzip folder." },
        { 0x00410002, L"Unziped downloaded file." },
        { 0x00410003, L"Failed to unzip downloaded file." }
    };
}
