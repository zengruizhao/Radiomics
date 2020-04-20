function [ Laws_features, Feature_map ] = extract_Laws3D(image, mask)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
for j=1:size(image, 3)
    Features = lawsfilter(image(:,:,j),5);
    for z=1:25
       TEMP{z, j} = Features(:,:,z); 
    end
end
Feature_map{1} = TEMP;
for i=1:25
   slice_temp = TEMP(i, :);
   for z=1:size(slice_temp, 2)
       slice_all(:,:,z)  = slice_temp{z};
   end
   temp1 = slice_all(logical(mask));
   Laws_features((i-1)*5+1) = median(temp1(:)); 
   Laws_features((i-1)*5+2) = mean(temp1(:)); 
   Laws_features((i-1)*5+3) = std(temp1(:)); 
   Laws_features((i-1)*5+4) = skewness(temp1(:)); 
   Laws_features((i-1)*5+5) = kurtosis(temp1(:)); 
end
end

