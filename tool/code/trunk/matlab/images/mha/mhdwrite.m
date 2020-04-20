function mhdwrite(dirname,fname,data)
%Writing MHD files for Inhomogeneity Correction via ITK
%function mhdwrite(fname,data)
%fname - Full path + filename in single quotes
%data - Array whose metadata needs to be written
%
%See INHOMO_CORRN
%SV,Apr 12 2008
if ~exist('dirname','var') || isempty(dirname)
    fid = fopen(fname,'w');
else
    fid = fopen([dirname,fname],'w');
end
fprintf(fid,'NDims = %d\n', length(size(data)));
if size(data,3)~=1
    fprintf(fid,'DimSize = %d %d %d\n', size(data));
else
    fprintf(fid,'DimSize = %d %d\n', size(data));
end
fprintf(fid,'ElementSpacing = 1 1 1\n');
fprintf(fid,'Position = 0 0 0\n');
fprintf(fid,'ElementByteOrderMSB = False\n');
if isa(data,'single')
    fprintf(fid,'ElementType = MET_FLOAT\n');
elseif isa(data,'uint16')
    fprintf(fid,'ElementType = MET_SHORT\n');
end    
if ~exist('dirname','var') || isempty(dirname)
    namestart = strfind(fname,'\');
    if isempty(namestart), namestart = strfind(fname,'/')+1; end
else
    namestart = 1;
end
fprintf(fid,'ElementDataFile = %s',fname(namestart(end):end-4));

fprintf(fid, data, class(data));

fclose(fid);

end