function GetProgress(s) 
  return string.match(s, '(%d+)%% decoded');
end
