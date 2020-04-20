function [datafile,datatype] = mhdread(mhdfile)
%function [datafile,datatype] = mhdread(mhdfile)
%Reads MHD file to return associated filename and datatype 
%
%SV, Jul 2010

fid = fopen(mhdfile,'r');
headerinfo = textscan(fid,'%s',20,'delimiter','\n');
headerinfo = headerinfo{1};
idx = strfind(headerinfo,'ElementType');
datatypeloc = find(~cellfun('isempty',idx));
idx = strfind(headerinfo{datatypeloc},'=')+2;
switch lower(headerinfo{datatypeloc}(idx:end))
    case 'met_short'
        datatype = 'uint16';
end
idx = strfind(headerinfo,'ElementDataFile');
datafileloc = find(~cellfun('isempty',idx));
idx = strfind(headerinfo{datafileloc},'=')+2;
datafile = headerinfo{datafileloc}(idx:end);
        
end


