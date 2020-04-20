function [skeleton,skeleton_rad]=get_fuzzy_skeleton(image_mask,image_edge,dim,atom_num,z_step,percentile,norm_num)
%get_skeleton takes in a set of information about the image and parameters
%for building the skeleton and returns a skeleton and matrix representation
%of the skeleton.
%
%image_mask is a logical image mask of the object
%
%image_edge is a logical image edge of the object
%
%dim is the dimensions of the object. Note this is the not the dimensions
%of the image but the pixel scaling. If dealing with a 2D image set the
%third dimension to zero!
%
%atom_num is the number of medial atoms to return. Options are a number and
%'Full'. For a given number that many medial atoms will be returned even if
%some atoms must be repeated. 
%
%Percentile is which percentile of the image to take as the backbone. I
%recommend 5% for most images. Go lower for spherical objects and higher
%for very convoluted objects.
%
%norm_num is the number of normals to return per atom, this will take the first set
%which are ranked according to eigenvalue (i.e. the first normal
%is the one with the largest eigenvalue-- one measure of vector importance)
%
%Returned: 
%A skeleton structure (Skeleton consiting of a set of atoms
%where each atom has a location,radius and set of normals). 
%
%A matrix with -1 at edges and positive values at atom locations.
%
%See Also:getskel_grad,inscribe_norms
%Rachel Sparks
%Last Updated 3/17/2010

%get the gradient based backbone
[backbone,map]=getskel_fuzzy(image_mask,percentile,dim);
size_map=size(map);

%initialize skeleton radius matrix
skeleton_rad=zeros(size_map);
slice_int=1;
slice_step=round(1:size(backbone,3)/z_step:size(backbone,3));
for slice_temp=slice_step;
%Find all backbone points in a given z slice
[i]=find(backbone(:,:,slice_temp));
map_temp=map(:,:,slice_temp);
%find all edge points
x_ind=find(image_edge);
[x_edge,y_edge,z_edge]=ind2sub(size(image_edge),x_ind);
   
%set a threshold
thresh=pi/3;

%set the atom step
if strcmp(atom_num,'full');
    atom_step=1;
else
    atom_step=round(1: length(i)/atom_num: length(i));
end

%iterate through each atom point selected
    j_temp=1;    
for k_temp=atom_step;
        k=k_temp;
        %find the radial distance
        rad=map_temp(i(k));
        
        %set each point to have the radial distance associated with it
        skeleton_rad(i(k))=rad;
        
        %find the x,y,z coordinates of the point
        [x,temp]=ind2sub(size_map,i(k));
        y=mod(temp,size_map(2));
        z=slice_temp;

% % %%%%%%For Imaging Purposes Only
% %         scrsz = get(0,'ScreenSize');
%         imshow(image_edge(:,:,z),[],'InitialMagnification','fit')
% %         set(gcf,'Position',[101 scrsz(4)/2+100 scrsz(3)/2-100 scrsz(4)/2-100])
%         hold on;
% % % %        plot the current point in magenta
%         plot(y,x,'.m');
% % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % %%
%         % Find the Inscribing Sphere
%         sphere=inscribe([x,y,z],rad,size_map,[dim(1),dim(2),dim(3)]);
% %         %Plot the Inscribing Sphere
%         [sphere_x,sphere_y]=find(sphere(:,:,z));
%          plot(sphere_y,sphere_x,'.g');
% % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%         
%         %find the intersections of sphere and edge of prostate
%         intersections=sphere.*image_edge;
%         [int_x,int_temp]=find(intersections(:,:,:));
%         int_y=mod(int_temp,size_map(2));
%         int_z=ceil(int_temp/size_map(2));
% % %        Plot the intersecting vectors
% %         [plot_x,plot_y]=find(intersections(:,:,z));
% %         plot(plot_y,plot_x,'.r');
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%calculate the distance from the medial atom to the surface points
            dist_to_atom=sqrt((x_edge-x).^2+(y_edge-y).^2+(z_edge-z).^2);
            vectors=[x_edge(dist_to_atom<(rad+1)) y_edge(dist_to_atom<(rad+1)) z_edge(dist_to_atom<(rad+1))];
            
%%%%%%%%%%%%%%%%For Imaging Purposes Only
% plot(vectors(:,2),vectors(:,1),'.b');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            if ~(size(vectors,2)<2||isempty(vectors))
             norms=inscribe_norms(vectors,[x,y,z],thresh);
%%%%%%%%%%%%%%%%For Imaging Pruposes Only
% display_inscribe_norms;
% 
% plot([5*norms(2,:)+y],[5*norms(1,:)+x],'.r')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                skeleton.atom{j_temp,slice_int}.location=[x,y,z];
                skeleton.atom{j_temp,slice_int}.radius=rad;
                skeleton.atom{j_temp,slice_int}.norms=norms(:,1:min(norm_num,size(norms,2)));
                j_temp=j_temp+1;
            else
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                skeleton.atom{j_temp,slice_int}.location=[x,y,z];
                skeleton.atom{j_temp,slice_int}.radius=rad;
                skeleton.atom{j_temp,slice_int}.norms=[vectors(:,1),vectors(:,1)];
                j_temp=j_temp+1;
            end
end
                slice_int=slice_int+1;
end
skeleton_rad(image_mask)=100*skeleton_rad(image_mask)/max(max(max(skeleton_rad)));
skeleton_rad(logical(image_edge))=-1;

end