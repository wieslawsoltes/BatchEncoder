function GetProgress(s) 
  if string.match(s, '^Done encoding') ~= nil then return "100";
  else return string.match(s, '%[%s-(%d+.%d+)%%%]'); end;
end
