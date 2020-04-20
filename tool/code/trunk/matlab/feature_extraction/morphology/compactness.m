function [compact]=compactness(image)
%COMPACTNESS calculates object compactness.
%
%[COMPACT]=COMPACTNESS(IMAGE). COMPACT is a number between 0 and 1.
%IMAGE is an image mask. where all points in the object are nonzero.
%
%COMPACTNESS is a ratio of perimeter to area of an object.

    %get boundary points, assumes the object is well behaved.
    edge_points=bwboundaries(image);
    edge_points=edge_points{1};
    
    %calculate the perimeter
    perim=sum(sqrt(sum( (edge_points-[edge_points(2:length(edge_points),:); edge_points(1,:)]).^2,2)));
    
    %calculate compactness ratio
    compact=(perim^2)/sum(image(:));
end