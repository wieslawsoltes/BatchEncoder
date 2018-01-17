# This is BatchEncoder download config script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/update-config.ps1

function Download-File($prefix, $file)
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

# Download Formats

Download-File $prefix "Formats.xml"

# Download Tools

Download-File $prefix "Tools.xml"

# Download Languages

Download-File $prefix "Languages.xml"

# Download Progress Functions Files

if (!(Test-Path "$pwd\progress")) 
{
    Write-Host "Creating progress directory..."
    New-Item -Path "$pwd\progress" -Type directory | out-null
}

$PathFormats = 'Formats.xml'
[xml]$XmlFormats = Get-Content -Path $PathFormats

foreach ($Format in $XmlFormats.Formats.Format)
{
    if ($Format.function -ne "- none -")
    {
        Download-File $prefix $Format.function
    }
}

# Download Language Files

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
    Download-File $prefix $file
}
