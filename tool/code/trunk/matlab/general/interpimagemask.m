function new_image=interpimagemask(image_mask,scale)

  type='euclidean';

  image_mask=logical(image_mask);
  map=-bwdist(image_mask,type);
   in_dist_map=bwdist(~image_mask,type);
   map(image_mask)=in_dist_map(image_mask);
   
   
[x,y]=size(image_mask);

[xmesh,ymesh]=meshgrid([1:y],[1:x]);

xi=1:1/scale:x;
yi=1:1/scale:y;

[ximesh,yimesh]=meshgrid(yi,xi);

new_map=interp2(xmesh,ymesh,map,ximesh,yimesh);
new_image=new_map>0;
end