function GetProgress(s)
  if string.match(s, '%swrote%s') ~= nil then return "100";
  else return string.match(s, '(%d+)%%'); end;
end
