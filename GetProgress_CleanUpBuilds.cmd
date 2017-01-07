@echo off
rem
rem BatchEncoder (Audio Conversion GUI)
rem Copyright (C) 2005-2017 Wieslaw Soltes <wieslaw.soltes@gmail.com>
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
setlocal

set BasePath=..\Progress
set ProgressNames=ProgressNames.inc

FOR /F "tokens=*" %%i IN (%ProgressNames%) DO (
    @del "%BasePath%\%%i\*.aps"
    @del "%BasePath%\%%i\*.ncb"
    @del /AH "%BasePath%\%%i\*.suo"
    @del "%BasePath%\%%i\*.user"

    @del "%BasePath%\%%i\Debug\Win32\*.dll"
    @del "%BasePath%\%%i\Debug\Win32\*.exp"
    @del "%BasePath%\%%i\Debug\Win32\*.lib"
    @del "%BasePath%\%%i\Debug\Win32\*.htm"
    @del "%BasePath%\%%i\Debug\Win32\*.idb"
    @del "%BasePath%\%%i\Debug\Win32\*.ilk"
    @del "%BasePath%\%%i\Debug\Win32\*.obj"
    @del "%BasePath%\%%i\Debug\Win32\*.pch"
    @del "%BasePath%\%%i\Debug\Win32\*.pdb"
    @del "%BasePath%\%%i\Debug\Win32\*.res"
    @del "%BasePath%\%%i\Debug\Win32\*.manifest"
    @del "%BasePath%\%%i\Debug\Win32\*.dep"

    @del "%BasePath%\%%i\Debug\x64\*.dll"
    @del "%BasePath%\%%i\Debug\x64\*.exp"
    @del "%BasePath%\%%i\Debug\x64\*.lib"
    @del "%BasePath%\%%i\Debug\x64\*.htm"
    @del "%BasePath%\%%i\Debug\x64\*.idb"
    @del "%BasePath%\%%i\Debug\x64\*.ilk"
    @del "%BasePath%\%%i\Debug\x64\*.obj"
    @del "%BasePath%\%%i\Debug\x64\*.pch"
    @del "%BasePath%\%%i\Debug\x64\*.pdb"
    @del "%BasePath%\%%i\Debug\x64\*.res"
    @del "%BasePath%\%%i\Debug\x64\*.manifest"
    @del "%BasePath%\%%i\Debug\x64\*.dep"

    @del "%BasePath%\%%i\Release\Win32\*.dll"
    @del "%BasePath%\%%i\Release\Win32\*.exp"
    @del "%BasePath%\%%i\Release\Win32\*.lib"
    @del "%BasePath%\%%i\Release\Win32\*.htm"
    @del "%BasePath%\%%i\Release\Win32\*.idb"
    @del "%BasePath%\%%i\Release\Win32\*.ilk"
    @del "%BasePath%\%%i\Release\Win32\*.obj"
    @del "%BasePath%\%%i\Release\Win32\*.pch"
    @del "%BasePath%\%%i\Release\Win32\*.pdb"
    @del "%BasePath%\%%i\Release\Win32\*.res"
    @del "%BasePath%\%%i\Release\Win32\*.manifest"
    @del "%BasePath%\%%i\Release\Win32\*.dep"

    @del "%BasePath%\%%i\Release\x64\*.dll"
    @del "%BasePath%\%%i\Release\x64\*.exp"
    @del "%BasePath%\%%i\Release\x64\*.lib"
    @del "%BasePath%\%%i\Release\x64\*.htm"
    @del "%BasePath%\%%i\Release\x64\*.idb"
    @del "%BasePath%\%%i\Release\x64\*.ilk"
    @del "%BasePath%\%%i\Release\x64\*.obj"
    @del "%BasePath%\%%i\Release\x64\*.pch"
    @del "%BasePath%\%%i\Release\x64\*.pdb"
    @del "%BasePath%\%%i\Release\x64\*.res"
    @del "%BasePath%\%%i\Release\x64\*.manifest"
    @del "%BasePath%\%%i\Release\x64\*.dep"
)

@call GetProgress_DeleteDebugBuilds.cmd
@call GetProgress_DeleteReleaseBuilds.cmd

endlocal