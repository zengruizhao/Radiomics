% [N, W] = callaghan_c( centroids , neighbors , Tr, Ta, res )
%
% I will make this more descriptive later.  Here are the basics.
%
% Inputs:
%     centroids [A x 2 double]: X and Y coordinates for A points of interest.
%                               O'Callaghan neighborhoods will be constructed for each row.
%     neighbors [B x 2 double]: X and Y coordinates for B potential neighbors.
%                               O'Callaghan neighborhoods will be constructed using these points.
%     Tr [double]: O'Callaghan distance constraint.
%                  All potential neighbors outside radius of Tr*[distance to nearest neighbor] are excluded.
%     Ta [double]: O'Callaghan direction constraint.
%                  All potential neighbors "behind" current neighbors (via angle Ta) are excluded.
%     res [double]: Scale factor used to normalize distance constraint across image resolutions.
%                   If you're not working with images or don't understand what this means, use 1.
%
% Outputs:
%     N [B x A logical]: neighborhood status of each potential neighbor (rows) for each point of interest (cols).
%     W [B x A double]: distances between each potential neighbor (rows) and each point of interest (cols).