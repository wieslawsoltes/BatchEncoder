# BatchEncoder (Audio Conversion GUI)

[![Gitter](https://badges.gitter.im/wieslawsoltes/BatchEncoder.svg)](https://gitter.im/wieslawsoltes/BatchEncoder?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

| Platform              | Configuration  | Build status                                                                                                                                                                              |
|-----------------------|----------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Debug          | [![Build status](https://ci.appveyor.com/api/projects/status/gf82ojfu97e7udx7/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/branch/master)          |
| Windows 32-bit        | Release        | [![Build status](https://ci.appveyor.com/api/projects/status/p8dg4vxqs2r5m7g4/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder-y606d/branch/master)    |
| Windows 64-bit        | Debug          | [![Build status](https://ci.appveyor.com/api/projects/status/mg4yfh8vs7y8v7hk/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder-3gv5t/branch/master)    |
| Windows 64-bit        | Release        | [![Build status](https://ci.appveyor.com/api/projects/status/kcfeuspoto5xmgsi/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder-1fq40/branch/master)    |

BatchEncoder is an audio files conversion software.

### Support BatchEncoder

[![PayPal - The safer, easier way to pay online!](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GFQMG36HPYT6W)

### Download BatchEncoder

NOTE: Version 1.0 is in the works, please download latest CI server builds.

| Platforn              | Type        | Version       | Download                                                                                                                               |
|-----------------------|-------------|---------------|----------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Installer   | 1.0           | [BatchEncoder-1.0-Win32.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-Win32.exe)           |
| Windows 64-bit        | Installer   | 1.0           | [BatchEncoder-1.0-x64.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-x64.exe)               |
| Windows 32-bit        | Portable    | 1.0           | [BatchEncoder-1.0-Win32.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-Win32.zip)           |
| Windows 64-bit        | Portable    | 1.0           | [BatchEncoder-1.0-x64.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-x64.zip)               |

You can also download bleeding edge builds from the CI server:

#### Debug Builds

| Platform              | Type        | Download                                                                                                                                                          |
|-----------------------|-------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Installer   | [BatchEncoder-1.0-Win32-(Debug).exe](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder/artifacts/artifacts/BatchEncoder-1.0-Win32-(Debug).exe)      |
| Windows 64-bit        | Installer   | [BatchEncoder-1.0-x64-(Debug).exe](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder-3gv5t/artifacts/artifacts/BatchEncoder-1.0-x64-(Debug).exe)    |
| Windows 32-bit        | Portable    | [BatchEncoder-1.0-Win32-(Debug).zip](https://ci.appveyor.com/api/projects/wieslawsoltes/BatchEncoder/artifacts/artifacts/BatchEncoder-1.0-Win32-(Debug).zip)      |
| Windows 64-bit        | Portable    | [BatchEncoder-1.0-x64-(Debug).zip](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder-3gv5t/artifacts/artifacts/BatchEncoder-1.0-x64-(Debug).zip)    |

#### Release Builds

| Platform              | Type        | Download                                                                                                                                                          |
|-----------------------|-------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Installer   | [BatchEncoder-1.0-Win32.exe](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder-y606d/artifacts/artifacts/BatchEncoder-1.0-Win32.exe)                |
| Windows 64-bit        | Installer   | [BatchEncoder-1.0-x64.exe](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder-1fq40/artifacts/artifacts/BatchEncoder-1.0-x64.exe)                    |
| Windows 64-bit        | Portable    | [BatchEncoder-1.0-x64.zip](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder-1fq40/artifacts/artifacts/BatchEncoder-1.0-x64.zip)                    |
| Windows 32-bit        | Portable    | [BatchEncoder-1.0-Win32.zip](https://ci.appveyor.com/api/projects/wieslawsoltes/batchencoder-y606d/artifacts/artifacts/BatchEncoder-1.0-Win32.zip)                |

## About

BatchEncoder is an audio files conversion software. It supports most of popular audio file formats 
including lossy and lossless compression. The program is very simple to use. 
BatchEncoder is basically GUI front-end for command-line tools. All it does is create background 
processes and pass arguments to it with options set in presets/format menu and additionally 
adds input and output file paths. For specific options for each format use help available for command-line tools.

## Features

* Graphical user interface for command-line tools.
* Supported are most popular audio formats.
* Supported file transcoding between audio formats.
* Support for multi-threaded conversion.
* Out of the box configuration for new users.
* Advanced options for experienced users.
* Open-source, fast and simple.

## Screenshots

### File list

[![File List](screenshots/FileList.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileList.png)

### Conversion progress

[![File List Converting](screenshots/FileListConverting.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileListConvertingFileList.png)

## Supported formats and tools

### Encoders

| Name            | Type        | File extension  | Version       | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|---------------|----------------------------------|-------------------------------------------|
| SSRC            | Encoder     | WAV             | -.-           | ssrc.exe                         | GetProgress_SsrcEnc.progress              |
| LAME            | Encoder     | MP3             | 3.99.5        | lame.exe                         | GetProgress_LameEnc.progress              |
| MPPENC          | Encoder     | MPC             | 1.30.0        | mppenc.exe                       | GetProgress_MppEnc.progress               |
| FAAC            | Encoder     | AAC             | 1.28-mod      | faac.exe                         | GetProgress_FaacEnc.progress              |
| NEROAACENC      | Encoder     | M4A             | 1.5.4         | neroAacEnc.exe                   | - none -                                  |
| NEROAACENC      | Encoder     | MP4             | 1.5.4         | neroAacEnc.exe                   | - none -                                  |
| OPUSENC         | Encoder     | OPUS            | 1.2-alpha     | opusenc.exe                      | - none -                                  |
| OGGENC          | Encoder     | OGG             | 2.88          | oggenc2.exe                      | GetProgress_OggEnc.progress               |
| AFTEN           | Encoder     | AC3             | git-2009122   | aften.exe                        | - none -                                  |
| FLAC            | Encoder     | FLAC            | 1.3.2         | flac.exe                         | GetProgress_FlacEnc.progress              |
| WAVPACK         | Encoder     | WV              | 5.0.0         | wavpack.exe                      | GetProgress_WavPackEnc.progress           |
| MAC             | Encoder     | APE             | 4.18          | mac.exe                          | GetProgress_MacEnc.progress               |
| OFR             | Encoder     | OFR             | 5.100         | ofr.exe                          | GetProgress_OfrEnc.progress               |
| TTAENC          | Encoder     | TTA             | -.-           | ttaenc.exe                       | GetProgress_TtaEnc.progress               |
| TAKC            | Encoder     | TAK             | 2.3.0         | takc.exe                         | - none -                                  |

### Decoders

| Name            | Type        | File extension  | Version       | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|---------------|----------------------------------|-------------------------------------------|
| LAME            | Decoder     | MP1             | -.-           | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP2             | -.-           | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP3             | -.-           | lame.exe                         | GetProgress_LameDec.progress              |
| MPPDEC          | Decoder     | MPC             | -.-           | mppdec.exe                       | GetProgress_MppDec.progress               |
| MPPDEC          | Decoder     | MPP             | -.-           | mppdec.exe                       | GetProgress_MppDec.progress               |
| MPPDEC          | Decoder     | MP+             | -.-           | mppdec.exe                       | GetProgress_MppDec.progress               |
| FAAD            | Decoder     | MP4             | -.-           | faad.exe                         | GetProgress_FaadDec.progress              |
| FAAD            | Decoder     | M4A             | -.-           | faad.exe                         | GetProgress_FaadDec.progress              |
| FAAD            | Decoder     | AAC             | -.-           | faad.exe                         | GetProgress_FaadDec.progress              |
| NEROAACDEC      | Decoder     | MP4             | -.-           | neroAacDec.exe                   | - none -                                  |
| NEROAACDEC      | Decoder     | M4A             | -.-           | neroAacDec.exe                   | - none -                                  |
| OPUSDEC         | Decoder     | OPUS            |  -.-          | opusdec.exe                      | - none -                                  |
| OGGDEC          | Decoder     | OGG             | -.-           | oggdec.exe                       | GetProgress_OggDec.progress               |
| AFTEN           | Decoder     | AC3             | -.-           | aften.exe                        | - none -                                  |
| FLAC            | Decoder     | FLAC            | -.-           | flac.exe                         | GetProgress_FlacDec.progress              |
| WVUNPACK        | Decoder     | WV              | -.-           | wvunpack.exe                     | GetProgress_WvUnpackDec.progress          |
| MAC             | Decoder     | APE             | -.-           | mac.exe                          | GetProgress_MacDec.progress               |
| MAC             | Decoder     | MAC             | -.-           | mac.exe                          | GetProgress_MacDec.progress               |
| OFR             | Decoder     | OFR             | -.-           | ofr.exe                          | GetProgress_OfrDec.progress               |
| TTAENC          | Decoder     | TTA             | -.-           | ttaenc.exe                       | GetProgress_TtaDec.progress               |
| TAKC            | Decoder     | TAK             | -.-           | takc.exe                         | - none -                                  |

### Tool downloads

For more information about tools please see the [wiki page](https://github.com/wieslawsoltes/BatchEncoder/wiki/Tools).

There is also available PowerShell [download script](https://github.com/wieslawsoltes/BatchEncoder/blob/master/scripts/download.ps1) for all supported tools.

| Name            | Downloads                                                                                          |
|-----------------|----------------------------------------------------------------------------------------------------|
| SSRC            | http://shibatch.sourceforge.net/                                                                   |
| LAME            | http://rarewares.org/mp3-lame-bundle.php                                                           |
| MPPENC          | https://musepack.net/index.php?pg=win                                                              |
| MPPDEC          | https://musepack.net/index.php?pg=win                                                              |
| FAAC            | http://rarewares.org/aac-encoders.php                                                              |
| FAAD            | http://rarewares.org/aac-decoders.php                                                              |
| OPUSENC         | https://sourceforge.net/projects/muldersoft/files/Opus%20Tools/                                    |
| OPUSDEC         | https://sourceforge.net/projects/muldersoft/files/Opus%20Tools/                                    |
| OGGENC          | http://rarewares.org/ogg-oggenc.php                                                                |
| OGGDEC          | http://rarewares.org/ogg-oggdec.php                                                                |
| AFTEN           | https://github.com/wieslawsoltes/wavtoac3encoder/releases                                          |
| FLAC            | https://xiph.org/flac/download.html                                                                |
| WAVPACK         | http://www.wavpack.com/downloads.html                                                              |
| WVUNPACK        | http://www.wavpack.com/downloads.html                                                              |
| MAC             | http://www.monkeysaudio.com/download.html                                                          |
| OFR             | http://losslessaudio.org/Downloads.php                                                             |
| TTAENC          | https://sourceforge.net/projects/tta/files/tta/ttaenc-win/                                         |
| TAKC            | http://www.thbeck.de/Tak/Tak.html                                                                  |
| NEROAACENC      | http://www.videohelp.com/software/Nero-AAC-Codec                                                   |
| NEROAACDEC      | http://www.videohelp.com/software/Nero-AAC-Codec                                                   |

## System requirements

Minimum supported version is Windows 7 SP1 or above, recommended is Windows 10 Anniversary Update.

Provided binaries should work under Windows XP Service Pack 3 (SP3) for x86, Windows XP Service Pack 2 (SP2) for x64.

Command-line tools have each different hardware and software requirements. All tools have been tested under Windows 10 Anniversary Update.

## Configuration files

* Program configuration files are used to store settings. 
* System registry is not used to store any program settings so program can be used as portable without installation. 
* Configuration is stored as XML text files with UTF-8 encoding.

Files extensions:

* .xml - Any onfiguration file
* .config - Program configuration
* .list - File-list items
* .presets - Predefined format presets
* .formats - Encoder and decoder formats
* .progress - Compiled progress function into windows DLL

Use only UTF-8 compatible text editors (e.g. Notepad) to edit xml configuration files.

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

### Default configuration files

https://github.com/wieslawsoltes/BatchEncoder/tree/master/config

### Progress function format

Progress function are compiled windows DLL libraries with exported `GetProgress` function as `C` api. The `progress` file extension is used instead of `dll`.

The progress function `C` signature:
```C
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

https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/progress

## Building BatchEncoder

To build program from sources you will need the following components:

### Microsoft Visual Studio 2015

Build `BatchEncoder.sln` solution with [Visual Studio 2015](https://www.visualstudio.com).

### Windows 7 SDK

For Windows XP compatibility program is compiled using Platform Toolset for Visual Studio 2015 – Windows XP (v140_xp).

For more details please read [Configuring Programs for Windows XP](https://msdn.microsoft.com/en-us/library/jj851139.aspx).

### TinyXml-2 Library

Required [TinyXml-2](https://github.com/leethomason/tinyxml2) sources are included in `src\xml` directory.

## License

BatchEncoder (Audio Conversion GUI) is licensed under the [MIT license](LICENSE.TXT).
