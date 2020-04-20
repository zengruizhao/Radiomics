function FEATURE = crop_mean_intensity(img, label)
    Raw_image = img;
    tumorMask = label;
    [r_mask,c_mask]=find(logical(tumorMask)==1);
    c_min=min(c_mask);c_max=max(c_mask);r_min=min(r_mask);r_max=max(r_mask);
    image = Raw_image(r_min:r_max,c_min:c_max);
    gray_level = 128;
    small_mask = tumorMask(r_min:r_max,c_min:c_max);
    image = round(rescale_range(image,0,gray_level-1));% Rescale
%     Haralick_features  = extract_Haralick_mean( image, gray_level );
%     temp = image(small_mask==1);
%     intensity = mean(temp);
       %% 117
%         [Haralick_feature_map, Haralick_features] = extract_Haralick(image , gray_level, small_mask, 3);
       %% 25
%         Laws_features = extract_Laws(image, small_mask);
       %% 48
%         LBP_features = extract_LBP(image, small_mask);
       %% 3 Gabor 240
        [Gabor_feature_map, Gabor_features] = extract_Gabor(image, small_mask);
         %% CoLlAGe 65*numofwin 195
        [CoLlAGe_feature_map, CoLlAGe_features] = extract_collage(Raw_image, tumorMask, 3);
       %% Gray_level 75 Langbin
%         GreyLevel_features = extract_gray_lavel(image,3);
%         FEATURE = [Haralick_features, Laws_features, LBP_features, Gabor_features, CoLlAGe_features, GreyLevel_features, intensity];
        FEATURE = [Gabor_features, CoLlAGe_features];
end
