function GetProgress(s) 
  if string.match(s, '%ssamples decoded%s') ~= nil then return "100";
  else return "-1"; end;
end
