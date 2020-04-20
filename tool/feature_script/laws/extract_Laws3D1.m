function [ Laws_features, Feature_map ] = extract_Laws3D1(image, mask)
%UNTITLED6 Summary of this function goes here
%   some cases mask is 2D, so the cases can't use the code
Features = lawsfilter3(image,5);
Feature_map{1} = Features;
for i=1:125
    slice_all = Features(:,:, :, i);
    temp1 = slice_all(logical(mask));
    Laws_features((i-1)*5+1) = median(temp1(:)); 
    Laws_features((i-1)*5+2) = mean(temp1(:)); 
    Laws_features((i-1)*5+3) = std(temp1(:)); 
    Laws_features((i-1)*5+4) = skewness(temp1(:)); 
    Laws_features((i-1)*5+5) = kurtosis(temp1(:)); 
end
end

