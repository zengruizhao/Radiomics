function Haralick_features = extract_Haralick_mean( image, gray_level )
%UNTITLED5 Summary of this function goes here
% Detailed explanation goes here
    for i=1:3
        ws = 2*i+1;
        Features = haralick2mex(image,gray_level,ws,1,0);
        for j=1:13
            temp = Features(:,:,j);
            Haralick_features((i-1)*13+j) = mean(temp(:));
        end
    end

end

