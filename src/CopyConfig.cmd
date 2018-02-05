@echo off
setlocal
set ConfigDir=%1
set SetupDir=%2
set OutputDir=%3
copy /-Y %ConfigDir%*.xml %OutputDir%
copy /Y %ConfigDir%*.ps1 %OutputDir%
if not exist %OutputDir%formats\ mkdir %OutputDir%formats\
if not exist %OutputDir%lang\ mkdir %OutputDir%lang\
if not exist %OutputDir%progress\ mkdir %OutputDir%progress\
if not exist %OutputDir%tools\ mkdir %OutputDir%tools\
copy /Y %ConfigDir%formats\*.xml %OutputDir%formats\
copy /Y %ConfigDir%lang\*.xml %OutputDir%lang\
copy /Y %ConfigDir%progress\*.lua %OutputDir%progress\
copy /Y %ConfigDir%tools\*.xml %OutputDir%tools\
copy /Y %SetupDir%*.portable %OutputDir%
endlocal
