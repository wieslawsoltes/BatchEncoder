# This is BatchEncoder formats validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-formats-path.ps1

$Files = Get-ChildItem "$pwd\formats" -Filter *.xml

"Validate Formats Path:"

ForEach ($File in $Files)
{
    [xml]$Xml = Get-Content -Path $File.FullName
    $path = $Xml.Format.path
    $PathExists = Test-Path $path
    if ($PathExists) { '[OK] ' + $path } else { '[X] ' + $path }
}
