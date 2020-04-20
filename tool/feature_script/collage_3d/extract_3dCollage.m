function [ feature, Feature_map ] = extract_3dCollage(img, label, biggest_slice_idx, numwin)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    upperSlice_idx = biggest_slice_idx + 1;
    lowerSlice_idx = biggest_slice_idx - 1;
    if upperSlice_idx > size(img, 3); upperSlice_idx = biggest_slice_idx;end
    if lowerSlice_idx < 1; lowerSlice_idx = biggest_slice_idx;end
    origImage = img(:, :, biggest_slice_idx);
    origMask = label(:, :, biggest_slice_idx);
%     origMask = imerode(origMask, strel('disk', 1));% 3 pixels
    upperSlice = img(:, :, upperSlice_idx);
    lowerSlice = img(:, :, lowerSlice_idx);
    for j=1:numwin
        [feat1_mask, feat2_mask] = compute_CoLlAGe3D(origImage,upperSlice,lowerSlice, origMask, j);
        Feature_map{j, 1} = feat1_mask;
        Feature_map{j, 2} = feat2_mask;
        for i=1:13
            temp = feat1_mask(:,:,i);
            temp = temp(~isnan(temp));
            feature(130 * (j - 1) + 5*(i-1) + 1) = mean(temp(:));
            feature(130 * (j - 1) + 5*(i-1) + 2) = median(temp(:));
            feature(130 * (j - 1) + 5*(i-1) + 3) = std(temp(:));
            feature(130 * (j - 1) + 5*(i-1) + 4) = skewness(temp(:));
            feature(130 * (j - 1) + 5*(i-1) + 5) = kurtosis(temp(:));
        end
        for i=1:13
            temp = feat2_mask(:,:,i);
            temp = temp(~isnan(temp));
            feature(130 * (j - 1) + 65 + 5*(i-1) + 1) = mean(temp(:));
            feature(130 * (j - 1) + 65 + 5*(i-1) + 2) = median(temp(:));
            feature(130 * (j - 1) + 65 + 5*(i-1) + 3) = std(temp(:));
            feature(130 * (j - 1) + 65 + 5*(i-1) + 4) = skewness(temp(:));
            feature(130 * (j - 1) + 65 + 5*(i-1) + 5) = kurtosis(temp(:));
        end
    end
end

