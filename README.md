# BatchEncoder

[![Build status](https://ci.appveyor.com/api/projects/status/gf82ojfu97e7udx7/branch/master?svg=true)](https://ci.appveyor.com/project/wieslawsoltes/batchencoder/branch/master)

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

## License

BatchEncoder (Audio Conversion GUI)
Copyright (C) 2005-2017 Wiesław Šoltés

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
