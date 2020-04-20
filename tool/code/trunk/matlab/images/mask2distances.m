% This function gets the distance to the surface of an object, given that
% object's mask.
% 
% Usage:
%   distances = mask2distances(mask);
%   distances = mask2distances(mask, options)
% 
% mask is a 2D or 3D logical matrix.
% 
% options is a struct containing the following optional fields:
% 
% input_voxel_size (default = [1, 1, 1]) is the size of a single voxel in
% [units_x, units_y, units_z]
% 
% distmethod (default = 'Euclidean') is the type of distance calculation
% used.  Note that if using anisotropic voxels, distmethod must be
% 'Euclidean'.
% 
% distances is in terms of "units" to the surface
% 
% Note that this function requires bwdistsc if voxel_size ~= [1, 1, 1]
%   which can be found at:
%   http://www.mathworks.cn/matlabcentral/fileexchange/15455-3d-euclidean-distance-transform-for-variable-data-aspect-ratio
% Created by Rob Toth, 2010-06-23
% 
function distances = mask2distances(mask, varargin)

  % check our inputs
  parser = inputParser;
  parser.StructExpand  = true;
  parser.KeepUnmatched = true;
  parser.addRequired('mask', ...
    @(x)islogical(x) && (ndims(x) == 2 || ndims(x) == 3));
  if ( verLessThan('matlab', '7.12.1') )
    parser.addParamValue('input_voxel_size', [1, 1, 1], ...
      @(x) isnumeric(x) && isvector(x) && (~any(isnan(x))) && (~any(x == 0)) && length(x) == 3);
  else % the above is depracated
    parser.addParamValue('input_voxel_size', [1, 1, 1], ...
      @(x)validateattributes(x, {'numeric'}, {'vector', 'numel', 3, 'nonnan', 'nonzero'}) );
  end
  distmethods = {'euclidean','cityblock','chessboard','quasi-euclidean'};
  parser.addParamValue('distmethod', 'Euclidean', ...
    @(x) any(strcmpi(x,distmethods)) );
  parser.parse(mask, varargin{:});
  
  % extract our input values
  input_results = parser.Results;
  voxel_size    = input_results.input_voxel_size;
  distmethod    = input_results.distmethod;
  isotropic     = all(voxel_size == 1);
  
  % calculate the distances to the surface
  % in modern versions, bwdistsc is slower than matlab's
  % but the only way to deal with anisotropic voxels
  if isotropic
    outside = bwdist( mask, distmethod);
    inside  = bwdist(~mask, distmethod);
  else % anisotropic
    if ~strcmpi(distmethod, 'Euclidean')
      error('Cannot have non-Euclidean and anisotropic voxels.');
    end
    if exist('bwdistsc', 'file') ~= 2
        error('Need bwdistsc for anisotropic voxels.');
    end
    outside = bwdistsc( mask, voxel_size);
    inside  = bwdistsc(~mask, voxel_size);
  end

  % combine the inside and outside distances
  distances = outside;
  distances(mask) = -inside(mask);

end
