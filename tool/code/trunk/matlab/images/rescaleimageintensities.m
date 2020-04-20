% 
% Rescales the values in 'images' to a new min and max, retaining the
% relative values in the old images, between cells.
% 
% [rescaled, old_min, old_max] = rescaleimageintensities(images, new_min, new_max);
% 
% images is a cell array
% 
% If size(images{i}, 4) > 1, then it will treat each fourth dimension (each
% feature) as independent and retain the relative range of each feature
% independently.
% 
% Created by Rob Toth.
% 
function [rescaled, old_min, old_max] = rescaleimageintensities( ...
  images, new_min, new_max)

  % check the inputs
  parser = inputParser;
  parser.addRequired('images', ...
    @(x) ...
    iscell(x) && length(x) >= 1  && ... % must be a cell
    all(cellfun(@ndims, x) >= 2) && ... % can't be <2D
    all(cellfun(@ndims, x) <= 4) && ... % can't be >4D
    all(cellfun(@isreal, x))   && ... % must be real images
    all(cellfun('size', x, 4) == size(x{1}, 4))); % must have the same # of features
  parser.addRequired('new_min', ...
    @(x) isscalar(x) && isreal(x));
  parser.addRequired('new_max', ...
    @(x) isscalar(x) && isreal(x));
  parser.parse(images, new_min, new_max);
  
  num_images   = length(images);
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
      rescaled{i}(:, :, :, f) = rescale(images{i}(:, :, :, f)) * ...
        (new_range) + new_min;

      % scale to the correct "relative" position within new_range
      rescaled{i}(:, :, :, f) = (current_range / new_range) * ...
        (rescaled{i}(:, :, :, f) - new_min) + current_min;

    end % for i
  end % for f

end % function