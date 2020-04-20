function [smooth]=smoothness(image,options)
%SMOOTHNESS calculates object smoothness.
%
%[SMOOTH]=SMOOTHNESS(image) smooth is a positive number. 
%IMAGE is an image mask where nonzero points are in the object.
%OPTIONS may contain SCALE which is the pixel scaling
%SMOOTHNESS is a measure of contour smoothness.

    if ~exist('options', 'var') || isempty(options) || ~isstruct(options)
        options = struct;
    end
     
    %set defaults it no options are passed
    if ~isfield(options, 'scale'), options.scale         = 1  ; end

%find edge points, assuming that the image contour is well behaved
    edge_points=bwboundaries(image);
    edge_points=edge_points{1};
    
    %get centroid
    centroid=mean(edge_points);
    
    %find the number of points
    i_max=length(edge_points);
    
    %shift edge points by 1
    edge_shift_for=[edge_points(2:i_max,:);edge_points(1,:)];
    edge_shift_back=[edge_points(i_max,:);edge_points(1:i_max-1,:)];
    
    %find radial distance of edge points
    edge_centered=sqrt(sum((edge_points-ones(length(edge_points),1)*centroid*options.scale).^2,2));
    edge_for_centered=sqrt(sum((edge_shift_for-ones(length(edge_points),1)*centroid*options.scale).^2,2));
    edge_back_centered=sqrt(sum((edge_shift_back-ones(length(edge_points),1)*centroid*options.scale).^2,2));

    %calculate summation of difference of points and neighbors.
    smooth=sum(abs(edge_centered - ((edge_for_centered +edge_back_centered)/2)));
end