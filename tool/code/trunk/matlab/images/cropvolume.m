function croppedvolume = cropvolume(vol,tempsize,zoffset)

if nargin<3, zoffset=0; end

croppedvolume=imshave(vol,tempsize);

if size(vol,3)>tempsize(3),
    % Slices
    extraslices=size(vol,3)-tempsize(3);
    croppedvolume=croppedvolume(:,:,(1:end-extraslices)+zoffset);
end

end
