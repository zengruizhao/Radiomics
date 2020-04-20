function aor=area_overlap_ratio(image)
%%AREA_OVERLAP_RATIO calculate object area overlap ratio(AOR).
%
%[AOR]=AREA_OVERLAP_RATIO(IMAGE) returns AOR, a number between 0 to 1. 
%IMAGE is an image mask, where points inside the image mask are positive.  
%
%Area Overlap Ratio is defined as the area of the object over the area of
%the minimally inscribing circle.

%if the image is not a binary image mask, make it one. This assumes
%that numbers greater than 0 are inside the image.
   if ~islogical(image)
        image=image/max(image(:));
        image=image>0;
   end %if ~islogical
   
    %find edge points on the image mask, this assumes the boundary of the
    %object is well behaved
    edge_points=bwboundaries(image);
    edge_points=edge_points{1};
   
    %get centroid
    centroid=mean(edge_points);
    
    %find the radius of the maximally inscribing circle
    circ_rad=max(sqrt(sum((edge_points-ones(length(edge_points),1)*centroid).^2,2)));
    
    %calculte ratio
    aor=sum(image(:))/(pi*circ_rad^2);

end %function