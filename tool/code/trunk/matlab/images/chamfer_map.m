function [d_map]=chamfer_map(image)
%chamfer_map calculates the chamfer_map for a given image mask.
%
%chamfer_map(image) takes in an image mask and returns a distance map.
%
%Image is an image mask
%
%See also chamfer_mat1, chamfer_mat2, naive_map, im2vol.
%Rachel Sparks

img_size=size(image);

%first calculate the initialization map

%find the city block distance map
d_map=mask2distances(image,'type','cityblock');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Chamfer Map Initialization
%set all non 1,-1 enteries to -99 or 99
d_map(d_map<-1)=-99;
d_map(d_map>1)=99;

%set 1 and -1 enteries equal to 5
d_map(d_map==1)=5;
d_map(d_map==-1)=-5;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%pad the image
pad=2;
new_image=padarray(d_map,[pad pad 0],'replicate');

%for every image slice
    for k=1:img_size(3)              
        %get the pixels to apply images to:
        [y,x]=find((d_map(:,:,k)>10)|(d_map(:,:,k)<-10));
        %shift the indices by 2 (same as the padded image
        y=y+pad;
        x=x+pad;
        %for top to bottom, left to right
        for j=1:length(y)
                new_image(y(j),x(j),k)=chamfer_mat1(new_image(y(j)-1:y(j)+1,x(j)-1:x(j)+1,k));
        end
 
        [y,x]=find((new_image(3:img_size(1)+2,3:img_size(2)+2,k)>10)|(new_image(3:img_size(1)+2,3:img_size(2)+2,k)<-10));

        y=y+pad;
        x=x+pad;
        %for bottom to top, right to left
        for j=length(y):-1:1
                new_image(y(j),x(j),k)=chamfer_mat2(new_image(y(j)-1:y(j)+1,x(j)-1:x(j)+1,k));
        end
           %set the distance map to be returned.
           d_map(:,:,k)=new_image(3:img_size(1)+2,3:img_size(2)+2,k); 
    end

end