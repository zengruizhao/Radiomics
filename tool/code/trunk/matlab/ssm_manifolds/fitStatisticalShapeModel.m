% fitStatisticalShapeModel takes a new_manifold and fits it to a SSMM.
% 
% The new_manifold is Nx1 vector where N is the number of points and dimensions,
% in the same structure as given by reshape.
%
% mean_manifold, eigenvector_manifold, eigenvalue_manifold are all direct
% outputs from generateStatsiticalShapeModel variables of the same name.
% 
% dim is the dimension of the manifold. 
% 
% b_max are the maximum deviation allowed by the variable. Pass as empty if
% all deviations are allowable (note this should return the original
% manifold and therefore should only be used for testing purposes).
%
% Created by: Rachel Sparks
function [fitManifold, fitD, b] = fitStatisticalShapeModel(new_manifold, mean_manifold, eigenvector_manifold, eigenvalue_manifold, dim, b_max)

b = ((new_manifold - mean_manifold)' * (eigenvector_manifold))'./sqrt(eigenvalue_manifold); % determine b

% change b if it is above the threshold.
if ~isempty(b_max)
    b(b>b_max)=b_max;
    b(b<-b_max)=-b_max;
end

% fit the manifold to the model.
fitModel   = mean_manifold + (eigenvector_manifold * (b.*sqrt(eigenvalue_manifold)));
model_size = size(fitModel);

%reshape returned values
fitManifold = reshape(fitModel(1:end-(dim)),[(model_size(1)-dim)/dim dim]);
fitD = fitModel(end+1-(dim):end,:);

end