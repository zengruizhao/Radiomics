function volwrite(fname,v,prec)
%volwrite(fname,v,prec)
%Function to write volume datasets in MATLAB
%fname - Full path + filename in single quotes
%v - Array to be written
%prec - Precision of data to be read in single quotes
%
%SV 30 Jul 2007

fid = fopen(fname,'w');
fwrite(fid,v,prec);
fclose(fid);

end
