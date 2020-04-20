function Gabor_features = extract_Gabor3D( image, mask)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
for i=1:8
   theta1 = (i-1)*pi/3;
   for z=1:8
       theta2 = (z-1)*pi/3;
       for j=1:3
           lambda = 2^(j-1);% wavelength
           [gb_c1, ~] = gabor3dkerns(theta1, theta2, lambda,1);
           feature = imfilter(image,gb_c1,'same');
           temp = feature(logical(mask));
           Gabor_features((i-1)*120+(z-1)*15+(j-1)*5+1) = median(temp(:));
           Gabor_features((i-1)*120+(z-1)*15+(j-1)*5+2) = mean(temp(:));
           Gabor_features((i-1)*120+(z-1)*15+(j-1)*5+3) = std(temp(:));
           Gabor_features((i-1)*120+(z-1)*15+(j-1)*5+4) = skewness(temp(:));
           Gabor_features((i-1)*120+(z-1)*15+(j-1)*5+5) = kurtosis(temp(:));
       end
   end
end
end

