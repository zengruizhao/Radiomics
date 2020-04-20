function [convex]=convexity(image)
%CONVEXITY calculates object convexity.
%
%[CONVEX]=CONVEXITY(IMAGE) returns CONVEX a number between 0 and 1.
%IMAGE is an image mask, where all points inside the object are positive
%
%CONVEXITY calculates ratio of the perimeter of the convex hull which
%inscribes the object to the perimeter of the object.

    %get boundary points, assuming the object is well behaved.
    edge_points=bwboundaries(image);
    edge_points=edge_points{1};
    
    %find the convext hull of the perimeter
    convex_points=edge_points(convexHull(DelaunayTri(edge_points(:,1),edge_points(:,2))),:);
    
    %calculate convex hull perimeter
    convex_perim=sum(sqrt(sum( (convex_points-[convex_points(2:length(convex_points),:); convex_points(1,:)]).^2,2)));
    
    %calculate object perimeter
    perim=sum(sqrt(sum( (edge_points-[edge_points(2:length(edge_points),:); edge_points(1,:)]).^2,2)));
    
    %calculate convex ratio.
    convex=convex_perim/perim;
end