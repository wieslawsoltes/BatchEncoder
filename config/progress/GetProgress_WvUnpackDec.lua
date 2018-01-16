function GetProgress(s) 
  if string.match(s, '^restored') ~= nil then return "100";
  else return string.match(s, '(%d+)%% done'); end;
end
