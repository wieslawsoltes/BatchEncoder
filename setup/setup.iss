#define ProgramName "BatchEncoder"
#define ProgramConfiguration CONFIGURATION
#define ProgramBuild BUILD
#define ProgramVersion VERSION
#define ProgramCopyright "Copyright (c) Wieslaw Soltes. All rights reserved."
#define ProgramWebsite "https://github.com/wieslawsoltes/BatchEncoder"
#define BasePath ".."

[Setup]

AppId               ={#ProgramName}-{#ProgramBuild}
AppName             ={#ProgramName}
AppVerName          ={#ProgramName} {#ProgramVersion}
AppCopyright        ={#ProgramCopyright}
DefaultDirName      ={pf}\{#ProgramName} ({#ProgramBuild})
OutputDir           ={#BasePath}\artifacts
VersionInfoVersion  ={#ProgramVersion}
DefaultGroupName    ={#ProgramName} ({#ProgramBuild})
AppPublisher        ={#ProgramWebsite}
AppPublisherURL     ={#ProgramWebsite}
AppVersion          ={#ProgramVersion}
UsePreviousAppDir   =yes
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

#if "Release" == ProgramConfiguration
    OutputBaseFilename ={#ProgramName}-{#ProgramVersion}-{#ProgramBuild}
#else
    OutputBaseFilename ={#ProgramName}-{#ProgramVersion}-{#ProgramBuild}-({#ProgramConfiguration})
#endif

#if "x64" == ProgramBuild
    ArchitecturesInstallIn64BitMode=x64
#endif

[Components]

Name: main; Description: Main Program; Types: full compact custom
Name: docs; Description: Documents; Types: full
Name: config; Description: Configuration; Types: full compact
Name: progress; Description: Progress Functions; Types: full compact
Name: scripts; Description: Scripts; Types: full

[Tasks]

Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main; Flags: unchecked

[Files]

Source: {#BasePath}\src\bin\{#ProgramConfiguration}\{#ProgramBuild}\BatchEncoder.exe; DestDir: {app}; Flags: ignoreversion; Components: main
Source: {#BasePath}\*.txt; DestDir: {app}; Flags: ignoreversion; Components: docs
Source: {#BasePath}\*.md; DestDir: {app}; Flags: ignoreversion; Components: docs
Source: {#BasePath}\config\*.options; DestDir: {app}; Flags: ignoreversion; Components: config
Source: {#BasePath}\config\*.formats; DestDir: {app}; Flags: ignoreversion; Components: config
Source: {#BasePath}\config\*.items; DestDir: {app}; Flags: ignoreversion; Components: config
Source: {#BasePath}\src\bin\{#ProgramConfiguration}\{#ProgramBuild}\*.progress; DestDir: {app}; Flags: ignoreversion; Components: progress
Source: {#BasePath}\scripts\*.ps1; DestDir: {app}; Flags: ignoreversion; Components: scripts

[INI]

Filename: {app}\{#ProgramName}.url; Section: InternetShortcut; Key: URL; String: {#ProgramWebsite}

[Icons]

Name: {group}\{#ProgramName}; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Components: main
Name: {group}\{#ProgramName} README; Filename: {app}\README.MD; WorkingDir: {app}; Components: docs
Name: {group}\{#ProgramName} License; Filename: {app}\LICENSE.TXT; WorkingDir: {app}; Components: docs
Name: {group}\{#ProgramName} Web Site; Filename: {app}\{#ProgramName}.url; WorkingDir: {app}
Name: {group}\Uninstall {#ProgramName}; Filename: {uninstallexe}; WorkingDir: {app}
Name: {userdesktop}\{#ProgramName}; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Tasks: desktopicon; Components: main
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#ProgramName}; Filename: {app}\BatchEncoder.exe; WorkingDir: {app}; Tasks: quicklaunchicon; Components: main

[Run]

Filename: {app}\BatchEncoder.exe; Description: {cm:LaunchProgram,{#ProgramName}}; WorkingDir: {app}; Flags: nowait postinstall skipifsilent unchecked; Components: main

[UninstallDelete]

Type: files; Name: {app}\{#ProgramName}.url
