function [ Feature_map, Gabor_features ] = extract_Gabor( image, mask)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
b = 1; psi = 0;gamma = 1;
for i=1:8
   theta = i*pi/8;
   for j=1:6
       lambda = 2^j;% wavelength
       [gb_c1, ~]=gabor2dkerns(theta,lambda,b,psi,gamma);
       feature = rescale_range(imfilter(image,gb_c1,'same'),0,255);
       Feature_map(:,:,6*(i-1)+j) = feature;
       temp = feature(mask==1);
       Gabor_features((i-1)*30+(j-1)*5+1) = median(temp(:));
       Gabor_features((i-1)*30+(j-1)*5+2) = mean(temp(:));
       Gabor_features((i-1)*30+(j-1)*5+3) = std(temp(:));
       Gabor_features((i-1)*30+(j-1)*5+4) = skewness(temp(:));
       Gabor_features((i-1)*30+(j-1)*5+5) = kurtosis(temp(:));
   end
end
end

