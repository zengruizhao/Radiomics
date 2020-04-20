function [segmentation, landmarks] = SegmentUsingASM(ASM, feature_image)

  % initialize
  size_f = size(feature_image);
  num_landmarks = size(ASM.means, 1);
  feature_image = reshape(feature_image, prod(size_f(1:3)), size_f(4));
  indices = ones(num_landmarks, 1);

  % minimize the mahalanobis distance for each landmark
  mean_f = ASM.mean_f;
  cov_f = ASM.cov_f;
  parfor l = 1:num_landmarks

    % subtract out the mean
    mean_subtracted = feature_image - repmat(mean_f(l, :), size(feature_image, 1), 1);

    % calculate the mahalanobis distance for each point
    mahalanobis = sum(mean_subtracted / cov_f(:, :, l) .* mean_subtracted, 2);

    % find the location with the minimum mahalanobis
    indices(l) = find(mahalanobis == nanmin(mahalanobis), 1);

  end % for l

  % convert the indices to landmarks    
  landmarks = ones(num_landmarks, 3);
  [landmarks(:, 2), landmarks(:, 1), landmarks(:, 3)] = ind2sub(size_f(1:3), indices);

  % align the landmarks onto the mean markers
  temp = ones(num_landmarks, 1);
  temp_l = cat(2, landmarks, temp);
  temp_m = cat(2, ASM.means, temp);
  T = temp_l \ temp_m;
  landmarks_aligned = temp_l * T;
  landmarks_aligned = landmarks_aligned(:, 1:3);

  % calculate the optimal projections
  b = CalculatePCAProjection(ASM.means, ASM.eigenvectors, ASM.eigenvalues, landmarks_aligned);

  % ASM's limit the standard deviations to +/- 3
  b(b > +3) = +3;
  b(b < -3) = -3;

  % apply everything to landmarks
  new_landmarks = reshape(InvertPCA(ASM.means(:), ASM.eigenvectors, ASM.eigenvalues, b), size(landmarks));
  new_landmarks = cat(2, new_landmarks, temp) / T; % unalign
  new_landmarks = new_landmarks(:, 1:3);

  % triangulate output
  landmarks = new_landmarks;
  [t, tnorm] = MyRobustCrust(landmarks);

  % turn output into a mask
  segmentation = false(size_f(1:3));
  min_l = floor(min(new_landmarks, [], 1));
  max_l = ceil(max(new_landmarks, [], 1));
  [mesh_x, mesh_y, mesh_z] = meshgrid(min_l(1):max_l(1), min_l(2):max_l(2), min_l(3):max_l(3));
  inside = InPolyedron(new_landmarks, t, tnorm, cat(2, mesh_x(:), mesh_y(:), mesh_z(:)));
  segmentation(min_l(2):max_l(2), min_l(1):max_l(1), min_l(3):max_l(3)) = reshape(inside, size(mesh_x));

end % function
