@echo off
setlocal

set BasePath=
set ProgressNames=ProgressNames.inc

FOR /F "tokens=*" %%i IN (%ProgressNames%) DO (
    @del /Q "%BasePath%\src\bin\Debug\Win32\GetProgress_%%i.progress"
    @del /Q "%BasePath%\src\bin\Debug\x64\GetProgress_%%i.progress"
)

endlocal