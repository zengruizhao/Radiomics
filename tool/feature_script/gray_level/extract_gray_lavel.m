function [ Feature ] = extract_gray_lavel(image, window_size, mask)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    GreyLevel_features = GreyLevel2(image,window_size);
    GreyLevel_features = reshape(GreyLevel_features, ...
                                size(GreyLevel_features,1), ...
                                size(GreyLevel_features,2), ...
                                size(GreyLevel_features,3)*size(GreyLevel_features,4));
    for i=1:size(GreyLevel_features,3)
        temp = GreyLevel_features(:,:,i);
        feature_map = temp(mask==1);
        Feature((i-1)*5+1) = skewness(feature_map(:));
        Feature((i-1)*5+2) = kurtosis(feature_map(:));
        Feature((i-1)*5+3) = median(feature_map(:));
        Feature((i-1)*5+4) = std(feature_map(:));
        Feature((i-1)*5+5) = mean(feature_map(:));
    end
        
end

