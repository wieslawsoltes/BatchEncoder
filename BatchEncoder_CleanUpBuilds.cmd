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

set BasePath=..
set BuildNames=BuildNames.inc

@del "%BasePath%\*.aps"
@del "%BasePath%\res\*.aps"
@del "%BasePath%\*.ncb"
@del /AH "%BasePath%\*.suo"
@del /Q "%BasePath%\*.user"

FOR /F "tokens=*" %%i IN (%BuildNames%) DO (
    @del "%BasePath%\%%i\BatchEncoder.exe"
    @del "%BasePath%\%%i\*.lib"
    @del "%BasePath%\%%i\*.htm"
    @del "%BasePath%\%%i\*.idb"
    @del "%BasePath%\%%i\*.ilk"
    @del "%BasePath%\%%i\*.obj"
    @del "%BasePath%\%%i\*.pch"
    @del "%BasePath%\%%i\*.pdb"
    @del "%BasePath%\%%i\*.res"
    @del "%BasePath%\%%i\*.manifest"
    @del "%BasePath%\%%i\*.dep"
)

@call BatchEncoder_DeleteConfig.cmd

endlocal