function sigma_ndr=std_distance_ratio(image)
%STD_DISTANCE_RATIO calculates the standard deviation of the normalized
%distance ratio of an object.
%
%[SIGMA]=STD_DISTANCE_RATIO(IMAGE) returns SIGMA a number between 0 and 1.
%IMAGE is an image mask where nonzero points are inside the object.
%
%STD_DISTANCE_RATIO is the standard deviation of the distance of contour
%points normalized by the maximum distance. A SIGMA of 0 is a perfect circle,
%a number of 1 would correspond to a star like pattern.

%find edge points, assuming that the image contour is well behaved.
    edge_points=bwboundaries(image);
    edge_points=edge_points{1};
    
    %get centroid
    centroid=mean(edge_points);
    
    %find the radius of the maximally inscribing circle
    circ_rad=max(sqrt(sum((edge_points-ones(length(edge_points),1)*centroid).^2,2)));
    
    %find the normalized distance ratio.
    distance_ratio=sqrt(sum((edge_points-ones(length(edge_points),1)*centroid).^2,2))/circ_rad;
	sigma_ndr=std(distance_ratio);
    
    
end