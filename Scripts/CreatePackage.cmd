@echo off
rem
rem BatchEncoder (Audio Conversion GUI)
rem Copyright (C) 2005-2008 Wieslaw Soltes <wisodev@users.sourceforge.net>
rem
rem This program is free software; you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation; version 2 of the License.
rem
rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem
rem You should have received a copy of the GNU General Public License
rem along with this program; if not, write to the Free Software
rem Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
rem
rem $Id: CreatePackage.cmd 437 2008-03-24 12:15:45Z wisodev $
setlocal

IF /i ""=="%1" (
    set varPackageVerion=1.0.0.0
) ELSE (
    set varPackageVerion=%1 
)

@echo Setting package version to: %varPackageVerion%

set varPathToWinRAR=C:\Program Files\WinRAR\WinRAR.exe
set varOptionsForWinRAR=a -ep1 -m5 -r
set varSrcDir=Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-src
set varSourceDirs=SourceDirs.inc
set varBuildNames=BuildNames.inc
set varProgressNames=ProgressNames.inc
set varPathToUnicoWS=..\UnicoWS
set varPathToSources=..
set varCurrentDate=%date:~0,4%%date:~5,2%%date:~8,2% 

@mkdir "%varSrcDir%"

FOR /F "tokens=*" %%i IN (%varSourceDirs%) DO (
    @mkdir "%varSrcDir%\%%i"
)

FOR /F "tokens=*" %%i IN (%varBuildNames%) DO (
    @mkdir "%varSrcDir%\%%i"
)

@mkdir "%varSrcDir%\Progress"
@mkdir "%varSrcDir%\Progress\Debug\Win32"
@mkdir "%varSrcDir%\Progress\Debug\x64"
@mkdir "%varSrcDir%\Progress\Release\Win32"
@mkdir "%varSrcDir%\Progress\Release\x64"

FOR /F "tokens=*" %%i IN (%varProgressNames%) DO (
    @mkdir "%varSrcDir%\Progress\%%i\Debug\Win32"
    @mkdir "%varSrcDir%\Progress\%%i\Debug\x64"
    @mkdir "%varSrcDir%\Progress\%%i\Release\Win32"
    @mkdir "%varSrcDir%\Progress\%%i\Release\x64"
    @copy /Y "%varPathToSources%\Progress\%%i\*.cpp" "%varSrcDir%\Progress\%%i"
    @copy /Y "%varPathToSources%\Progress\%%i\*.h" "%varSrcDir%\Progress\%%i"
    @copy /Y "%varPathToSources%\Progress\%%i\*.def" "%varSrcDir%\Progress\%%i"
    @copy /Y "%varPathToSources%\Progress\%%i\*.vcproj" "%varSrcDir%\Progress\%%i"
)

@copy /Y "%varPathToSources%\*.sln" "%varSrcDir%"
@copy /Y "%varPathToSources%\*.vcproj" "%varSrcDir%"

@copy /Y "%varPathToSources%\doc\*.*" "%varSrcDir%\doc"
@copy /Y "%varPathToSources%\res\*.*" "%varSrcDir%\res"
@copy /Y "%varPathToSources%\src\*.*" "%varSrcDir%\src"
@copy /Y "%varPathToSources%\Scripts\*.*" "%varSrcDir%\Scripts"
@copy /Y "%varPathToSources%\Help\*.*" "%varSrcDir%\Help\"
@copy /Y "%varPathToSources%\Help\Images\*.*" "%varSrcDir%\Help\Images"
@copy /Y "%varPathToSources%\Logo\*.*" "%varSrcDir%\Logo\"
@copy /Y "%varPathToSources%\Presets\*.*" "%varSrcDir%\Presets\"
@copy /Y "%varPathToSources%\Formats\*.*" "%varSrcDir%\Formats\"
@copy /Y "%varPathToSources%\Config\*.*" "%varSrcDir%\Config\"
@copy /Y "%varPathToSources%\UnicoWS\*.*" "%varSrcDir%\UnicoWS\"

@mkdir "%varSrcDir%\src\tinyxml"
@mkdir "%varSrcDir%\src\tinyxml\docs"

@copy /Y "%varPathToSources%\src\tinyxml\*.*" "%varSrcDir%\src\tinyxml"
@copy /Y "%varPathToSources%\src\tinyxml\docs\*.*" "%varSrcDir%\src\tinyxml\docs"

"%varPathToWinRAR%" %varOptionsForWinRAR% "%varSrcDir%.zip" "%varSrcDir%\*"

@rmdir /S /Q "%varSrcDir%"

FOR /D %%i IN (Win32;x64) DO (
    @mkdir Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin

    @copy /Y "..\Release\%%i\BatchEncoder.exe" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"

    if /i "Win32"=="%%i" @copy /Y "%varPathToUnicoWS%\UnicoWS.dll" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"

    @copy /Y "%varPathToSources%\doc\*.*" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"
    @copy /Y "%varPathToSources%\Help\*.pdf" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"
    @copy /Y "%varPathToSources%\Presets\*.presets" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"
    @copy /Y "%varPathToSources%\Formats\*.formats" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"
    @copy /Y "%varPathToSources%\Config\*.config" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"
    @copy /Y "%varPathToSources%\Progress\Release\%%i\*.progress" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"

    "%varPathToWinRAR%" %varOptionsForWinRAR% "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin.zip" "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin\*"

    @rmdir /S /Q "Package-%varPackageVerion%\BatchEncoder-%varPackageVerion%-%%i-bin"

    @copy /Y SetupVersion.inc SetupVersion.inc.bak
    echo #define ProgramVersion "%varPackageVerion%" > SetupVersion.inc
    echo #define ProgramBuild "%%i" >> SetupVersion.inc
    @call BatchEncoder_CreateInstaller.cmd
    @del SetupVersion.inc
    @move /Y SetupVersion.inc.bak SetupVersion.inc
    @move /Y "BatchEncoder-%varPackageVerion%-%%i-installer.exe" "Package-%varPackageVerion%\"
)

echo Package %varPackageVerion% was created at %varCurrentDate%.

endlocal