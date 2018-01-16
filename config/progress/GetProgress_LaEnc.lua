function GetProgress(s) 
  r = string.match(s, '%*+');
  if r == nil then return "-1";
  else return tostring((string.len(r) * 100) / 24); end;
end
