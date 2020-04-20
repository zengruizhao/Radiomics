% This function gets the distance to the surface of an object, given that
% object's mask.
% 
% Usage: distances = GetDistances(mask, voxel_size, use_caps)
% 
% mask is a 2D or 3D matrix.
% voxel_size (default = [1, 1, 1]) is the size of a single voxel in [units_x, units_y, units_z]
% use_caps (default = true) is whether or not to make the top and bottom
%   slices of the mask closed.  This is useful for closed surfaces of
%   "foreground" objects, but performs poorly for "background" masks.  If
%   mask is 2D, use_caps = false regardless of what you set.
% 
% distances is in terms of "units" to the surface
% 
% Note that this function requires bwdistsc if voxel_size ~= [1, 1, 1]
%   which can be found at:
%   http://www.mathworks.cn/matlabcentral/fileexchange/15455-3d-euclidean-distance-transform-for-variable-data-aspect-ratio
% Created by Rob Toth, 23-June-2010
% 
function [distances, edges] = GetDistances(mask, voxel_size, use_caps)

    
    if ~exist('voxel_size', 'var') || isempty(voxel_size), voxel_size = [1, 1, 1]; end
    if ~exist('use_caps', 'var') || isempty(use_caps), use_caps = true; end
    
    % get the surface edge
    edges = GetEdges(mask, use_caps);

    % calculate the distances to the surface
    if (sum(voxel_size == [1, 1, 1]) == 3) || (exist('bwdistsc', 'file') ~= 2)
        distances = bwdist(edges);
    else
    	distances = bwdistsc(edges, voxel_size);
    end
    
    % make the "inside" distances negative
    distances(mask) = -distances(mask);

end
