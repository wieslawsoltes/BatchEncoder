# Download tools https://github.com/wieslawsoltes/BatchEncoder/wiki/Tools

$path = (Get-Item -Path ".\" -Verbose).FullName

$tools = 
@(
    # SSRC
    ( "http://shibatch.sourceforge.net/download/ssrc-1.33.tar.gz", "ssrc-1.33.tar.gz", "ssrc-1.33" ),
    ( "http://shibatch.sourceforge.net/download/ssrc-1.30.tgz", "ssrc-1.30.tgz", "ssrc-1.30" ),
    # LAME
    ( "http://www.rarewares.org/files/mp3/lame3.99.5.zip", "lame3.99.5.zip", "lame3.99.5"    ),
    ( "http://www.rarewares.org/files/mp3/lame3.99.5-64.zip", "lame3.99.5-64.zip", "lame3.99.5-64" ),
    # Musepack 
    ( "https://files.musepack.net/windows/musepack_windows_2009-04-02.zip", "musepack_windows_2009-04-02.zip", "musepack_windows_2009-04-02" ),
    # FAAC 
    ( "http://www.rarewares.org/files/aac/faac-1.28-mod.zip", "faac-1.28-mod.zip", "faac-1.28-mod" ),
    # FAAD2 
    ( "http://www.rarewares.org/files/aac/faad2-20100614.zip", "faad2-20100614.zip", "faad2-20100614" ),
    # Opus
    ( "https://downloads.sourceforge.net/project/muldersoft/Opus%20Tools/opus-tools.2016-11-04.zip?use_mirror=netix", "opus-tools.2016-11-04.zip", "opus-tools.2016-11-04" ),
    # Ogg Vorbis
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-1.3.5-generic.zip", "oggenc2.88-1.3.5-generic.zip", "oggenc2.88-1.3.5-generic" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-1.3.5-P4.zip", "oggenc2.88-1.3.5-P4.zip", "oggenc2.88-1.3.5-P4" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-1.3.5-x64.zip", "oggenc2.88-1.3.5-x64.zip", "oggenc2.88-1.3.5-x64" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-generic.zip", "oggenc2.88-aoTuVb6.03_2015-generic.zip", "oggenc2.88-aoTuVb6.03_2015-generic" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-P4.zip", "oggenc2.88-aoTuVb6.03_2015-P4.zip", "oggenc2.88-aoTuVb6.03_2015-P4" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-x64.zip", "oggenc2.88-aoTuVb6.03_2015-x64.zip", "oggenc2.88-aoTuVb6.03_2015-x64" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE2.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE2.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE2" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE3.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE3.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE3" ),
    ( "http://www.rarewares.org/files/ogg/oggenc2.88-aoTuVb6.03_2015-LancerSSE3_x64.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE3_x64.zip", "oggenc2.88-aoTuVb6.03_2015-LancerSSE3_x64" ),
    ( "http://www.rarewares.org/files/ogg/oggdecV1.10.1.zip", "oggdecV1.10.1.zip", "oggdecV1.10.1" ),
    # Aften
    ( "https://github.com/wieslawsoltes/wavtoac3encoder/releases/download/5.5/aften-git-20091226-icl11_1_054-win32-bin.zip", "aften-git-20091226-icl11_1_054-win32-bin.zip", "aften-git-20091226-icl11_1_054-win32-bin" ),
    ( "https://github.com/wieslawsoltes/wavtoac3encoder/releases/download/5.5/aften-git-20091226-icl11_1_054-win64-bin.zip", "aften-git-20091226-icl11_1_054-win64-bin.zip", "aften-git-20091226-icl11_1_054-win64-bin" ),
    # FLAC
    ( "http://downloads.xiph.org/releases/flac/flac-1.3.2-win.zip", "flac-1.3.2-win.zip", "flac-1.3.2-win" ),
    # WavPack 
    ( "http://www.wavpack.com/wavpack-5.0.0-x86.zip", "wavpack-5.0.0-x86.zip", "wavpack-5.0.0-x86" ),
    ( "http://www.wavpack.com/wavpack-5.0.0-x64.zip", "wavpack-5.0.0-x64.zip", "wavpack-5.0.0-x64" ),
    # Monkey’s Audio
    ( "http://monkeysaudio.com/files/MAC_418.exe", "MAC_418.exe", "MAC_418" ),
    # La
    ( "http://www.lossless-audio.com/La04b.exe", "La04b.exe", "La04b" ),
    # OptimFROG
    ( "http://losslessaudio.org/Downloads/A1C0/5100/Win/OptimFROG_Win_x86_5100.zip", "OptimFROG_Win_x86_5100.zip", "OptimFROG_Win_x86_5100" ),
    ( "http://losslessaudio.org/Downloads/A1C0/5100/Win/OptimFROG_Win_x64_5100.zip", "OptimFROG_Win_x64_5100.zip", "OptimFROG_Win_x64_5100" ),
    # The True Audio
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-32bit.zip?use_mirror=netassist", "tta-2.3-32bit.zip", "tta-2.3-32bit" ),
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-32bit-sse4.zip?use_mirror=netassist", "tta-2.3-32bit-sse4.zip", "tta-2.3-32bit-sse4" ),
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-64bit.zip?use_mirror=netassist", "tta-2.3-64bit.zip", "tta-2.3-64bit" ),
    ( "https://downloads.sourceforge.net/project/tta/tta/ttaenc-win/tta-2.3-64bit-sse4.zip?use_mirror=netassist", "tta-2.3-64bit-sse4.zip", "tta-2.3-64bit-sse4" ),
    # TAK 
    ( "http://www.thbeck.de/Download/TAK_2.3.0.zip", "TAK_2.3.0.zip", "TAK_2.3.0" ),
    # Nero AAC
    ( "http://www.videohelp.com/download/NeroAACCodec-1.5.4.zip?r=rmHRsLcz", "NeroAACCodec-1.5.4.zip", "NeroAACCodec-1.5.4" ),
    # qaac
    ( "https://sites.google.com/site/qaacpage/cabinet/qaac_2.62.zip?attredirects=0&d=1", "qaac_2.62.zip", "qaac_2.62" ),
    # lossyWAV
    ( "https://hydrogenaud.io/index.php?action=dlattach;topic=112649.0;attach=10350", "lossyWAV_v1.4.2.zip", "lossyWAV_v1.4.2" ),
    # FFmpeg
    ( "https://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-3.2.2-win32-static.zip", "ffmpeg-3.2.2-win32-static.zip", "ffmpeg-3.2.2-win32-static" ),
    ( "https://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-3.2.2-win64-static.zip", "ffmpeg-3.2.2-win64-static.zip", "ffmpeg-3.2.2-win64-static" )
)

Function Download {
    Param ($path, $url, $name, $dir)
    (New-Object System.Net.WebClient).DownloadFile($url, $path + "\"  + $name)
    New-Item -Name $dir -ItemType directory
}

foreach ($tool in $tools) {
    Try {
        "Downloading: " + $tool[0]
        Download $path $tool[0] $tool[1] $tool[2]
    }
    Catch {
        "Failed to download: " + $tool[0]
    }
}
