function GetProgress(s) 
  if string.match(s, '^Decoding complete%s-') ~= nil then return "100";
  else return string.match(s, '%((%d+)%%%)'); end;
end
