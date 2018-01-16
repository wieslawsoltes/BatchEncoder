# This is BatchEncoder xml generator script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/xml.ps1

function Write-Xml($node, $file)
{
    $doc = New-Object System.Xml.XmlDocument
    $inserted = $doc.ImportNode($node, $true)
    $child = $doc.AppendChild($inserted)

    $settings = New-Object System.Xml.XmlWriterSettings
    $settings.Encoding = New-Object System.Text.UTF8Encoding($true)
    $settings.Indent = $true
    $settings.IndentChars = "    "
    $settings.NewLineChars = "`r`n"

    $writer = [System.Xml.XmlWriter]::Create($file, $settings)
    $doc.Save($writer)
    $writer.Close()
}

# Extract Formats

$outputPathFormats = "$pwd\formats\"
$pathFormats = 'Formats.xml'
[xml]$xmlFormats = Get-Content -Path $pathFormats

foreach($format in $xmlFormats.Formats.Format)
{
    $name = $format.id
    $file = $outputPathFormats + $name + ".xml"
    "File: $file"
    Write-Xml $format $file
}

# Extract Presets

$outputPathPresets = "$pwd\presets\"

foreach($format in $xmlFormats.Formats.Format)
{
    $name = $format.id
    $file = $outputPathPresets + $name + ".xml"
    "File: $file"
    Write-Xml $format.Presets $file
}

# Extract Tools

$outputPathTools = "$pwd\tools\"
$pathTools = 'Tools.xml'
[xml]$xmlTools = Get-Content -Path $pathTools

foreach($tool in $xmlTools.Tools.Tool)
{
    $name = $tool.name
    $file = $outputPathTools + $name + ".xml"
    "File: $file"
    Write-Xml $tool $file
}
