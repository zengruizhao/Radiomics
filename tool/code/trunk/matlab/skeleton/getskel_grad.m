function [backbone,map]=getskel_grad(image_mask,percentile,dim)
 %set the image to get
 
   %calculate a euclidean distance map of the masked image (in three dimensions)
   type='euclidean';
   map=-bwdist(image_mask,type);
   in_dist_map=bwdist(~image_mask,type);
   map(image_mask)=in_dist_map(image_mask);
   
   %calculate the gradient on the distance map to get the internal skeleton
   if dim(3)>0
            [grad_x,grad_y,grad_z]=gradient(map,dim(1),dim(2),dim(3));
            grad_norm=sqrt(grad_x.^2+grad_y.^2+grad_z.^2);
   else
            [grad_x,grad_y]=gradient(map,dim(1),dim(2));
            grad_norm=sqrt(grad_x.^2+grad_y.^2);
   end
   
   %Filter over the gradient data to remove any local minimum outside of
   %the object or within a certain boundary
   %grad_norm=smooth3(grad_norm,'gauss',3);
   backbone=grad_norm(:,:,:)<prctile(grad_norm(map>0),percentile);
   
   image_mask2=map>2;
   backbone=image_mask2.*backbone;
%   backbone=smooth3
   if dim(3)<0
       backbone=bwmorph(backbone,'thin','Inf');
       backbone=bwmorph(backbone,'clean');
    
   end
end