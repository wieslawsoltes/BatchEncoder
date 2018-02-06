# This is BatchEncoder tools download script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/download.ps1
# Detailed list of tools can be found here https://github.com/wieslawsoltes/BatchEncoder/wiki/Tools

<#
.SYNOPSIS
This is a Powershell script to download tools for BatchEncoder.
.DESCRIPTION
This Powershell script will download all necessary tools for BatchEncoder to work.
.PARAMETER Path
The tools output path.
.LINK
https://github.com/wieslawsoltes/BatchEncoder
#>

[CmdletBinding()]
Param(
    [string]$Path = (Get-Item -Path ".\" -Verbose).FullName
)

if(!(Test-Path -Path $Path )) {
    New-Item -ItemType directory -Name $Path
}

$FullPath = Resolve-Path $Path
$FullPath = (Get-Item -Path $FullPath -Verbose).FullName

"Ddownload Path: " + $FullPath

$Files = Get-ChildItem "$pwd\tools" -Filter *.xml

ForEach ($File in $Files)
{
    [xml]$Xml = Get-Content -Path $File.FullName
    $url = $Xml.Tool.url
    $file = $Xml.Tool.file
    Try {
        "Downloading: " + $url
        (New-Object System.Net.WebClient).DownloadFile($url, $FullPath + "\"  + $file)
    }
    Catch {
        "Failed to download: " + $Xml.Tool.name
    }
}
