# This is BatchEncoder tools validation script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/blob/master/config/validate-tools-url.ps1

$Files = Get-ChildItem "$pwd\tools" -Filter *.xml

"Validate Tools Url:"

ForEach ($File in $Files)
{
    [xml]$Xml = Get-Content -Path $File.FullName
    $url = $Xml.Tool.url
    $req = [net.HttpWebRequest]::create($url)
    $req.Method = "GET"
    $res = $req.getResponse()
    if ($res.StatusCode -ge "200") { '[OK] ' + $url } else { '[X] ' + $url }
    $res.Close()
}
