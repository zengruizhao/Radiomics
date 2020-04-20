% 
% Rescales the values in 'images' to a new min and max, retaining the
% relative values in the old images, between cells.
% 
% [rescaled, old_min, old_max] = RescaleAll(images, new_min, new_max);
% 
% If size(images{i}, 4) > 4, then it will keep each fourth dimension (each
% feature) as independent.
% 
% Created by Rob Toth.
% 
function [rescaled, old_min, old_max] = RescaleAll(images, new_min, new_max)

    num_images = length(images);
    num_features = size(images{1}, 4);
    
    for f = 1:num_features
        % find the min and max values in images
        images_min = ones(num_images, 1);
        images_max = images_min;
        for i = 1:num_images
            images_min(i) = min(min(min(images{i}(:, :, :, f))));
            images_max(i) = max(max(max(images{i}(:, :, :, f))));
        end % for i
        old_min = min(images_min);
        old_max = max(images_max);
        old_range = old_max - old_min;

        % what is the new range?
        new_range = new_max - new_min;

        % rescale the data
        rescaled = images; % initialize
        for i = 1:num_images

            % how far (normalized) into the range is the current image's min and max
            relative_min = (images_min(i) - old_min) / old_range;
            relative_max = (images_max(i) - old_min) / old_range;

            % what will the current images final min and max be
            current_min = relative_min * new_range + new_min;
            current_max = relative_max * new_range + new_min;
            current_range = current_max - current_min;

            % scale to be from new_min to new_max
            rescaled{i}(:, :, :, f) = jrescale(images{i}(:, :, :, f)) * (new_range) + new_min;

            % scale to the correct "relative" position within new_range
            rescaled{i}(:, :, :, f) = (current_range / new_range) * (rescaled{i}(:, :, :, f) - new_min) + current_min;

        end % for i
    end % for f

end % function