# BatchEncoder (Audio Conversion GUI)

[![Gitter](https://badges.gitter.im/wieslawsoltes/BatchEncoder.svg)](https://gitter.im/wieslawsoltes/BatchEncoder?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

[![Build status](https://ci.appveyor.com/api/projects/status/gf82ojfu97e7udx7/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/branch/master)

BatchEncoder is an audio files conversion software.

### Download BatchEncoder

| Platforn              | Type        | Version       | Download                                                                                                                               |
|-----------------------|-------------|---------------|----------------------------------------------------------------------------------------------------------------------------------------|
| Windows 32-bit        | Installer   | 1.0           | [BatchEncoder-1.0-Win32.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-Win32.exe)                      |
| Windows 64-bit        | Installer   | 1.0           | [BatchEncoder-1.0-x64.exe](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-x64.exe)          |
| Windows 32-bit        | Portable    | 1.0           | [BatchEncoder-1.0-Win32.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-Win32.zip)                      |
| Windows 64-bit        | Portable    | 1.0           | [BatchEncoder-1.0-x64.zip](https://github.com/wieslawsoltes/BatchEncoder/releases/download/1.0/BatchEncoder-1.0-x64.zip)          |

You can also download bleeding edge builds from the CI server [here](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/build/artifacts).

## About

BatchEncoder is an audio files conversion software. It supports most of popular audio file formats 
including lossy and lossless compression. The program is very simple to use. 
BatchEncoder is basically GUI front-end for command-line tools. All it does is create background 
processes and pass arguments to it with options set in presets/format menu and additionally 
adds input and output file paths. For specific options for each format use help available for command-line tools.

## Features

* Audio conversion GUI.
* Supports popular audio formats.
* Easily configurable by new users.
* Advanced options for experienced users.
* Open-source, fast and simple.

## Screenshots

### File list

[![File List](screenshots/FileList.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileList.png)

### File list conversion progress

[![File List Converting](screenshots/FileListConverting.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/FileListConvertingFileList.png)

### Graphical conversion progress

[![Progress Window](screenshots/ProgressWindow.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/ProgressWindow.png)

### Live LAME histogram

[![Histogram Window](screenshots/HistogramWindow.png)](https://github.com/wieslawsoltes/BatchEncoder/blob/master/screenshots/HistogramWindow.png)

## Supported formats and tools

| Name            | Type        | File extension  | Version     | Executable                       | Progress function                         |
|-----------------|-------------|-----------------|-------------|----------------------------------|-------------------------------------------|
| SSRC            | Encoder     | WAV             | -.-         | ssrc.exe                         | GetProgress_SsrcEnc.progress              |
| LAME            | Encoder     | MP3             | -.-         | lame.exe                         | GetProgress_LameEnc.progress              |
| LAME            | Decoder     | MP1             | -.-         | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP2             | -.-         | lame.exe                         | GetProgress_LameDec.progress              |
| LAME            | Decoder     | MP3             | -.-         | lame.exe                         | GetProgress_LameDec.progress              |
| MPPENC          | Encoder     | MPC             | -.-         | mppenc.exe                       | GetProgress_MppEnc.progress               |
| MPPDEC          | Decoder     | MPC             | -.-         | mppdec.exe                       | GetProgress_MppDec.progress               |
| MPPDEC          | Decoder     | MPP             | -.-         | mppdec.exe                       | GetProgress_MppDec.progress               |
| MPPDEC          | Decoder     | MP+             | -.-         | mppdec.exe                       | GetProgress_MppDec.progress               |
| FAAC            | Encoder     | AAC             | -.-         | faac.exe                         | GetProgress_FaacEnc.progress              |
| FAAD            | Decoder     | MP4             | -.-         | faad.exe                         | GetProgress_FaadDec.progress              |
| FAAD            | Decoder     | M4A             | -.-         | faad.exe                         | GetProgress_FaadDec.progress              |
| FAAD            | Decoder     | AAC             | -.-         | faad.exe                         | GetProgress_FaadDec.progress              |
| OGGENC          | Encoder     | OGG             | -.-         | oggenc2.exe                      | GetProgress_OggEnc.progress               |
| OGGDEC          | Decoder     | OGG             | -.-         | oggdec.exe                       | GetProgress_OggDec.progress               |
| AFTEN           | Encoder     | AC3             | -.-         | aften.exe                        | - none -                                  |
| AFTEN           | Decoder     | AC3             | -.-         | aften.exe                        | - none -                                  |
| FLAC            | Encoder     | FLAC            | -.-         | flac.exe                         | GetProgress_FlacEnc.progress              |
| FLAC            | Decoder     | FLAC            | -.-         | flac.exe                         | GetProgress_FlacDec.progress              |
| WAVPACK         | Encoder     | WV              | -.-         | wavpack.exe                      | GetProgress_WavPackEnc.progress           |
| WVUNPACK        | Decoder     | WV              | -.-         | wvunpack.exe                     | GetProgress_WvUnpackDec.progress          |
| MAC             | Encoder     | APE             | -.-         | mac.exe                          | GetProgress_MacEnc.progress               |
| MAC             | Decoder     | APE             | -.-         | mac.exe                          | GetProgress_MacDec.progress               |
| MAC             | Decoder     | MAC             | -.-         | mac.exe                          | GetProgress_MacDec.progress               |
| OFR             | Encoder     | OFR             | -.-         | ofr.exe                          | GetProgress_OfrEnc.progress               |
| OFR             | Decoder     | OFR             | -.-         | ofr.exe                          | GetProgress_OfrDec.progress               |
| TTAENC          | Encoder     | TTA             | -.-         | ttaenc.exe                       | GetProgress_TtaEnc.progress               |
| TTAENC          | Decoder     | TTA             | -.-         | ttaenc.exe                       | GetProgress_TtaDec.progress               |
| TAKC            | Encoder     | TAK             | -.-         | takc.exe                         | - none -                                  |
| TAKC            | Decoder     | TAK             | -.-         | takc.exe                         | - none -                                  |
| NEROAACENC      | Encoder     | M4A             | -.-         | neroAacEnc.exe                   | - none -                                  |
| NEROAACENC      | Encoder     | MP4             | -.-         | neroAacEnc.exe                   | - none -                                  |
| NEROAACENC      | Encoder     | M4A             | -.-         | neroAacEnc_sse2.exe              | - none -                                  |
| NEROAACENC      | Encoder     | MP4             | -.-         | neroAacEnc_sse2.exe              | - none -                                  |
| NEROAACDEC      | Decoder     | MP4             | -.-         | neroAacDec.exe                   | - none -                                  |
| NEROAACDEC      | Decoder     | M4A             | -.-         | neroAacDec.exe                   | - none -                                  |

### Tool downloads

| Name            | Downloads                                                                                          |
|-----------------|----------------------------------------------------------------------------------------------------|
| SSRC            | http://shibatch.sourceforge.net/                                                                   |
| LAME            | http://rarewares.org/mp3-lame-bundle.php                                                           |
| MPPENC          | https://musepack.net/index.php?pg=win                                                              |
| MPPDEC          | https://musepack.net/index.php?pg=win                                                              |
| FAAC            | http://rarewares.org/aac-encoders.php                                                              |
| FAAD            | http://rarewares.org/aac-decoders.php                                                              |
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

## Command-Line

    USAGE: BatchEncoder
           Default GUI interface.
    
    USAGE: BatchEncoder [SETTINGS] [OPTIONS] [FILES]
           Default GUI interface with additional command-line options.
    
    USAGE: BatchEncoder [TOOLS]
           GUI stand-alone tools.
    
    SETTINGS:
    
    --settings <[path]filename>         Set main program settings file.
                                        <[path]filename> = configuration filename
    --scf, --current-format <format>    Set current output format.
    --scp, --current-preset <preset>    Set current output preset.
    --ldp, --load-presets <[path]filename>
                                        Load presets configuration from file.
    --ldf, --load-formats <[path]filename>
                                        Load formats configuration from file.
    
    OPTIONS:
    
    --stc, --start-conversion           Start conversion when loaded all settings.
    --spc, --stop-conversion            Works only with --single-instance switch.
    -i, --si, --single-instance         Run only one instance of a program.
    -m, --min, --minimize               Minimize main window to taskbar.
    -t, --pt, --progress-type <type>    Conversion rogress type. Default: normal
                                        <type> = normal (default progressbar)
                                        <type> = extended (extended progress window)
                                        <type> = histogram (histogram window)
                                        <type> = console (forced console window)
    --sd, --shutdown <mode>             Shutdown system or exit the program.
                                        <mode> = now (only shutdown the system)
                                        <mode> = done (when conversion is done)
                                        <mode> = exit (close the program)
                                        Default: exit
    --soe, --stop-on-error [on|off]     Stop conversion process on first error.
                                        Default: off
    --doe, --delete-on-error [on|off]   Delete output file on error. Default: on
    --ti, --tray-icon [on|off]          Show icon in system tray. Default: off
    --ss, --save-settings [on|off]      Save settings on exit. Default: on
    --pt, --priority-thread <priority>  Main program thread priority
                                        <priority> = from 0 to 6
    --pp, --priority-process <priority> Main program process priority.
                                        <priority> = from 0 to 3
    --ptt, --priority-thread-tool <priority>
                                        Encoder/Decoder thread priority.
                                        <priority> = from 0 to 6
    --ppt, --priority-process-tool <priority>
                                        Encoder/Decoder process priority.
                                        <priority> = from 0 to 3
    --lco, --log-console-output [on|off]
                                        Log console output to file. Default: off
    --lf, --log-file <[path]filename>   Console output is saved in this file.
                                        Default: console.log
    --lff, --log-file-format <format>   Logfile data encoding. Default: 2
                                        <format> = 0 (ANSI)
                                        <format> = 1 (UNICODE)
                                        <format> = 2 (UTF-8)
    
    FILES:
    
    -r, --rd, --recurse-dirs [on|off]   Recurse subdirectoris when adding files.
                                        Default: on
    -f, --af, --add-file <[path]filename> <format> <preset>
                                        Add one file to filelist.
                                        <format> Output format.
                                        <preset> Output preset.
                                        Use X for current format or format.
    -d, --ad, --add-dir <path> <format> <preset>
                                        Add files from specified path.
                                        <path> Full or relative directory path.
                                        <format> Output format.
                                        <preset> Output preset.
                                        Use X for current format or format.
    -l, --ll, --load-list <[path]filename>
                                        Load file-list from configuration file.
                                        You can load multiple file lists,
                                        using this switch more then once,
                                        because the list is not erased 
                                        before loading new one.
    -s, --sl, --save-list <[path]filename>
                                        Save file-list to configuration file.
                                        If this option is used no conversion
                                        process is created. Files are added to list
                                        and configuration processed. File list is 
                                        saved to specified file and program exits.
    -b, --bf, --batch-file <[path]filename>
                                        Create batch processing file (.bat or .cmd).
                                        If this option is used no conversion
                                        process is created. Files are added to list
                                        and configuration processed. Batch-file is 
                                        saved to specified file and program exits.
    -o, --op, --output-path <path>      Path where the output files are stored.
                                        By default is used same as source file path.
    
    TOOLS:
    
    --cp, --configure-presets [<[path]filename>]
                                        Configure presets in stand-alone editor.
                                        <[path]filename> Default filename.
    --cf, --configure-formats [<[path]filename>]
                                        Configure formats in stand-alone editor.
                                        <[path]filename> Default filename.
    -c, --copyfile <[path]src-filename> <[path]dst-filename>]
                                        Copy one file.
                                        <[path]src-filename> Source filename.
                                        <[path]dst-filename> Destination filename.
    -w, --website                       Open in default browser program website.
    -a, --about                         Show program about dialog.
    -h, -?, --help                      Show program help (html format).
    
    When 'on' or 'off' is not specified the 'on' is used if not declared else.

## System requirements

Program requires Windows 7 SP1 or above. Check dependencies of used command-line encoders 
and decoders for specific information about hardware and sofrware requirements.

## Configuration files extensions

Program configuration files are used to store all settings. System registry is not used to store program settings. Only XML text files with UTF-8 encoding are supported.

Configuration files extensions:

    .xml		- All configurations files
    .config		- Main program configuration
    .list		- File-list entry's
    .presets	- Preset names and format options
    .formats	- Encoders and decoders format specification
    .progress   - Compiled progress function DLL

Please use only UTF-8 text editors to edit any configuration files. There is no description of format 
of configuration files. The file format is self explaining and most of time it is automatically 
generated by the program. If you really need to know more check the program sources to find how the things are done.

### Default configuration

https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/data/Config

### Default format definitions

https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/data/Formats

### Default [reset definitions

https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/data/Presets

### Default progress functions projects

https://github.com/wieslawsoltes/BatchEncoder/tree/master/src/progress

## Developers

To build program from sources you will need the following components:

### Microsoft Visual Studio 2015

Build `BatchEncoder.sln` solution with [Visual Studio 2015](https://www.visualstudio.com).

### TinyXml-2 Library

Required [TinyXml-2](https://github.com/leethomason/tinyxml2) sources are included in `src\xml` directory.

## License

BatchEncoder (Audio Conversion GUI) is licensed under the [MIT license](LICENSE.TXT).
