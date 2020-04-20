% Usage:
%   edges = GetEdges(mask);
%   edges = GetEdges(mask, use_caps);
% 
% This function will extract the surface from a mask.
% 
% use_caps (default = true) is whether or not to find the first and last
% slice of the mask, and to not extract edges from those slices.  This will
% inherently "close off" holes at the top and bottom of the mask.  Think of
% a cylinder, with either open holes at each end, or caps at each end.
% 
% Created by Rob Toth.
% 
function edges = mask2edges(mask, varargin)

  % check our inputs
  parser = inputParser;
  parser.addRequired('mask', ...
    @(x)islogical(x) && (ndims(x) == 2 || ndims(x) == 3));
  parser.addOptional('use_caps', true, ...
    @(x)islogical(x) && isscalar(x));
  parser.parse(mask, varargin{:});
  
  % extract our input values
  input_results = parser.Results;
  if size(mask, 3) == 1
    % caps makes no sense for 2D
    use_caps = false;
  else
    use_caps = input_results.use_caps;
  end

  % initialize
  [num_rows, num_cols, num_slices] = size(mask);
  edges = zeros(num_rows, num_cols, num_slices);

  % calculate the edge of each mask
  any_edges = false(num_slices, 1);
  for k = 1:num_slices
    edges(:, :, k) = edge   (mask (:, :, k));
    any_edges(  k) = any(any(edges(:, :, k)));
  end % for k

  if use_caps

    % where are the caps?
    first_slice = find(any_edges, 1, 'first');
    last_slice  = find(any_edges, 1, 'last' );

    % make the caps just the original mask (not an edge)
    edges(:, :, first_slice) = mask(:, :, first_slice);
    edges(:, :, last_slice ) = mask(:, :, last_slice );

  end % if use_caps

  % cast it back
  edges = cast(edges, 'logical');

end % function