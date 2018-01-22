﻿# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools-path.ps1

$PathTools = 'Tools.xml'
[xml]$Xml = Get-Content -Path $PathTools

"Validate Tools Path:"

foreach ($Tool in $Xml.Tools.Tool) 
{
    $path = $Tool.path
    $PathExists = Test-Path $path
    if ($PathExists) { '[OK] ' + $path } else { '[X] ' + $path }
}