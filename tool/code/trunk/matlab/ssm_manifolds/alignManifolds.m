% alignManifolds takes in a set of manifolds and aligns them.
%
% manifolds is a NxDxk set of manifolds were N is the number of samples, 
% D is the dimensions of each sample and k is the number of manifolds.
%
% options.reference can be selected to denote which manifold should be the
% reference manifold to align all other manifolds to. Default is 1.
%
% Created by: Rachel Sparks

function newManifolds = alignManifolds(manifolds,options)

if ~isfield(options,'reference')   
    options.reference = 1;
end

% initialize manifold size
newManifolds = zeros(size(manifolds));

% loop through manifolds and perform alignment using procrustes.
for i = 1:size(manifolds,3)
    
    if i == options.reference
        newManifolds(:,:,i)     = manifolds(:,:,i);
    else
        [~,newManifolds(:,:,i)] = procrustes(manifolds(:,:,options.reference), manifolds(:,:,i));
    end    
    
end % for i = 1:size(manifolds,3)

end