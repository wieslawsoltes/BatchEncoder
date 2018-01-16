function GetProgress(s) 
  if string.match(s, '^Decoding') ~= nil then return "100";
  else return string.match(s, '(%d+)%%'); end;
end
