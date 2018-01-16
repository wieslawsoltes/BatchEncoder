function GetProgress(s) 
  return string.match(s, '^%s-(%d+.%d+)');
end
