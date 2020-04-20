function [ Feature_map, Haralick_features ] = extract_Haralick( image, gray_level, mask, numofwin)
%UNTITLED5 Summary of this function goes here
% Detailed explanation goes here
for i=1:numofwin
    ws = 2*i+1;
    Features = haralick2mex(image, gray_level, ws, 1, 0);
    Feature_map(:,:,(13*i-12):13*i) = Features;
    for j=1:13
        temp = Features(:,:,j);
        temp1 = temp(mask==1);
        Haralick_features((i-1)*65+(j-1)*5+1) = median(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+2) = mean(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+3) = std(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+4) = skewness(temp1(:));
        Haralick_features((i-1)*65+(j-1)*5+5) = kurtosis(temp1(:));
    end
end

end

