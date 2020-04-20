% Takes a low dimensional projection from PCA and reconstructs the high
% dimensional original data.
% 
% Usage: output = CalculatePCAReconstruction( ...
%   means, eigenvectors, eigenvalues, projection, transformation, options);
% 
% eigenvalues is optional.  If eigenvalues == [], then the projections are
% considered to be in raw values, whereas if eigenvalues ~= [], then the
% projections are considered to be in standard deviations from the mean.
% 
% transformation (default = []) is an optional matrix containing an affine
%   transformation.  If it's empty, no transformation is applied.
% 
% options (default = []) is a struct containing the following members:
%   new_size (default = 1) is a value to pass to imresize before deforming
%   the image.  Greatly increases the speed when large images are to be
%   transformed.
%   interpolation (default = 'linear') is a value of how to interpolate the
%   image when applying a transformation
%   num_rows (default = size(means, 1))
%   num_cols (default = size(means, 2))
% 
% Created by Rob Toth
% 
function reconstruction = CalculatePCAReconstruction( ...
  means, eigenvectors, eigenvalues, projection, transformation, options)

  % initialize
  if ~exist('options', 'var') || isempty(options) || ~isstruct(options), options = struct; end
  if ~isfield(options, 'new_size'      ), options.new_size       = 1             ; end % by default, no rescaling
  if ~isfield(options, 'interpolation' ), options.interpolation  = 'linear'      ; end
  if ~isfield(options, 'num_rows'      ), options.num_rows       = size(means, 1); end
  if ~isfield(options, 'num_cols'      ), options.num_cols       = size(means, 2); end
  if ~exist('transformation', 'var'), transformation = []; end

  % get the size of the images
  size_m = size(means);
  size_e = size(eigenvectors);

  % invalid
  no_eigenvalues = isempty(eigenvalues);
  valid = ...
    numel(means) == prod(size_e(2:end)) && ...
    (no_eigenvalues || length(eigenvalues) == size_e(1));
  if ~valid
    reconstruction = means;
    return;
  end % invalid

  % extract the data
  if sum(projection ~= 0) > 0 % there is at least one projection value

    % reshape everything
    if ndims(means) > 2
      means = reshape(means, prod(size_m), 1);
    end
    if ndims(eigenvectors) > 2
      eigenvectors = reshape(eigenvectors, size_e(1)   , prod(size_e(2:end)));
    end

    % we must multiply by the square root of the eigenvalues
    if ~no_eigenvalues
      projection = projection .* sqrt(eigenvalues);
    end

    % calculate the inverse
    reconstruction = means + eigenvectors' * projection(:);

    % convert it back to the correct size
    reconstruction = reshape(reconstruction, size_m);

  else % no need to do all the calculations if b = 0
    reconstruction = means;
  end

  % optional affine transformation
  reconstruction = Transform(transformation, reconstruction, options);
    
end % function
