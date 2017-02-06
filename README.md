﻿# BatchEncoder (Audio Conversion GUI)

[![Gitter](https://badges.gitter.im/wieslawsoltes/BatchEncoder.svg)](https://gitter.im/wieslawsoltes/BatchEncoder?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

[![Build status](https://ci.appveyor.com/api/projects/status/gf82ojfu97e7udx7/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/branch/master)
[![Github All Releases](https://img.shields.io/github/downloads/wieslawsoltes/batchencoder/total.svg)](https://github.com/wieslawsoltes/BatchEncoder/releases)
[![GitHub Release](https://img.shields.io/github/release/wieslawsoltes/batchencoder.svg)](https://github.com/wieslawsoltes/batchencoder/releases/latest)
[![Github Releases](https://img.shields.io/github/downloads/wieslawsoltes/batchencoder/latest/total.svg)](https://github.com/wieslawsoltes/BatchEncoder/releases)

BatchEncoder is an audio files conversion software.

### Support BatchEncoder

[![PayPal - The safer, easier way to pay online!](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GFQMG36HPYT6W)

### Download BatchEncoder

| Platform              | Type        | Version       | Download                                                                                                                               |
|-----------------------|-------------|---------------|----------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Installer   | 1.1           | [BatchEncoder-1.1-Win32.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/BatchEncoder-1.1-Win32.exe)           |
| Windows 64-bit        | Installer   | 1.1           | [BatchEncoder-1.1-x64.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/BatchEncoder-1.1-x64.exe)               |
| Windows 32-bit        | Portable    | 1.1           | [BatchEncoder-1.1-Win32.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/BatchEncoder-1.1-Win32.zip)           |
| Windows 64-bit        | Portable    | 1.1           | [BatchEncoder-1.1-x64.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/BatchEncoder-1.1-x64.zip)               |

### Mirrors

[![Softpedia Download Mirror](http://www.softpedia.com/_img/softpedia_100_free.png)](http://www.softpedia.com/get/Multimedia/Audio/Audio-CD-Rippers-Encoders/BatchEncoder.shtml)

### CI Builds

[Download](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/build/artifacts) bleeding edge builds from the CI server.

### Download LanguageEditor

| Platform                   | Type        | Version       | Download                                                                                                                               |
|----------------------------|-------------|---------------|----------------------------------------------------------------------------------------------------------------------------------------|
| Windows .NET4.5 (Any CPU)  | Portable    | 1.0.0         | [LanguageEditor-1.0.0-AnyCPU.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/LanguageEditor-1.0.0-AnyCPU.zip) |
| Windows .NET4.5 (32-bit)   | Portable    | 1.0.0         | [LanguageEditor-1.0.0-x86.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/LanguageEditor-1.0.0-x86.zip)       |
| Windows .NET4.5 (64-bit)   | Portable    | 1.0.0         | [LanguageEditor-1.0.0-x64.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.1/LanguageEditor-1.0.0-x64.zip)       |

## About

BatchEncoder is an audio files conversion software. It supports most of popular audio file formats 
including lossy and lossless compression. The program is very simple to use. 
BatchEncoder is basically GUI front-end for command-line tools. All it does is create background 
processes and pass arguments to it with options set in presets/format menu and additionally 
adds input and output file paths. For specific options for each format use help available for command-line tools.

## Features

* Front-end for command-line tools.
* Supports popular audio formats.
* File transcoding between audio formats.
* Multi-threaded conversion.
* Full UNICODE support.
* Multi-language user interface.
* Out of the box configuration for new users.
* Advanced options for experienced users.
* Open-source, fast and simple.

## Screenshots

### File list

[![File List](screenshots/FileList.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileList.png)

### Conversion progress

[![File List Converting](screenshots/FileListConverting.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileListConverting.png)

### Configure formats

[![Configure Formats](screenshots/ConfigureFormats.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/ConfigureFormats.png)

### Configure presets

[![Configure Presets](screenshots/ConfigurePresets.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/ConfigurePresets.png)

## Supported formats and tools

### Encoders

| Name            | Type        | File extension  | Version         | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|-----------------|----------------------------------|-------------------------------------------|
| SSRC            | Encoder     | WAV             | 1.33            | ssrc.exe                         | GetProgress_SsrcEnc.progress              |
| LAME            | Encoder     | MP3             | 3.99.5          | lame.exe                         | GetProgress_LameEnc.progress              |
| MPPENC          | Encoder     | MPC             | 1.30.0          | mppenc.exe                       | GetProgress_MppEnc.progress               |
| FAAC            | Encoder     | AAC             | 1.28-mod        | faac.exe                         | GetProgress_FaacEnc.progress              |
| NEROAACENC      | Encoder     | M4A             | 1.5.4           | neroAacEnc.exe                   | - none -                                  |
| NEROAACENC      | Encoder     | MP4             | 1.5.4           | neroAacEnc.exe                   | - none -                                  |
| OPUSENC         | Encoder     | OPUS            | 1.2-alpha       | opusenc.exe                      | - none -                                  |
| OGGENC          | Encoder     | OGG             | 2.88            | oggenc2.exe                      | GetProgress_OggEnc.progress               |
| AFTEN           | Encoder     | AC3             | git-2009122     | aften.exe                        | - none -                                  |
| FFMPEG          | Encoder     | AC3             | 3.2.2           | ffmpeg.exe                       | - none -                                  |
| FLAC            | Encoder     | FLAC            | 1.3.2           | flac.exe                         | GetProgress_FlacEnc.progress              |
| WAVPACK         | Encoder     | WV              | 5.0.0           | wavpack.exe                      | GetProgress_WavPackEnc.progress           |
| QAAC            | Encoder     | M4A             | 2.62            | qaac.exe                         | - none -                                  |
| MAC             | Encoder     | APE             | 4.18            | mac.exe                          | GetProgress_MacEnc.progress               |
| OFR             | Encoder     | OFR             | 5.100           | ofr.exe                          | GetProgress_OfrEnc.progress               |
| TTA             | Encoder     | TTA             | 2.3             | tta.exe                          | GetProgress_TtaEnc.progress               |
| TAKC            | Encoder     | TAK             | 2.3.0           | takc.exe                         | - none -                                  |

### Decoders

| Name            | Type        | File extension  | Version         | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|-----------------|----------------------------------|-------------------------------------------|
| LAME            | Decoder     | MP1             | 3.99.5          | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP2             | 3.99.5          | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP3             | 3.99.5          | lame.exe                         | GetProgress_LameDec.progress              |
| MPCDEC          | Decoder     | MPC             | 1.0             | mpcdec.exe                       | GetProgress_MppDec.progress               |
| MPCDEC          | Decoder     | MPP             | 1.0             | mpcdec.exe                       | GetProgress_MppDec.progress               |
| MPCDEC          | Decoder     | MP+             | 1.0             | mpcdec.exe                       | GetProgress_MppDec.progress               |
| FAAD            | Decoder     | AAC             | faad2-20100614  | faad.exe                         | GetProgress_FaadDec.progress              |
| NEROAACDEC      | Decoder     | MP4             | 1.5.1           | neroAacDec.exe                   | - none -                                  |
| NEROAACDEC      | Decoder     | M4A             | 1.5.1           | neroAacDec.exe                   | - none -                                  |
| OPUSDEC         | Decoder     | OPUS            | 1.2-alpha       | opusdec.exe                      | - none -                                  |
| OGGDEC          | Decoder     | OGG             | 1.10.1          | oggdec.exe                       | GetProgress_OggDec.progress               |
| FLAC            | Decoder     | FLAC            | 1.3.2           | flac.exe                         | GetProgress_FlacDec.progress              |
| WVUNPACK        | Decoder     | WV              | 5.0.0           | wvunpack.exe                     | GetProgress_WvUnpackDec.progress          |
| MAC             | Decoder     | APE             | 4.18            | mac.exe                          | GetProgress_MacDec.progress               |
| MAC             | Decoder     | MAC             | 4.18            | mac.exe                          | GetProgress_MacDec.progress               |
| OFR             | Decoder     | OFR             | 5.100           | ofr.exe                          | GetProgress_OfrDec.progress               |
| TTA             | Decoder     | TTA             | 2.3             | tta.exe                          | GetProgress_TtaDec.progress               |
| TAKC            | Decoder     | TAK             | 2.3.0           | takc.exe                         | - none -                                  |
| FFMPEG          | Decoder     | AC3             | 3.2.2           | ffmpeg.exe                       | - none -                                  |
| FFMPEG          | Decoder     | EAC3            | 3.2.2           | ffmpeg.exe                       | - none -                                  |
| FFMPEG          | Decoder     | DTS             | 3.2.2           | ffmpeg.exe                       | - none -                                  |
| FFMPEG          | Decoder     | DTSHD           | 3.2.2           | ffmpeg.exe                       | - none -                                  |
| FFMPEG          | Decoder     | TRUEHD          | 3.2.2           | ffmpeg.exe                       | - none -                                  |

### Tool downloads

For more information about tools please see the [wiki page](https://github.com/wieslawsoltes/BatchEncoder/wiki/Tools).

There is also available PowerShell [download script](https://github.com/wieslawsoltes/BatchEncoder/blob/master/scripts/download.ps1) for all supported tools.

| Name            | Downloads                                                                                                                           |
|-----------------|-------------------------------------------------------------------------------------------------------------------------------------|
| SSRC            | [http://shibatch.sourceforge.net/](http://shibatch.sourceforge.net/)                                                                |
| LAME            | [http://rarewares.org/mp3-lame-bundle.php](http://rarewares.org/mp3-lame-bundle.php)                                                |
| MPPENC          | [https://musepack.net/index.php?pg=win](https://musepack.net/index.php?pg=win)                                                      |
| MPCDEC          | [https://musepack.net/index.php?pg=win](https://musepack.net/index.php?pg=win)                                                      |
| FAAC            | [http://rarewares.org/aac-encoders.php](http://rarewares.org/aac-encoders.php)                                                      |
| FAAD            | [http://rarewares.org/aac-decoders.php](http://rarewares.org/aac-decoders.php)                                                      |
| OPUSENC         | [https://sourceforge.net/projects/muldersoft/files/Opus%20Tools/](https://sourceforge.net/projects/muldersoft/files/Opus%20Tools/)  |
| OPUSDEC         | [https://sourceforge.net/projects/muldersoft/files/Opus%20Tools/](https://sourceforge.net/projects/muldersoft/files/Opus%20Tools/)  |
| OGGENC          | [http://rarewares.org/ogg-oggenc.php](http://rarewares.org/ogg-oggenc.php)                                                          |
| OGGDEC          | [http://rarewares.org/ogg-oggdec.php](http://rarewares.org/ogg-oggdec.php)                                                          |
| AFTEN           | [https://github.com/wieslawsoltes/wavtoac3encoder/releases](https://github.com/wieslawsoltes/wavtoac3encoder/releases)              |
| FLAC            | [https://xiph.org/flac/download.html](https://xiph.org/flac/download.html)                                                          |
| WAVPACK         | [http://www.wavpack.com/downloads.html](http://www.wavpack.com/downloads.html)                                                      |
| WVUNPACK        | [http://www.wavpack.com/downloads.html](http://www.wavpack.com/downloads.html)                                                      |
| QAAC            | [https://sites.google.com/site/qaacpage/cabinet](https://sites.google.com/site/qaacpage/cabinet)                                    |
| MAC             | [http://www.monkeysaudio.com/download.html](http://www.monkeysaudio.com/download.html)                                              |
| OFR             | [http://losslessaudio.org/Downloads.php](http://losslessaudio.org/Downloads.php)                                                    |
| TTA             | [https://sourceforge.net/projects/tta/files/tta/ttaenc-win/](https://sourceforge.net/projects/tta/files/tta/ttaenc-win/)            |
| TAKC            | [http://www.thbeck.de/Tak/Tak.html](http://www.thbeck.de/Tak/Tak.html)                                                              |
| NEROAACENC      | [http://www.videohelp.com/software/Nero-AAC-Codec](http://www.videohelp.com/software/Nero-AAC-Codec)                                |
| NEROAACDEC      | [http://www.videohelp.com/software/Nero-AAC-Codec](http://www.videohelp.com/software/Nero-AAC-Codec)                                |
| FFMPEG          | [https://ffmpeg.zeranoe.com/builds/](https://ffmpeg.zeranoe.com/builds/)                                                            |

## System requirements

Minimum supported Windows version is Windows 7 SP1 or above, recommended is Windows 10 Anniversary Update.

Provided binaries should work under Windows XP Service Pack 3 (SP3) for x86, Windows XP Service Pack 2 (SP2) for x64.

Minimum supported Linux version is Ubutnu 16.10 (using Wine 2.0).

Command-line tools may have different hardware and software requirements. All tools have been tested under Windows 10 Anniversary Update.

## Configuration files

* Program configuration files are used to store settings. 
* System registry is not used to store any program settings so program can be used as portable without installation. 
* Configuration is stored as XML text files with UTF-8 encoding.

Files extensions:

* .xml - Any configuration file
* .options - Program options
* .formats - Encoder and decoder formats
* .presets - Predefined format presets
* .items - Input file items
* .progress - Compiled progress function into windows DLL
* .language - Language translation strings

Use only UTF-8 compatible text editors (e.g. Notepad) to edit xml configuration files.

### Default configuration files

The default configuration files can be found [here](https://github.com/wieslawsoltes/BatchEncoder/tree/master/config).

## Translations

* [English (en-US)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/lang/en-US.language) by Wiesław Šoltés
* [Polish (pl-PL)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/lang/pl-PL.language) by Wiesław Šoltés

Use only UTF-8 compatible text editors (e.g. Notepad or Visual Studio Code) to edit xml language files
and [use special characters in XML](https://www.dvteclipse.com/documentation/svlinter/How_to_use_special_characters_in_XML.3F.html#gsc.tab=0).

Language files have to be placed in the same directory as the `BatchEncoder.exe` executable to be loaded automatically.

Language files can also be edited in dedicated `LanguageEditor` program. LanguageEditor requires .NET Framework 4.5 and Windows 7 SP1 or above.

### Default language files

The default language files can be found [here](https://github.com/wieslawsoltes/BatchEncoder/tree/master/lang).

## Format specification

### Template format

| Variable        | Description                                                                                        |
|-----------------|----------------------------------------------------------------------------------------------------|
| $EXE            | Executable path replaced with format `path` attribute.                                             |
| $OPTIONS        | Executable command-line options replaced with preset `options` attribute.                          |
| $INFILE         | Input file path (if pipes are supported `-` is automatically used).                                |
| $OUTFILE        | Output file path (if pipes are supported `-` is automatically used).                               |

The output file name is generated from the combination of output path, item `name` attribute and output format `extension` attribute.

Default template format order is `$EXE $OPTIONS $INFILE $OUTFILE`. 

You can also add custom options (additional command-line parameters) and text inside the `template` string.

### Type, input and output extensions

BatchEncoder does support file trans-coding. To enable this you have to configure proper `Decoder` formats.

BatchEncoder will chech input file extensions and if `Encoder` format `Input extensions` (multiple extension are supported,
separated by comma `,` e.g.: WAV,WV,CAF,W64,DFF,DSF, but only one `Output extension` e.g. WV) does not match the input file extension 
than BatchEncoder will search for `Decoder` that does support this input file extension. 

### Pipes configuration

Pipes configuration provides information to BatchEncoder whether the command-line tool has support 
to read/write data from/to stdin/stdout pipes.

* The input pipe means that encoder/decoder can directly read dat  from stdin and does not need input file name.
* The output pipe means that encoder/decoder can directly write data to stdout and does not need output file name.
* When encoder/decoder supports input pipe BatchEncoder reads file data and streams this data into encoder/decoder input pipe (stdin).
* When encoder/decoder supports output pipe BatchEncoder writes data to a file using encoder/decoder output pipe stream (stdout).
* Usually you can specify pipe support in command-line options by using "-" instead of input/output file name.

The input pipes support allows BatchEncoder to get current progress status by counting read bytes, 
instead of relying on console text output (this is done by reading encoder/decoder stderr pipe output). 

To get console output BatchEncoder is parsing text read from stderr pipe and uses provided GetProgress function
for specific tool. There are cases when stderr output is delayed by tool (stderr pipe buffer is not flushed)
and resulting in progress status can be delay.

Also when you want to do more complicated operations like transscoding you can for example connect pipes 
between one encoder output (stdout) and others decoder input (stdin). This case is not yet support by BatchEncoder.

The best case scenario is when encoder/decoder tool does support both input and output pipes, this gives BatchEncoder more 
control over encoding/decoding and trans-coding processes, also in case of trans-coding you do not need temporary files 
as data can be streamed between tools directly without temporary storage and in result you can get much better performance.

Unfortunately not all command-line tools provide support for pipes, some only support stdin pipes (mainly encoders) 
and some support only stdout pipes (mainly decoders). In this cases BatchEncoder depending on pipes input/output 
and progress function configuration selects best possible option to do conversion process. 

Sometimes you will not get nice progress during conversion, nonetheless you will successfully convert files.

### Progress function format

Progress function are compiled windows DLL libraries with exported `GetProgress` function as `C` api. The `progress` file extension is used instead of `dll`.

The progress function C signature:

```
int GetProgress(char *szLineBuff, int nLineLen);
```

The example of the progress function export definition:

```
LIBRARY GetProgress.dll
EXPORTS

GetProgress             @1
```

For a quick start use [GetProgress_Null](https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/progress/Null) VC++ project as a template.

### Default progress functions

The default progress functions sources can be found [here](https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/progress).

## BatchEncoder Sources

Sources are available in the [git source code repository](https://github.com/wieslawsoltes/BatchEncoder.git).

## Building BatchEncoder

To build program from sources you will need the following components:

### Microsoft Visual Studio 2015

For building `BatchEncoder.sln` solution use [Visual Studio 2015](https://www.visualstudio.com).

### Windows 7 SDK

For Windows XP compatibility program is compiled using Platform Toolset for Visual Studio 2015 – Windows XP (v140_xp).

For more details please read [Configuring Programs for Windows XP](https://msdn.microsoft.com/en-us/library/jj851139.aspx).

### TinyXml-2 Library

Required [TinyXml-2](https://github.com/leethomason/tinyxml2) sources are included in `src\xml` directory.

## License

BatchEncoder (Audio Conversion GUI) is licensed under the [MIT license](LICENSE.TXT).
