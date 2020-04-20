function [new_image, new_dim]=mask_interp(image,mask,varargin)
%mask_interp uses shape based interpolation to interpolate a mask and image
%
%Usage:
%  mask_interp(image, mask, options)
%
%Image is the image to be interpolated.
%
%mask is the image mask of the given image. It must be the same dimensions
%as the image.
%
%options is a struct contains the following optional fields:
%
%  input_voxel_size
%  output_voxel_size
%  interpmethod
%  output_rows
%  output_cols
%  output_slices
%
% 
%For the defaults and a description of the above parameters, 
%type "help resizeimage"
% 
%options also can include these optional fields:
%
%  cropping: a logical determining if image be cropped to only include the necessary
%   pixels. Default is false.
%  padding: a positive integer indicated how many pixels should be
%   interpolated in each direction. Default is zero.
%  return_mask: a logical determing if the returned volume should be the
%   mask or the image.
%  distance_map: a string either determining what type of 
%   distance map to use during interpolation.  Type "doc bwdist" to see a
%   list of valid types, in addition to the "chamfer" type.
% 

    distmethods = {'chamfer','euclidean','cityblock','chessboard','quasi-euclidean'};
    ip=inputParser;
    ip.StructExpand  = true;
    ip.KeepUnmatched = true;

    %set default parameters
    ip.addParamValue('cropping', false, @islogical);
    ip.addParamValue('padding',0,@(x)isnumeric(x) && numel(x)==3 && all(x>-1));
    ip.addParamValue('return_mask', false, @(x)islogical(x));
    ip.addParamValue('distance_map','euclidean',@(x)any(strcmpi(x,distmethods)));

	options=varargin{:};
	ip.parse(options);
	
    %Get the parameters
    bounding   =ip.Results.cropping;
    padding    =ip.Results.padding;
    return_mask=ip.Results.return_mask;
    dist_type  =ip.Results.distance_map;

    imsize=size(mask);
    %If bouding is chosen then bound the image
    if bounding
       [x,y,z]=ind2sub(size(mask),find(mask));
        mask=mask(min(x):max(x),min(y):max(y),min(z):max(z));
        image=image(min(x):max(x),min(y):max(y),min(z):max(z));
    end

    %then pad images:
    mask=padarray(mask,padding,'both');

    %get the distance map
    if strcmp(dist_type,'chamfer')
        dist_map=chamfer_map(logical(mask));    
    else
        dist_map=mask2distances(logical(mask), options);
    end

    %interpolate the distance map and convert it to a mask
    [new_mask, new_dim] = resizeimage(dist_map, options);
    new_mask = new_mask < 0;
    
    if return_mask;
        new_image=new_mask;
    else

        if imsize~=size(image)
            error('Image and Mask must be the same size');
        end
        image=padarray(image,padding,'both','replicate');

        new_image = resizeimage(image, options);
        
        oldclass=class(new_image);
        new_image=new_mask.*double(new_image);
        new_image=cast(new_image,oldclass);
    end % if return_mask

end % function
