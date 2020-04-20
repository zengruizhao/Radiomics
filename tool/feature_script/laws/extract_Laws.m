function [ Laws_features ] = extract_Laws( image,  mask )
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
[lawsresponses, K2names] = lawsfilter(image,5);
for i=1:25
   temp = lawsresponses(:,:,i);
   temp1 = temp(mask==1);
   Laws_features((i-1)*5+1) = median(temp1(:)); 
   Laws_features((i-1)*5+2) = mean(temp1(:)); 
   Laws_features((i-1)*5+3) = std(temp1(:)); 
   Laws_features((i-1)*5+4) = skewness(temp1(:)); 
   Laws_features((i-1)*5+5) = kurtosis(temp1(:)); 
end
end

