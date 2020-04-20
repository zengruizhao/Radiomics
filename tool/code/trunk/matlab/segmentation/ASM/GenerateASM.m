% 
% Usage: ASM = GenerateASM(features, masks, options);
% 
% Where features is a cell array of 4D feature images.
% The fourth dimension is which feature.
% masks is a cell array of 3D binary masks.
% 
% Created by Rob Toth
% 
function ASM = GenerateASM(features, masks, options)

  % options
  if ~exist('options', 'var') || isempty(options) || ~isstruct(options), options = struct; end
  if ~isfield(options, 'num_landmarks'), options.num_landmarks = 500; end

  % initialize
  fprintf('Generating ASM...');
  num_studies    = length(features);
  num_features   = size(features{1}, 4);
  image_size     = size(masks{1});
  surface_points = cell(num_studies, 1);
  feature_values = cell(num_studies, 1);
  for i = 1:num_studies
      surface_points{i} = ones(1, options.num_landmarks, 3);
      feature_values{i} = ones(1, options.num_landmarks, num_features);
  end % for i

  % get the surface points
  volume_size = image_size(1:3);
  for i = 1:num_studies
    
    surface_points{i}(1, :, :) = GetLandmarksFromMask(masks{i}, options);

    % get the feature values at the landmark points
    for k = 1:num_features
      current_feature = features{i}(:, :, :, k);
      indices = sub2ind(volume_size, ...
        surface_points{i}(1, :, 2), ...
        surface_points{i}(1, :, 1), ...
        surface_points{i}(1, :, 3));
      feature_values{i}(1, :, k) = current_feature(indices);
    end % for k

  end % for i
  
  surface_points = cell2mat(surface_points);
  feature_values = cell2mat(feature_values);

  % get the statistical shape model
  ASM = struct;
  [ASM.eigenvectors, ASM.eigenvalues, ASM.means] = ...
    CalculatePCAEmbedding(surface_points, options);
  
%   % testing
%   clf; scatter3(ASM.means(:, 1), ASM.means(:, 2), ASM.means(:,3), ...
%     ones(1, options.num_landmarks) * 30, 1:options.num_landmarks, 'filled');
  
  % get the statistical appearance model
  ASM.mean_f = squeeze(mean(feature_values, 1));
  ASM.cov_f = ones(num_features, num_features, options.num_landmarks);
  for l = 1:options.num_landmarks
      ASM.cov_f(:, :, l) = nancov(squeeze(feature_values(:, l, :)));
  end % for k

  fprintf('done.\n');
    
end % function
