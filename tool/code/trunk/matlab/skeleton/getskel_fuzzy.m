function [backbone,map]=getskel_fuzzy(mask,percentile,dim,options)
 %set the image to get
    
    %options
    if ~exist('options', 'var') || isempty(options) || ~isstruct(options)
        options = struct;
    end
    
    if ~isfield(options, 'use_caps' ), options.use_caps    = false; end
 
   %calculate a euclidean distance map of the masked image (in three dimensions)

   [map, edges] = getDistances(mask, dim, options.use_caps);
   %calculate the gradient on the distance map to get the internal skeleton
   if dim(3)>0
            [grad_x,grad_y,grad_z]=gradient(map);
            grad_norm=sqrt(grad_x.^2+grad_y.^2+grad_z.^2);
   else
            [grad_x,grad_y]=gradient(map,dim(1),dim(2));
            grad_norm=sqrt(grad_x.^2+grad_y.^2);
   end
   
   %Filter over the gradient data to remove any local minimum outside of
   %the object or within a certain boundary
   backbone=grad_norm(:,:,:)<prctile(grad_norm(:),percentile);
   
   image_mask2=map<-6;
   backbone=image_mask2.*backbone;
   
end