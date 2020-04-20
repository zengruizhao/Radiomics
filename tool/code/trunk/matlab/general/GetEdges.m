% 
% Usage: edges = GetEdges(mask, use_caps);
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
function edges = GetEdges(mask, use_caps)

    if ~exist('use_caps', 'var') || isempty(use_caps), use_caps = true; end
    if size(mask, 3) == 1, use_caps = false; end % makes no sense for 2D
    
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
    edges = cast(edges, class(mask));

end % function