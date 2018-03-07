@echo off
setlocal

set ConfigDir=%1
set OutputDir=%2
set BuildSystem=%3
set FormatsFolder=formats
set LangFolder=lang
set ProgressFolder=progress
set ToolsFolder=tools

if %BuildSystem% == CI goto done

echo ConfigDir: %ConfigDir%
echo OutputDir: %OutputDir%
echo BuildSystem: %BuildSystem%

copy /Y %ConfigDir%*.ps1 %OutputDir%

if not exist %OutputDir%%FormatsFolder%\ mkdir %OutputDir%%FormatsFolder%\
if not exist %OutputDir%%LangFolder%\ mkdir %OutputDir%%LangFolder%\
if not exist %OutputDir%%ProgressFolder%\ mkdir %OutputDir%%ProgressFolder%\
if not exist %OutputDir%%ToolsFolder%\ mkdir %OutputDir%%ToolsFolder%\

copy /Y %ConfigDir%%FormatsFolder%\*.xml %OutputDir%%FormatsFolder%\
copy /Y %ConfigDir%%LangFolder%\*.xml %OutputDir%%LangFolder%\
copy /Y %ConfigDir%%ProgressFolder%\*.lua %OutputDir%%ProgressFolder%\
copy /Y %ConfigDir%%ToolsFolder%\*.xml %OutputDir%%ToolsFolder%\
copy /Y %ConfigDir%*.portable %OutputDir%

:done
endlocal
