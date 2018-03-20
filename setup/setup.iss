#define ProgramName "BatchEncoder"
#define ProgramExe "BatchEncoder.exe"
#define ProgramConfiguration CONFIGURATION
#define ProgramBuild BUILD
#define ProgramVersion VERSION
#define ProgramVersionSuffix SUFFIX
#define ProgramCopyright "Copyright (c) Wieslaw Soltes. All rights reserved."
#define ProgramWebsite "https://github.com/wieslawsoltes/BatchEncoder"
#define BasePath ".."

[Setup]

AppId               ={#ProgramName}-{#ProgramBuild}
AppName             ={#ProgramName}
AppVerName          ={#ProgramName} {#ProgramVersion}{#ProgramVersionSuffix}
AppCopyright        ={#ProgramCopyright}
DefaultDirName      ={pf}\{#ProgramName} ({#ProgramBuild})
OutputDir           ={#BasePath}\artifacts
VersionInfoVersion  ={#ProgramVersion}
DefaultGroupName    ={#ProgramName} ({#ProgramBuild})
AppPublisher        ={#ProgramWebsite}
AppPublisherURL     ={#ProgramWebsite}
AppVersion          ={#ProgramVersion}{#ProgramVersionSuffix}
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
    OutputBaseFilename ={#ProgramName}-{#ProgramVersion}{#ProgramVersionSuffix}-{#ProgramBuild}
#else
    OutputBaseFilename ={#ProgramName}-{#ProgramVersion}{#ProgramVersionSuffix}-{#ProgramBuild}-({#ProgramConfiguration})
#endif

#if "x64" == ProgramBuild
    ArchitecturesInstallIn64BitMode=x64
#endif

[Components]

Name: main; Description: Main Program; Types: full compact custom
Name: docs; Description: Documents; Types: full
Name: formats; Description: Format Definitions; Types: full compact
Name: lang; Description: Languages; Types: full
Name: progress; Description: Progress Functions; Types: full compact
Name: tools; Description: Tool Definitions; Types: full
Name: scripts; Description: Scripts; Types: full

[Tasks]

Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main; Flags: unchecked

[Files]

Source: {#BasePath}\src\bin\{#ProgramConfiguration}\{#ProgramBuild}\{#ProgramExe}; DestDir: {app}; Flags: ignoreversion; Components: main
Source: {#BasePath}\*.txt; DestDir: {app}; Flags: ignoreversion; Components: docs
Source: {#BasePath}\*.md; DestDir: {app}; Flags: ignoreversion; Components: docs
Source: {#BasePath}\config\formats\*.xml; DestDir: {userappdata}\{#ProgramName}\formats; Flags: ignoreversion; Components: formats
Source: {#BasePath}\config\lang\*.xml; DestDir: {userappdata}\{#ProgramName}\lang; Flags: ignoreversion; Components: lang
Source: {#BasePath}\config\progress\*.lua; DestDir: {userappdata}\{#ProgramName}\progress; Flags: ignoreversion; Components: progress
Source: {#BasePath}\config\tools\*.xml; DestDir: {userappdata}\{#ProgramName}\tools; Flags: ignoreversion; Components: tools
Source: {#BasePath}\config\*.ps1; DestDir: {userappdata}\{#ProgramName}; Flags: ignoreversion; Components: scripts

[INI]

Filename: {app}\{#ProgramName}.url; Section: InternetShortcut; Key: URL; String: {#ProgramWebsite}

[Icons]

Name: {group}\{#ProgramName}; Filename: {app}\{#ProgramExe}; WorkingDir: {app}; Components: main
Name: {group}\{#ProgramName} README; Filename: {app}\README.MD; WorkingDir: {app}; Components: docs
Name: {group}\{#ProgramName} License; Filename: {app}\LICENSE.TXT; WorkingDir: {app}; Components: docs
Name: {group}\{#ProgramName} Web Site; Filename: {app}\{#ProgramName}.url; WorkingDir: {app}; Components: main
Name: {group}\Uninstall {#ProgramName}; Filename: {uninstallexe}; WorkingDir: {app}; Components: main
Name: {userdesktop}\{#ProgramName}; Filename: {app}\{#ProgramExe}; WorkingDir: {app}; Tasks: desktopicon; Components: main
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#ProgramName}; Filename: {app}\{#ProgramExe}; WorkingDir: {app}; Tasks: quicklaunchicon; Components: main

[Run]

Filename: {app}\{#ProgramExe}; Description: {cm:LaunchProgram,{#ProgramName}}; WorkingDir: {app}; Flags: nowait postinstall skipifsilent unchecked; Components: main

[UninstallDelete]

Type: files; Name: {app}\{#ProgramName}.url
