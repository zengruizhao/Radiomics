function [xi,yi,zi,vi]=im2vol(image_stack, dim, type,varargin)
    %this function will take in a set of evenly spaced images and turn them
    %into a volume matrix with roughly the same dimensions
    %there is a variable option argument that allows a user specified image
    %slice spacing as well as the new slice spacing. the default is the
    %slices are assumed to be evenly spaced and voxels are returned to be
    %as close to square as possible

    %for now asssume x,y dimension are the same.
    
    %get the dimensions of the image size
    im_size=size(image_stack);
   
    if size(varargin)>0
    %this is for user defined z_slices
        z_slices=varargin{1};
        new_z_slice=varargin{2};
    else
        %this is the default option assumes you want isotropic scaling.
        z_slices=dim(3):dim(3):(im_size(3)*dim(3));
        new_z_slice=dim(3):dim(1):(dim(3)*im_size(3));
    end
    %create a mesh grid for the image
    [x,y,z]=meshgrid(1:im_size(2),1:im_size(1),z_slices);
    

    %create the new mesh grid for in interpolated image
    [xi,yi,zi]=meshgrid(1:im_size(2),1:im_size(1),new_z_slice);
    
            %now use interp3 to create the same spacing between slices in the z
            %direction
                [vi]=interpn(y,x,z,image_stack,yi,xi,zi,type);


end