function [ newMask ] = fillMissingSlices( mask, oldInfo, newInfo )

% determine the missing slices
inds = find ( mask );
[~,~,k] = ind2sub( size(mask), inds );

maskSlices = sort(unique(k));

if length(maskSlices) == 1;
    slicePadding =  round ( (newInfo.PixelDimensions(3)/oldInfo.PixelDimensions(3))/2 );
    
    for i = (maskSlices-slicePadding) : (maskSlices+slicePadding)
      mask(:,:,i) = mask(:,:,maskSlices);
    end
    
    maskSlices = (maskSlices-slicePadding) : (maskSlices+slicePadding);
end

missingSlices = setdiff( maskSlices(1):maskSlices(end), maskSlices );

% fill in holes
for i=1:length( maskSlices )
  mask(:,:,maskSlices(i)) = imfill( mask(:,:,maskSlices(i)),'holes');
end

%interpolate
options.input_voxel_size  = oldInfo.PixelDimensions;
options.output_voxel_size = newInfo.PixelDimensions;
options.output_rows = newInfo.Dimensions(1);
options.output_cols = newInfo.Dimensions(2);
options.output_slices = newInfo.Dimensions(3);

newMask = addSlicesToMask( mask, missingSlices, options );

end