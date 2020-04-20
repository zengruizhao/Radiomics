% modelError takes in two manifolds and calculates SSD normalized by the variance. 
%
% Manifolds are assumed to be NxD, containing N points in D dimensions.
% Point correspondences are assumed known a priori.
%
% Created by: Rachel Sparks

function [normError]=normModelError(manifoldModel,manifoldOriginal,var)

%first need to reshape the manifold model
normError=sum(sqrt(((manifoldModel-manifoldOriginal)./var).^2),2);

end