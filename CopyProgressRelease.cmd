@echo off
setlocal

set BasePath=Progress
set ProgressNames=ProgressNames.inc

FOR /F "tokens=*" %%i IN (%ProgressNames%) DO (
    @copy /Y "%BasePath%\%%i\bin\Release\Win32\GetProgress_%%i.dll" "src\bin\Release\Win32\GetProgress_%%i.progress"
    @copy /Y "%BasePath%\%%i\bin\Release\x64\GetProgress_%%i.dll" "src\bin\Release\x64\GetProgress_%%i.progress"
)

endlocal