function [Feature_map, CoLlAGe_features] = extract_collage(Raw_image,tumorMask,numofwin)
j=1;
for i=1:numofwin
    [feature,display] = compute_CoLlAGe2D_update(uint16(Raw_image), logical(tumorMask),0.5,0,i);
    Feature_map(:,:,(13*i-12):13*i) = display.fimg;
    CoLlAGe_features(j:i*65)=feature;
%     Dis(j:i*65) = display;
    j=j+65;
end