# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools-url.ps1

$PathTools = 'Tools.xml'
[xml]$Xml = Get-Content -Path $PathTools

"Validate Tools Url:"

foreach ($Tool in $Xml.Tools.Tool) 
{
    $XmlPath = "$pwd\tools\" + $Tool.name + ".xml"
    [xml]$XmlTool = Get-Content -Path $XmlPath
    $url = $XmlTool.Tool.url
    $req = [net.HttpWebRequest]::create($url)
    $req.Method = "GET"
    $res = $req.getResponse()
    if ($res.StatusCode -ge "200") { '[OK] ' + $url } else { '[X] ' + $url }
    $res.Close()
}
