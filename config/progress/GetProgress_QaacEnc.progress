function GetProgress(s) 
  if string.match(s, '%ssamples processed%s') ~= nil then return "100";
  else return string.match(s, '%[(%d+.%d+)%%%]'); end;
end
