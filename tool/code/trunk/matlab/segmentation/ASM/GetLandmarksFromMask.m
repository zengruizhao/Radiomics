% 
% Usage: landmarks = GetLandmarksFromMask( mask, options )
% 
% mask is a logical image containing true at every pixel inside
% the object of interest (2D or 3D)
% 
% landmarks will be a num_landmarks x dims dataset containing the Cartesian
% coordinates of the landmarks.  x in the first row, y in the second row,
% and z in the third row.  If mask is 2D, then no third row.
% 
% options is a struct containing the following optional fields:
%   num_landmarks (default = 500) how many landmarks to extract?
%   use_caps (default = true) should we "cap" the 3D mask at each end?
%     (ignored in 2D)
% 
function landmarks = GetLandmarksFromMask( mask, options )

  % options
  if ~exist('options', 'var') || isempty(options) || ~isstruct(options), options = struct; end
  if ~isfield(options, 'use_caps'     ), options.use_caps   = true; end
  if ~isfield(options, 'num_landmarks'), options.num_landmarks = 500; end

  % extract our relevant options
  num_landmarks = uint32 ( options.num_landmarks );
  use_caps      = logical( options.use_caps );
  
  % check our input
  dims       = ndims(mask);
  image_size = size (mask);
  if ...
      ~islogical( mask ) || ...
      dims > 3           || ...
      dims < 2
    error( 'mask input is invalid in GetLandmarksFromMask' );
  end
    
  % prepare our output
  landmarks = ones(num_landmarks, dims);
  
  % get the edges
  edges = GetEdges(mask, use_caps);
%   if dims == 3, clf; isosurface( edges, .5 ); % testing
  indices = find(edges);

  % get the surface points at those edges
  points = ones(size(indices, 1), 3);
  [points(:, 2), points(:, 1), points(:, 3)] = ind2sub(image_size, indices);

  % angle will range between -pi and +pi
  centroid = mean(points, 1);
  differences = points - repmat(centroid, size(points, 1), 1);
  radii = sqrt(sum(differences(:, 1:2).^2, 2));
  angle = asin(differences(:, 2) ./ radii);
  angle(differences(:, 1) <  0) = pi - angle(differences(:, 1) < 0);
  angle = angle - pi/2;

  % split it up into segments of grouped angles
  num_unique = 50;
  unique_angles = linspace(min(angle), max(angle), num_unique);

  % find the closest group
  differences = abs(repmat(angle, 1, num_unique) - repmat(unique_angles, size(angle, 1), 1));
  [~, which_angle] = min(differences, [], 2);
  angle = unique_angles(which_angle);

  % sort by grouped angle, then by z
  [~, order] = sortrows(cat(2, points(:, 3), angle(:)), [2, -1]);
  points     = points(order, :);
  angle      = angle (order);

  % now, we only want a few points per angle group
  points_per_angle = round(num_landmarks / num_unique);
  for group_index = 1:num_unique
    
    % extract the points from our angle group
    points_in_group = points(angle == unique_angles(group_index), :);
    
    % only select some (points_per_angle) of them
    relevant_indices = ceil(linspace(1, size(points_in_group, 1), points_per_angle));
      
    % assign it to our output
    output_start_index = (group_index - 1) * points_per_angle + 1;
    output_end_index   = group_index       * points_per_angle;
    
    landmarks( output_start_index : output_end_index, : ) = ...
      points_in_group(relevant_indices, 1:dims);
    
  end % for group_index
  
%   % testing
%   clf;
%   scatter_size = 30;
%   if dims == 2
%     scatter(landmarks(:, 1), landmarks(:, 2), ...
%       ones(1, num_landmarks) * scatter_size, ...
%       1:num_landmarks, 'filled' );
%   elseif dims == 3
%     scatter3(landmarks(:, 1), landmarks(:, 2), landmarks(:, 3), ...
%       ones(1, num_landmarks) * scatter_size, ...
%       1:num_landmarks, 'filled' );
%   end % dims

end % GetLandmarksFromMask