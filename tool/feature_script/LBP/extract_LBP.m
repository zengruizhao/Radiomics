function [ LBP_features ] = extract_LBP( image, mask )
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here
texture_lbp = [];
for jj = 0:7
    [texture_lbp] = cat(3, texture_lbp, lbp(image,jj));
end
for jj = 0:7
    [texture_lbp] = cat(3, texture_lbp, lbp(image,jj,false));
end
n=0;
for i = 1:size(texture_lbp,3)
    map = texture_lbp(:,:,i);
    temp = map(mask==1);
    LBP_features(n*3+1) = mean(temp(:));
    LBP_features(n*3+2) = median(temp(:));     
    LBP_features(n*3+3) = std(temp(:));
    n=n+1;
end

end

