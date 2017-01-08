@echo off
setlocal

set BasePath=Progress
set ProgressNames=ProgressNames.inc

FOR /F "tokens=*" %%i IN (%ProgressNames%) DO (
    @copy /Y "%BasePath%\%%i\bin\Debug\Win32\GetProgress_%%i.dll" "src\bin\Debug\Win32\GetProgress_%%i.progress"
    @copy /Y "%BasePath%\%%i\bin\Debug\x64\GetProgress_%%i.dll" "src\bin\Debug\x64\GetProgress_%%i.progress"
)

endlocal