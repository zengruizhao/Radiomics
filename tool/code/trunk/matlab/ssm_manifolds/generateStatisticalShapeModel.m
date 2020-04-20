% generateStatisticalShapeModel takes in a set of manifolds and learns the
% mean manifold and primary modes of variation.
%
% manifolds is a NxDxk set of manifolds were N is the number of samples, 
% D is the dimensions of each sample and k is the number of manifolds.
% 
% d is the scaling factor associated with each manifold. 
%
% options.thresh can be set to determine how much variation the primary
% modes of variation should include. The default is 0.95 (corresponding to
% 2 standard deviations).
%
% Created by: Rachel Sparks

function [mean_manifold, eigenvector_manifold, eigenvalue_manifold]=generateStatisticalShapeModel(manifolds, d, options)

if ~isfield(options,'thresh')   
    options.thresh=.95; 
end

% prepare manifold for statistical analysis
manifold_size    = size(manifolds);
reshape_manifold = [reshape(manifolds,[manifold_size(1)*manifold_size(2) manifold_size(3)]); d];
    
% calculate mean
mean_manifold = mean(reshape_manifold,2);
    
% calculate modes of variation
center_manifold = reshape_manifold-repmat(mean_manifold, [1 manifold_size(3)]);
[eigenvector_manifold eigenvalue_manifold] = eig(center_manifold'*center_manifold);
 
% scale and recenter modes of variation 
eigenvalue_manifold = diag(eigenvalue_manifold);
sigma = diag(1./sqrt(eigenvalue_manifold));

eigenvector_manifold = center_manifold*eigenvector_manifold*sigma;

% calculate the number of eigenvalues to describe options.thresh level
% of variance
variance=flipud(cumsum(flipud(eigenvalue_manifold)/sum(eigenvalue_manifold)));
    
if options.thresh==1
    var_ind=1; % special case if we want to explain all the variation
else
    var_ind=find(variance>=options.thresh,1,'last');
end

eigenvector_manifold = eigenvector_manifold(:,var_ind:end);
eigenvalue_manifold  = eigenvalue_manifold(var_ind:end);

end