# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools-path.ps1

$Files = Get-ChildItem "$pwd\tools" -Filter *.xml

"Validate Tools Path:"

ForEach ($File in $Files)
{
    [xml]$Xml = Get-Content -Path $File.FullName
    $path = $Xml.Tool.path
    $PathExists = Test-Path $path
    if ($PathExists) { '[OK] ' + $path } else { '[X] ' + $path }
}
