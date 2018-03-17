-- You can run tests in the browser: https://www.lua.org/demo.html

-- GetProgress_AftenEnc

function GetProgress(s) 
  return string.match(s, 'progress:%s-(%d+)%%');
end

p = GetProgress("progress:  67% | q: 368.9 | bw: 60.0 | bitrate: 448.0 kbps ")
print(p)
assert(p == "67")

--- GetProgress_AvsDec

function GetProgress(s) 
  return string.match(s, '^Progress: (%d+)%%');
end

p = GetProgress("Progress: 100% (265533250/265533250)")
print(p)
assert(p == "100")

-- GetProgress_Eac3toEnc.progress

function GetProgress(s) 
  if string.match(s, '^Done.%s-') ~= nil then return "100";
  else return string.match(s, 'process: (%d+)%%'); end;
end

p = GetProgress("Done.")
print(p)
assert(p == "100")

p = GetProgress("process: 88%")
print(p)
assert(p == "88")

-- GetProgress_FFdcaEnc

function GetProgress(s) 
  if string.match(s, '^Done.%s-') ~= nil then return "100";
  else return string.match(s, '%[(%d+.%d+)%%%]'); end;
end

p = GetProgress("Done.")
print(p)
assert(p == "100")

p = GetProgress("Encoding... 3:39 [84.7%]")
print(p)
assert(p == "84.7")

-- GetProgress_FFmpegDec

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_FFmpegEnc

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_FaacEnc

function GetProgress(s) 
  return string.match(s, '(%d+)%%');
end

p = GetProgress("144200/157023 ( 91%)|  222.6  |   59.6/64.9   |   56.17x | 5.3")
print(p)
assert(p == "91")

-- GetProgress_FaadDec

function GetProgress(s) 
  if string.match(s, '^Decoding') ~= nil then return "100";
  else return string.match(s, '(%d+)%%'); end;
end

p = GetProgress("Decoding 01 Beach House - Myth.aac took:  0.75 sec. 344.92x real-time.")
print(p)
assert(p == "100")

p = GetProgress("31% decoding 01 Beach House - Myth.aac.")
print(p)
assert(p == "31")

-- GetProgress_FlacDec

function GetProgress(s)
  if string.match(s, 'done') ~= nil then return "100";
  else return string.match(s, '(%d+)%%'); end;
end

p = GetProgress("Image.flac: done")
print(p)
assert(p == "100")

p = GetProgress("Image.flac: 42% complete")
print(p)
assert(p == "42")

-- GetProgress_FlacEnc

function GetProgress(s)
  if string.match(s, '%swrote%s') ~= nil then return "100";
  else return string.match(s, '(%d+)%%'); end;
end

p = GetProgress("Image.wav: wrote 415168867 bytes, ratio=0,646")
print(p)
assert(p == "100")

p = GetProgress("Image.wav: 41% complete, ratio=0,643")
print(p)
assert(p == "41")

-- GetProgress_LaDec

function GetProgress(s) 
  r = string.match(s, '%*+');
  if r == nil then return "-1";
  else return tostring((string.len(r) * 100) / 24); end;
end

p = GetProgress("Decoding 01 Beach House - Myth.la [........................]")
print(p)
assert(p == "-1")

p = GetProgress("Decoding 01 Beach House - Myth.la [**************..........]")
print(p)
assert(p == "58.333333333333")

p = GetProgress("Decoding 01 Beach House - Myth.la [************************]")
print(p)
assert(p == "100.0")

-- GetProgress_LaEnc

function GetProgress(s) 
  r = string.match(s, '%*+');
  if r == nil then return "-1";
  else return tostring((string.len(r) * 100) / 24); end;
end

p = GetProgress("Encoding 01 Beach House - Myth.wav [........................] (42.4%)")
print(p)
assert(p == "-1")

p = GetProgress("Encoding 01 Beach House - Myth.wav [********................] (52.5%)")
print(p)
assert(p == "33.333333333333")

p = GetProgress("Encoding 01 Beach House - Myth.wav [************************]")
print(p)
assert(p == "100.0")

-- GetProgress_LameDec

function GetProgress(s) 
  c,t = string.match(s, 'Frame#%s-(%d+)/(%d+)');
  if (c == nil) or (t == nil) then return "-1";
  elseif (c == "0") or (t == "0") then return "0";
  else return tostring((100 * c) / t); end;
end

p = GetProgress("Frame# 23067/139576 160 kbps   MS")
print(p)
assert(p == "16.526480197169")

p = GetProgress("Frame# 0/0 160 kbps   MS")
print(p)
assert(p == "0")

p = GetProgress("Frame# 0/139576 160 kbps   MS")
print(p)
assert(p == "0")

p = GetProgress("Frame#139576/139576  32 kbps  L  R   ")
print(p)
assert(p == "100.0")

-- GetProgress_LameEnc

function GetProgress(s) 
  return string.match(s, '((%d+))%%');
end

p = GetProgress(" 21400/139576 (15%)|    0:09/    0:59|    0:09/    0:59|   61.108x|    0:50")
print(p)
assert(p == "15")

--- GetProgress_MacDec

function GetProgress(s) 
  return string.match(s, 'Progress: (%d+.%d+)%%');
end

p = GetProgress("'Progress: 21.4% (4.8 seconds remaining, 1.3 seconds total)'")
print(p)
assert(p == "21.4")

--- GetProgress_MacEnc

function GetProgress(s) 
  return string.match(s, 'Progress: (%d+.%d+)%%');
end

p = GetProgress("Progress: 49.7% (2.8 seconds remaining, 2.7 seconds total)")
print(p)
assert(p == "49.7")

-- GetProgress_MpcDec

function GetProgress(s) 
  if string.match(s, '%ssamples decoded%s') ~= nil then return "100";
  else return "-1"; end;
end

p = GetProgress("160790364 samples decoded in 4350 ms (838.17x)")
print(p)
assert(p == "100")

-- GetProgress_MpcEnc

function GetProgress(s) 
  return string.match(s, '^%s-(%d+.%d+)');
end

p = GetProgress("  -.-    -.- kbps  -.--x     -:--.-    -:--.-     -:--.-    -:--.-     -:--.-")
print(p)
assert(p == nil)

p = GetProgress("  1.5  171.5 kbps 89.11x     0:56.1 1:00:46.0     0:00.6    0:40.9     0:40.2")
print(p)
assert(p == "1.5")

-- GetProgress_NeroAacDec

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_NeroAacEnc

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_OfrDec

function GetProgress(s) 
  if string.match(s, 'done.') ~= nil then return "100";
  else return string.match(s, '(%d+.%d+)%%'); end;
end

p = GetProgress("Decompression done.")
print(p)
assert(p == "100")

p = GetProgress("Decompressing  10.2%")
print(p)
assert(p == "10.2")

-- GetProgress_OfrEnc

function GetProgress(s) 
  if string.match(s, 'done.') ~= nil then return "100";
  else return string.match(s, '(%d+.%d+)%%'); end;
end

p = GetProgress("Compressing   done.")
print(p)
assert(p == "100")

p = GetProgress("Compressing    15.0%")
print(p)
assert(p == "15.0")

-- GetProgress_OggDec

function GetProgress(s) 
  return string.match(s, '(%d+)%% decoded');
end

p = GetProgress("   22% decoded.")
print(p)
assert(p == "22")

-- GetProgress_OggEnc

function GetProgress(s) 
  if string.match(s, '^Done encoding') ~= nil then return "100";
  else return string.match(s, '%[%s-(%d+.%d+)%%%]'); end;
end

p = GetProgress("[ 22,4%] [ 0m51s remaining] -")
print(p)
assert(p == "22,4")

p = GetProgress("Done encoding file Image.ogg")
print(p)
assert(p == "100")

-- GetProgress_OpusDec

function GetProgress(s) 
  if string.match(s, '^Decoding complete%s-') ~= nil then return "100";
  else return string.match(s, '%((%d+)%%%)'); end;
end

p = GetProgress("[/] 00:11:06 (93%)")
print(p)
assert(p == "93")

p = GetProgress("Decoding complete.")
print(p)
assert(p == "100")

-- GetProgress_OpusEnc

function GetProgress(s) 
  if string.match(s, '^Encoding complete%s-') ~= nil then return "100";
  else return string.match(s, '%((%d+)%%%)'); end;
end

p = GetProgress("[|] 00:22:45.04 (37%) 75.8x realtime, 193.2kbit/s")
print(p)
assert(p == "37")

p = GetProgress("Encoding complete")
print(p)
assert(p == "100")

-- GetProgress_QaacDec

function GetProgress(s) 
  if string.match(s, '%ssamples processed%s') ~= nil then return "100";
  else return string.match(s, '%[(%d+.%d+)%%%]'); end;
end

p = GetProgress("[59.9%] 2:34.830/4:18.660 (354.3x), ETA 0:00.293  ")
print(p)
assert(p == "59.9")

p = GetProgress("11406949/11406949 samples processed in 0:00.718")
print(p)
assert(p == "100")

-- GetProgress_QaacEnc

function GetProgress(s) 
  if string.match(s, '%ssamples processed%s') ~= nil then return "100";
  else return string.match(s, '%[(%d+.%d+)%%%]'); end;
end

p = GetProgress("[18.4%] 11:12.333/1:00:46.041 (79.8x), ETA 0:37.250 ")
print(p)
assert(p == "18.4")

p = GetProgress("11406949/11406949 samples processed in 0:03.281")
print(p)
assert(p == "100")

-- GetProgress_ReWaviDec

function GetProgress(s) 
  return string.match(s, '^Progress: (%d+)%%');
end

p = GetProgress("TODO")
print(p)
assert(p == "TODO")

-- GetProgress_RefalacEnc

function GetProgress(s) 
  if string.match(s, '%ssamples processed%s') ~= nil then return "100";
  else return string.match(s, '%[(%d+.%d+)%%%]'); end;
end

p = GetProgress("TODO")
print(p)
assert(p == "TODO")

-- GetProgress_ReturnAlwaysError

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_ReturnAlwaysSuccess

function GetProgress(s) 
  return "100";
end

p = GetProgress("")
print(p)
assert(p == "100")

-- GetProgress_SsrcEnc

function GetProgress(s) 
  return string.match(s, '(%d+)%% processed');
end

p = GetProgress("  10% processed")
print(p)
assert(p == "10")

-- GetProgress_TakcDec

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_TakcEnc

function GetProgress(s) 
  return "-1";
end

p = GetProgress("")
print(p)
assert(p == "-1")

-- GetProgress_TtaDec

function GetProgress(s) 
  return string.match(s, 'Progress: (%d+)%%');
end

p = GetProgress("Progress: 90%")
print(p)
assert(p == "90")

-- GetProgress_TtaEnc

function GetProgress(s) 
  return string.match(s, 'Progress: (%d+)%%');
end

p = GetProgress("Progress: 90%")
print(p)
assert(p == "90")

-- GetProgress_TwoLAME

function GetProgress(s) 
  return string.match(s, '((%d+))%%');
end

p = GetProgress("TODO")
print(p)
assert(p == "TODO")

-- GetProgress_WavPackEnc

function GetProgress(s) 
  if string.match(s, '^created') ~= nil then return "100";
  else return string.match(s, '(%d+)%% done'); end;
end

p = GetProgress("created 1.wv in 7.95 secs (lossless, 26.56%)")
print(p)
assert(p == "100")

p = GetProgress("creating 1.wv,  77% done...")
print(p)
assert(p == "77")

--- GetProgress_WavSplit

function GetProgress(s) 
  return string.match(s, '^Progress: (%d+)%%');
end

p = GetProgress("Progress: 100% (265533250/265533250)")
print(p)
assert(p == "100")

-- GetProgress_WvUnpackDec

function GetProgress(s) 
  if string.match(s, '^restored') ~= nil then return "100";
  else return string.match(s, '(%d+)%% done'); end;
end

p = GetProgress("restored 2.wav in 2.86 secs (lossless, 26.56%)")
print(p)
assert(p == "100")

p = GetProgress("restoring 2.wav,  20% done...")
print(p)
assert(p == "20")
