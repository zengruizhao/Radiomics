function arr=average_radial_ratio(image)
%AVERAGE_RADIAL_RATIO calculates object average radial ratio (ARR).
%
%[ARR]=AVERAGE_RADIAL_RATIO(IMAGE) returns ARR a number between 0 and 1.
%IMAGE is an image mask, where all points inside the object are nonzero.
%
%Average Radial Ratio calculates ratio of the radial distance of the object
%contour over the radial distance of the minimally inscribing sphere.

%get boundary points, assuming image boundary is well behaved (no holes).
    edge_points=bwboundaries(image);
    edge_points=edge_points{1};
    
    %get centroid
    centroid=mean(edge_points);
    
    %find the radius of the maximally inscribing circle
    circ_rad=max(sqrt(sum((edge_points-ones(length(edge_points),1)*centroid).^2,2)));
    
    %calculate arr
    arr=sum(sqrt(sum((edge_points-ones(length(edge_points),1)*centroid).^2,2)))/(length(edge_points)*circ_rad);
end