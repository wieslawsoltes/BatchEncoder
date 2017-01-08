@echo off
setlocal

set BasePath=
set XmlConfigFile=BatchEncoder.config
set XmlConfigFileTest="%BasePath%\data\Config\%XmlConfigFile%"
set XmlPresets=%BasePath%\data\Presets
set BuildNames=BuildNames.inc

FOR /F "tokens=*" %%i IN (%BuildNames%) DO (
    @copy /Y %XmlConfigFileTest% "%BasePath%\src\bin\%%i\%XmlConfigFile%"
    @copy "%XmlPresets%\*.presets" "%BasePath%\src\bin\%%i\"
)

@copy /Y "%BasePath%\UnicoWS\UnicoWS.dll" "%BasePath%\src\bin\Debug\Win32\UnicoWS.dll"
@copy /Y "%BasePath%\UnicoWS\UnicoWS.dll" "%BasePath%\src\bin\Release\Win32\UnicoWS.dll"

endlocal