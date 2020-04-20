% modelError takes in two manifolds and calculates the SSD. 
%
% Manifolds are assumed to be NxD, containing N points in D dimensions.
% Point correspondences are assumed known a priori.
%
% Created by: Rachel Sparks
function [EuclideanError] = modelError(manifoldModel,manifoldOriginal)

%first need to reshape the manifold model
EuclideanError=sum(sqrt((manifoldModel-manifoldOriginal).^2),2);

end