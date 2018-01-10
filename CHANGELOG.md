# Changelog

### Version 2.3

- Added refalac support.
- Updated rewavi tool.
- Added $OUTPATH variable to format template.
- Added support for split multi-channel WAV files.

### Version 2.2

- Added AvsDec tool to support .avs Avisynth scripts.
- Added rewavi tool to support .avs Avisynth scripts.
- Added rewavi tool to support .avi files.
- Added .mka decoder support using ffmpeg.
- Added Japanese translation.

### Version 2.1

- Updated tools download urls.

### Version 2.0

- Added configure tools dialog.
- Added dynamic layout support for dialogs.
- Added multi-selection support for presets list.
- Added multi-selection support for formats list.
- Refactored configuration file storage.
- Installed version config is stored in %appdata%\BatchEncoder path.
- Portable version config is stored in .\BatchEncoder path.
- Installed version tools are stored in %appdata%\BatchEncoder\tools path.
- Portable version tools are stored in .\BatchEncoder\tools path.
- Refactored worker threads to use lambdas.
- Refactored common code as submodules.
- Refactored xml configuration code.
- Added eac3to support.
- Updated tools to latest versions.
- Updated QAAC AAC format to use --no-optimize instead of --adts.
- Updated TinyXml-2 to version 5.0.1.

### Version 1.4

- Updated Spanish translation.
- Updated AAC presets to show birate for VBR.
- Updated projects to Microsoft Visual Studio 2017 (PlatformToolset v141_xp).
- Changed projects WindowsTargetPlatformVersion to version 7.0.

### Version 1.3

- Added advanced output path options.
- Added Validate Input Files option.
- Added overwrite existing files option.
- Updated tools download.ps1 script.
- Variable names in format path are case insensitive.
- Variable names in output path are case insensitive.
- Added ffdcaenc DTS encoder format.

### Version 1.2

- Changed progress function format to use Lua script files.
- Added La Lossless Audio compression format.
- Added qaac AAC encoder format.
- Added qaac AAC decoder format.
- Added qaac M4A decoder format.
- Added individual format definitions files.
- Added pipes only trans-coding support (no temp file created if pipes are supported).
- Added hide console window option.
- Added drag and drop support for options files (main dialog).
- Added drag and drop support for items files (main dialog).
- Added drag and drop support for formats files (main and formats dialog).
- Added drag and drop support for format files (main and formats dialog).
- Added drag and drop support for presets files (main, formats and presets dialog).
- Added drag and drop support for exe files (main and formats dialog, set current format exe path).
- Added drag and drop support for progress files (main and formats dialog, set current format progress path).
- Added drag and drop support for language files (main dialog).
- Added option to open and edit presets from Configure Formats dialog.
- Added import, export and duplicate format buttons.
- Added duplicate preset button.
- Added Korean translation by JaeHyung Lee.
- Added Spanish translation by Overdrive80.
- Optimized compiler and linker settings.
- Update status-bar when changing language.
- Sorted formats in alphabetical order in the drop-down.
- Use unique file name for temporary file when trans-coding.
- Bug fixes.

### Version 1.1

- Added multi-language support.
- Added Polish language translation.
- Added ffmpeg ac3 encoder support.
- Added language editor tool.

### Version 1.0

- First public release.
