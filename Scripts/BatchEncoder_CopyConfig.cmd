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
rem $Id: BatchEncoder_CopyConfig.cmd 437 2008-03-24 12:15:45Z wisodev $
setlocal

set BasePath=..
set XmlConfigFile=BatchEncoder.config
set XmlConfigFileTest="%BasePath%\Config\%XmlConfigFile%"
set XmlPresets=%BasePath%\Presets
set BuildNames=BuildNames.inc

FOR /F "tokens=*" %%i IN (%BuildNames%) DO (
    @copy /Y %XmlConfigFileTest% "%BasePath%\%%i\%XmlConfigFile%"
    @copy "%XmlPresets%\*.presets" "%BasePath%\%%i\"
)

@copy /Y "%BasePath%\UnicoWS\UnicoWS.dll" "%BasePath%\Debug\Win32\UnicoWS.dll"
@copy /Y "%BasePath%\UnicoWS\UnicoWS.dll" "%BasePath%\Release\Win32\UnicoWS.dll"

@copy /Y "%BasePath%\Progress\Debug\Win32\*.progress" "%BasePath%\Debug\Win32\"
@copy /Y "%BasePath%\Progress\Debug\x64\*.progress" "%BasePath%\Debug\x64\"
@copy /Y "%BasePath%\Progress\Release\Win32\*.progress" "%BasePath%\Release\Win32\"
@copy /Y "%BasePath%\Progress\Release\x64\*.progress" "%BasePath%\Release\x64\"

endlocal