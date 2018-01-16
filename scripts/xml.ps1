# This is BatchEncoder xml generator script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/scripts/xml.ps1

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
$pathFormats = 'BatchEncoder.formats'
[xml]$xmlFormats = Get-Content -Path $pathFormats

foreach($format in $xmlFormats.Formats.Format)
{
    $name = $format.id
    $file = $outputPathFormats + $name + ".format"
    "File: $file"
    Write-Xml $format $file
}

# Extract Presets

$outputPathPresets = "$pwd\presets\"

foreach($format in $xmlFormats.Formats.Format)
{
    $name = $format.id
    $file = $outputPathPresets + $name + ".presets"
    "File: $file"
    Write-Xml $format.Presets $file
}

# Extract Tools

$outputPathTools = "$pwd\tools\"
$pathTools = 'BatchEncoder.tools'
[xml]$xmlTools = Get-Content -Path $pathTools

foreach($tool in $xmlTools.Tools.Tool)
{
    $name = $tool.name
    $file = $outputPathTools + $name + ".tool"
    "File: $file"
    Write-Xml $tool $file
}
