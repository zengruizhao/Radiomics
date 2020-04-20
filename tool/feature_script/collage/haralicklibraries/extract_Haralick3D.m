function [Haralick_features, Feature_map]= extract_Haralick3D(image, gray_level, mask, numofwin)
%UNTITLED5 Summary of this function goes here
% Detailed explanation goes here
Haralick_features = [];
for i=1:numofwin
    ws = 2*i+1;
    if ndims(image)==3
        Features = haralick3mex(image, gray_level, ws, 1, 0);
    else
        Features = reshape(haralick2mex(image, gray_level, ws, 1, 0), size(image, 1), size(image, 2), 1, 13);        
    end
    Feature_map{i, 1} = Features;
    for j=1:13
        slice_all = Features(:,:, :, j);
        temp1 = slice_all(logical(mask));
        Haralick_features((i-1)*65+(j-1)*5+1) = median(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+2) = mean(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+3) = std(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+4) = skewness(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+5) = kurtosis(temp1(:));
    end
end
end

