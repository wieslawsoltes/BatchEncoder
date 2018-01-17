# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools.ps1

$PathTools = 'Tools.xml'
[xml]$Xml = Get-Content -Path $PathTools

"Validate Tools Path:"

foreach ($Tool in $Xml.Tools.Tool) 
{
    $path = $Tool.path
    $PathExists = Test-Path $path
    if ($PathExists) { '[OK] ' + $path } else { '[X] ' + $path }
}

"Validate Tools File:"

foreach ($Tool in $Xml.Tools.Tool) 
{
    $file = "tools\" + $Tool.file
    $FileExists = Test-Path $file
    if ($FileExists) { '[OK] ' + $file } else { '[X] ' + $file }
}

"Validate Tools Url:"

foreach ($Tool in $Xml.Tools.Tool) 
{
    $url = $Tool.url
    $req = [net.HttpWebRequest]::create($url)
    $req.Method = "GET"
    $res = $req.getResponse()
    if ($res.StatusCode -ge "200") { '[OK] ' + $url } else { '[X] ' + $url }
    $res.Close()
}
