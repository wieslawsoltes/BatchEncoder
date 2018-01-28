# This is BatchEncoder update config script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/update-config.ps1

function Get-File($prefix, $file)
{
    try
    {
        $url = $prefix + $file
        $path = "$pwd\$file"
        Write-Host "Downloading: $url"
        $webclient = New-Object System.Net.WebClient
        $webclient.DownloadFile($url, $path)
        Write-Host "Downloaded: $path"
    } 
    catch
    {
        Write-Host "Could not download $file."
    }
}

# Config Updates Url

$prefix = "https://raw.githubusercontent.com/wieslawsoltes/BatchEncoder/master/config/"

# Download Config

Get-File $prefix "Formats.xml"
Get-File $prefix "Tools.xml"
Get-File $prefix "Languages.xml"

# Download Formats

if (!(Test-Path "$pwd\formats")) 
{
    Write-Host "Creating formats directory..."
    New-Item -Path "$pwd\formats" -Type directory | out-null
}

$PathFormats = 'Formats.xml'
[xml]$XmlFormats = Get-Content -Path $PathFormats

foreach ($Format in $XmlFormats.Formats.Format)
{
    $file = "formats\" + $Format.id + ".xml"
    Get-File $prefix $file
}

# Download Tools

if (!(Test-Path "$pwd\tools")) 
{
    Write-Host "Creating tools directory..."
    New-Item -Path "$pwd\tools" -Type directory | out-null
}

$PathTools = 'Tools.xml'
[xml]$XmlTools = Get-Content -Path $PathTools

foreach ($Tool in $XmlTools.Tools.Tool)
{
    $file = "tools\" + $Tool.name + ".xml"
    Get-File $prefix $file
}

# Download Progress

if (!(Test-Path "$pwd\progress")) 
{
    Write-Host "Creating progress directory..."
    New-Item -Path "$pwd\progress" -Type directory | out-null
}

foreach ($Format in $XmlFormats.Formats.Format)
{
    $XmlPath = "$pwd\formats\" + $Format.id + ".xml"
    [xml]$XmlFormat = Get-Content -Path $XmlPath
    $file = $XmlFormat.Format.function

    if ($XmlFormat.Format.function -ne "- none -")
    {
        Get-File $prefix $file
    }
}

# Download Languages

if (!(Test-Path "$pwd\lang")) 
{
    Write-Host "Creating lang directory..."
    New-Item -Path "$pwd\lang" -Type directory | out-null
}

$PathLanguages = 'Languages.xml'
[xml]$XmlLanguages = Get-Content -Path $PathLanguages

foreach ($Language in $XmlLanguages.Languages.Language)
{
    $file = "lang\" + $Language.id + ".xml"
    Get-File $prefix $file
}
