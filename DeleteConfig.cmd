@echo off
setlocal

set BasePath=
set XmlConfigFile=BatchEncoder.config
set BuildNames=BuildNames.inc

FOR /F "tokens=*" %%i IN (%BuildNames%) DO (
    @del "%BasePath%\src\bin\%%i\%XmlConfigFile%"
    @del /Q "%BasePath%\src\bin\%%i\*.presets"
)

@del "%BasePath%\src\bin\Debug\Win32\UnicoWS.dll"
@del "%BasePath%\src\bin\Release\Win32\UnicoWS.dll"

endlocal