function [major_axis minor_axis]= fitEllipseToModel(bounds)
%FITELLIPSETOMODEL calculates the axes of an ellipse fit to a medial axis
% shape model for a set of objects
%
% [major_axis minor_axis]=  fitEllipseToModel(bounds).
% bounds is a struct with bounds().r storing the x-axis location of points
% and bounds().c storing the y-axis location of points.
%
% Returns the major and minor axis of the object as determined by fitting an
% ellipse to the boundary of the medial axis shape model
%
% Author: Rachel E. Sparks 
% Date:   October, 20, 2012

    major_axis = zeros([ length(bounds) 2]);
    minor_axis = zeros ([length(bounds) 2]);
    
for i=1:length(bounds);

    % translate object so the range of the location is [1, inf] for x and y
    % location
    centered_r = ((bounds(i).r-min(bounds(i).r)))+1;
    centered_c = ((bounds(i).c-min(bounds(i).c)))+1;

    % extract the edge of the gland and make into a mask
    edge_gland       = zeros( ceil( max([centered_c; centered_r], [], 2))');
    [inds]           = sub2ind( size(edge_gland), round(centered_c), round(centered_r));
    edge_gland(inds) = 1;

    % get the image of the gland
    image_gland = imfill(edge_gland,'holes');

    % rescale gland up to ensure subtle morphologic cues are captures
    image_gland = padarray(image_gland,[2 2],'both');
    rescale_size= size(image_gland)*(100/(min(size(image_gland))));
    
    options.output_rows = rescale_size(1);
    options.output_cols = rescale_size(2);
    options.return_mask = true;
    
    image_gland = mask_interp(image_gland, image_gland, options);
        
    % get the medial atoms (call the subfunction as surface vectors will not
    % be needed)
    percentile = 5;
    dim =[1 1 0];
    [backbone]=getskel_grad(image_gland,percentile,dim);

    % get the spatial coordinates of the backbone and find the principle
    % component
    [row, column] = find(backbone);
    components = princomp([column row]);
    
    major_axis(i,:) = components(:,1);
    minor_axis(i,:) = components(:,2);
end

end
