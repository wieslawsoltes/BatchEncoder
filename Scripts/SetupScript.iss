; BatchEncoder (Audio Conversion GUI)
; Copyright (C) 2005-2017 Wieslaw Soltes <wieslaw.soltes@gmail.com>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; version 2 of the License.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

; Using:
; Inno Setup Compiler v5.1.6 (http://www.innosetup.com/)
; Inno Setup Preprocessor (ISPP) v5.1.2.0 (http://ispp.sourceforge.net/)

[Defines]

; Set program base name.
#define ProgramName "BatchEncoder"

; Set program release number.
#include "SetupVersion.inc"

; Set program copyright information.
#define ProgramCopyright "Copyright (C) 2005-2008 Wieslaw Soltes"

; Set program website url.
#define ProgramWebsite "http://www.BatchEncoder.com/"

; Base pathe for included files.
#define BasePath ".."

[Setup]

AppId               ={#ProgramName}-{#ProgramBuild}
UsePreviousAppDir   =yes
AppName             ={#ProgramName}
AppVerName          ={#ProgramName} {#ProgramVersion}
AppCopyright        ={#ProgramCopyright}
DefaultDirName      ={pf}\{#ProgramName} ({#ProgramBuild})
OutputDir           =.
OutputBaseFilename  ={#ProgramName}-{#ProgramVersion}-{#ProgramBuild}-installer
VersionInfoVersion  ={#ProgramVersion}
DefaultGroupName    ={#ProgramName} ({#ProgramBuild})
AppPublisher        ={#ProgramWebsite}
AppPublisherURL     ={#ProgramWebsite}
AppVersion          ={#ProgramVersion}
AllowNoIcons        =yes
Compression         =lzma/max
SolidCompression    =yes
WindowVisible       =no
WindowShowCaption   =no
BackColor           =clBlue
BackColor2          =clBlack
BackColorDirection  =toptobottom
BackSolid           =no
UserInfoPage        =no
LicenseFile         =SetupLicense.inc

[Components]

Name: main; Description: Main Program; Types: full compact custom
Name: config; Description: Configuration; Types: full compact
Name: presets; Description: Presets; Types: full compact
Name: formats; Description: Formats; Types: full compact
Name: progress; Description: Progress Functions; Types: full compact
Name: help; Description: Help; Types: full
Name: docs; Description: Documents; Types: full
Name: unicows; Description: UnicoWS; Types: full compact

[Tasks]

Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main; Flags: unchecked

[Files]

Source: {#BasePath}\Release\{#ProgramBuild}\BatchEncoder.exe; DestDir: {app}; Flags: ignoreversion; Components: main
Source: {#BasePath}\Config\BatchEncoder.config; DestDir: {app}; Flags: ignoreversion; Components: config
Source: {#BasePath}\Presets\*.presets; DestDir: {app}; Flags: ignoreversion; Components: presets
Source: {#BasePath}\Formats\*.formats; DestDir: {app}; Flags: ignoreversion; Components: formats
Source: {#BasePath}\Progress\Release\{#ProgramBuild}\*.progress; DestDir: {app}; Flags: ignoreversion; Components: progress
Source: {#BasePath}\Help\*.pdf; DestDir: {app}; Flags: ignoreversion; Components: help
Source: {#BasePath}\doc\*.txt; DestDir: {app}; Flags: ignoreversion; Components: docs

#ifdef ProgramBuildWin32
Source: {#BasePath}\UnicoWS\UnicoWS.dll; DestDir: {app}; Flags: ignoreversion; Components: unicows
#endif

[INI]

Filename: {app}\{#ProgramName}.url; Section: InternetShortcut; Key: URL; String: {#ProgramWebsite}

[Icons]

Name: {group}\{#ProgramName}; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Components: main
Name: {group}\{#ProgramName} License; Filename: {app}\License.txt; WorkingDir: {app}; Components: docs
Name: {group}\{#ProgramName} Help; Filename: {app}\BatchEncoder.pdf; WorkingDir: {app}; Components: help
Name: {group}\{#ProgramName} Web Site; Filename: {app}\{#ProgramName}.url; WorkingDir: {app}
Name: {group}\Uninstall {#ProgramName}; Filename: {uninstallexe}; WorkingDir: {app}
Name: {userdesktop}\{#ProgramName}; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Tasks: desktopicon; Components: main
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#ProgramName}; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Tasks: quicklaunchicon; Components: main
Name: {group}\{#ProgramName} Tools\Presets Editor; Parameters: --configure-presets; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Components: main
Name: {group}\{#ProgramName} Tools\Formats Editor; Parameters: --configure-formats; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Components: main

[Run]

Filename: {app}\BatchEncoder.exe; Description: {cm:LaunchProgram,{#ProgramName}}; WorkingDir: {app}; Flags: nowait postinstall skipifsilent unchecked; Components: main

[UninstallDelete]

Type: files; Name: {app}\{#ProgramName}.url
