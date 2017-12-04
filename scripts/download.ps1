# This is BatchEncoder tools download script for PowerShell.
# This file was downloaded from https://github.com/wieslawsoltes/BatchEncoder/edit/master/scripts/download.ps1
# Detailed list of tools can be found here https://github.com/wieslawsoltes/BatchEncoder/wiki/Tools

<#
.SYNOPSIS
This is a Powershell script to download tools for BatchEncoder.
.DESCRIPTION
This Powershell script will download all necessary tools for BatchEncoder to work.
.PARAMETER Path
The tools output path.
.LINK
https://github.com/wieslawsoltes/BatchEncoder
#>

[CmdletBinding()]
Param(
    [string]$Path = (Get-Item -Path ".\" -Verbose).FullName
)

$Tools = 
@(
    # SSRC
    ( "http://shibatch.sourceforge.net/download/ssrc-1.33.tar.gz", "ssrc-1.33.tar.gz" ),
    ( "http://shibatch.sourceforge.net/download/ssrc-1.30.tgz", "ssrc-1.30.tgz" ),
    # LAME
    ( "http://www.rarewares.org/files/mp3/lame3.100.zip", "lame3.100.zip" ),
    ( "http://www.rarewares.org/files/mp3/lame3.100-64.zip", "lame3.100-64.zip" ),
    # Musepack 
    ( "https://files.musepack.net/windows/musepack_windows_2009-04-02.zip", "musepack_windows_2009-04-02.zip" ),
    # FAAC 
    ( "http://www.rarewares.org/files/aac/faac-1.28-mod.zip", "faac-1.28-mod.zip" ),
    # FAAD2 
    ( "http://www.rarewares.org/files/aac/faad2-20100614.zip", "faad2-20100614.zip" ),
    # Opus
    ( "https://downloads.sourceforge.net/project/muldersoft/Opus%20Tools/opus-tools.2016-11-04.zip?use_mirror=netix", "opus-tools.2016-11-04.zip" ),
    # Ogg Vorbis
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-1.3.5-generic.zip", "oggenc2.88-1.3.5-generic.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-1.3.5-P4.zip", "oggenc2.88-1.3.5-P4.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-1.3.5-x64.zip", "oggenc2.88-1.3.5-x64.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-generic.zip", "oggenc2.88-aoTuVb6.03_2015-generic.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-P4.zip", "oggenc2.88-aoTuVb6.03_2015-P4.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-x64.zip", "oggenc2.88-aoTuVb6.03_2015-x64.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE2.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE2.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE3.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE3.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE3_x64.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE3_x64.zip" ),
    ( "http://www.rarewares.org/files/ogg/oggdecV1.10.1.zip", "oggdecV1.10.1.zip" ),
    # Aften
    ( "https://github.com/wieslawsoltes/AftenWindowsBuilds/releases/download/git-20091226/aften-git-20091226-icl11_1_054-win32-bin.zip", "aften-git-20091226-icl11_1_054-win32-bin.zip" ),
    ( "https://github.com/wieslawsoltes/AftenWindowsBuilds/releases/download/git-20091226/aften-git-20091226-icl11_1_054-win64-bin.zip", "aften-git-20091226-icl11_1_054-win64-bin.zip" ),
    # FLAC
    ( "http://downloads.xiph.org/releases/flac/flac-1.3.2-win.zip", "flac-1.3.2-win.zip" ),
    # WavPack 
    ( "http://www.wavpack.com/wavpack-5.0.0-x86.zip", "wavpack-5.0.0-x86.zip" ),
    ( "http://www.wavpack.com/wavpack-5.0.0-x64.zip", "wavpack-5.0.0-x64.zip" ),
    # Monkey’s Audio
    ( "http://monkeysaudio.com/files/MAC_418.exe", "MAC_418.exe" ),
    # La
    ( "http://www.lossless-audio.com/La04b.exe", "La04b.exe" ),
    # OptimFROG
    ( "http://losslessaudio.org/Downloads/A1C0/5100/Win/OptimFROG_Win_x86_5100.zip", "OptimFROG_Win_x86_5100.zip" ),
    ( "http://losslessaudio.org/Downloads/A1C0/5100/Win/OptimFROG_Win_x64_5100.zip", "OptimFROG_Win_x64_5100.zip" ),
    # The True Audio
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-32bit.zip?use_mirror=netassist", "tta-2.3-32bit.zip" ),
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-32bit-sse4.zip?use_mirror=netassist", "tta-2.3-32bit-sse4.zip" ),
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-64bit.zip?use_mirror=netassist", "tta-2.3-64bit.zip" ),
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-64bit-sse4.zip?use_mirror=netassist", "tta-2.3-64bit-sse4.zip" ),
    # TAK 
    ( "http://www.thbeck.de/Download/TAK_2.3.0.zip", "TAK_2.3.0.zip" ),
    # Nero AAC
    ( "http://www.videohelp.com/download/NeroAACCodec-1.5.4.zip?r=rmHRsLcz", "NeroAACCodec-1.5.4.zip" ),
    # qaac
    ( "https://sites.google.com/site/qaacpage/cabinet/qaac_2.64.zip?attredirects=0&d=1", "qaac_2.64.zip" ),
    # lossyWAV
    ( "https://hydrogenaud.io/index.php?action=dlattach;topic=112649.0;attach=10350", "lossyWAV_v1.4.2.zip" ),
    # FFmpeg
    ( "https://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-3.4-win32-static.zip", "ffmpeg-3.4-win32-static.zip" ),
    ( "https://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-3.4-win64-static.zip", "ffmpeg-3.4-win64-static.zip" ),
    # ffdcaenc
    ( "https://github.com/filler56789/ffdcaenc-2/files/299448/ffdcaenc.zip", "ffdcaenc.zip" )
)

if(!(Test-Path -Path $Path )) {
    New-Item -ItemType directory -Name $Path
}

$FullPath = Resolve-Path $Path
$FullPath = (Get-Item -Path $FullPath -Verbose).FullName

"Ddownload Path: " + $FullPath

foreach ($Tool in $Tools) {
    Try {
        "Downloading: " + $tool[0]
        (New-Object System.Net.WebClient).DownloadFile($Tool[0], $FullPath + "\"  + $Tool[1])
    }
    Catch {
        "Failed to download: " + $Tool[0]
    }
}
