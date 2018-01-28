# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools-file.ps1

$PathTools = 'Tools.xml'
[xml]$Xml = Get-Content -Path $PathTools

"Validate Tools File:"

foreach ($Tool in $Xml.Tools.Tool) 
{
    $XmlPath = "$pwd\tools\" + $Tool.name + ".xml"
    [xml]$XmlTool = Get-Content -Path $XmlPath
    $file = "$pwd\tools\" + $XmlTool.Tool.file
    $FileExists = Test-Path $file
    if ($FileExists) { '[OK] ' + $file } else { '[X] ' + $file }
}
