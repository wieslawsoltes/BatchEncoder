function GetProgress(s) 
  c,t = string.match(s, 'Frame#%s-(%d+)/(%d+)');
  if (c == nil) or (t == nil) then return "-1";
  elseif (c == "0") or (t == "0") then return "0";
  else return tostring((100 * c) / t); end;
end
