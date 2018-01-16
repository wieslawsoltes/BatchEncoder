function GetProgress(s)
  if string.match(s, 'done') ~= nil then return "100";
  else return string.match(s, '(%d+)%%'); end;
end
