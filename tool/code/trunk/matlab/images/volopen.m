function vol = volopen(fname,dims,prec,be)
%vol = volopen(fname,dims,prec,be)
%Function to open 3D volume datasets in MATLAB
%fname - Full path + filename in single quotes
%dims - Can be array in form of [X Y] dimensions if 3D. Should contain corr values.
%be - Big Endian(1)/Little Endian(0,PC,default)
%Else needs to contain values for each dimension in dataset.
%prec - Precision of data to be read in single quotes
%
%SV 30 Jul 2007

if nargin < 4 || ~be 
    fid = fopen(fname,'r');
else
    fid = fopen(fname,'r','ieee-be');
end

vol = fread(fid,prec);
if numel(dims) == 2
    z = size(vol,1)/(dims(1)*dims(2));
    dims = [dims z];
end
vol = reshape(vol,dims); %#ok<NASGU>
vol = eval([prec,'(vol)']);
fclose(fid);

end
