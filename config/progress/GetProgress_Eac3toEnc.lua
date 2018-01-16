function GetProgress(s) 
  if string.match(s, '^Done.%s-') ~= nil then return "100";
  else return string.match(s, 'process: (%d+)%%'); end;
end
