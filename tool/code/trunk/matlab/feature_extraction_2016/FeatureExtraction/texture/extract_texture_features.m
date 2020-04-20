function [texture_vector] = extract_texture_features(I)

texture = texfeats(rgb2hsv(I));
feature_num = size(texture,3);
texture_vector = zeros(3*feature_num, 1);

for i = 1:feature_num
    featmap = texture(:,:,i);
    texture_vector(i) = mean2(featmap);
    texture_vector(feature_num+i) = std2(featmap);
    texture_vector(2*feature_num+i) = mode(featmap(:));
end


