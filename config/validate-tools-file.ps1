# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools-file.ps1

$Files = Get-ChildItem "$pwd\tools" -Filter *.xml

"Validate Tools File:"

ForEach ($File in $Files)
{
    [xml]$Xml = Get-Content -Path $File.FullName
    $file = "$pwd\tools\" + $Xml.Tool.file
    $FileExists = Test-Path $file
    if ($FileExists) { '[OK] ' + $file } else { '[X] ' + $file }
}
