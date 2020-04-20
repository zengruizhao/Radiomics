function db = ReadEYEImage( filename, isLogCompressed )

% read eye file
[RF,~] = ReadEye(filename);

% display true transducer voltage output
if( isLogCompressed )
  env = abs(hilbert(RF));
  db  = 20*log10(env/max(env(:)));
else
    db = RF;
end

end


