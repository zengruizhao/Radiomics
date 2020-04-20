% function [valid_neighbors W] = callaghan2(XY_centers, XY_neighbors,Tr,Tt,res)
function [valid_neighbors W] = callaghan2(binmap,Tr,Tt,res)
% for both outputs, each column is the neighborhood for a given center

[X Y] = find(binmap(:,:,1));
XY_neighbors = [X Y];
XY_centers = [X Y];

% valid_neighbors = false(size(XY_neighbors,1),size(XY_centers,1));
% W = zeros(size(XY_neighbors,1),size(XY_centers,1));
% for i=1:size(XY_centers,1)
%     [valid_neighbors(:,i) W(:,i)] = callaghan_c2(XY_centers(i,:), XY_neighbors, Tr, Tt, res);
% end

[valid_neighbors W] = callaghan_c(XY_centers, XY_neighbors, Tr, Tt, res);