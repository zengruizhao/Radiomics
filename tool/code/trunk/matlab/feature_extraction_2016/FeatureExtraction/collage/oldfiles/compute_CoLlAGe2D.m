%Prateek 7/1/2015
function [feature_concat]=compute_CoLlAGe2D(origImage, tumorMask, k)
%close all
Med_entropy=[];
Mean_entropy=[];
index_array=[];
val_hist=[];
feature_concat=[];

%k=2;
Ws=k;
%origImage=case19_2.standardized_volume_T1(:,:,4);
I = flipdim(origImage ,2);
%tumorMask=case19_2.mask_T1(:,:,4);
mask = flipdim(tumorMask ,2);

%change as necessary
I=origImage;
mask=tumorMask;


%imshow(mask)
[r_mask,c_mask]=find(mask==1);
c_min=min(c_mask);c_max=max(c_mask);r_min=min(r_mask);r_max=max(r_mask);

x1=r_min;
x2=r_max;
y1=c_min;
y2=c_max;

% h = imfreehand; % now pick ROI
% BW = createMask(h); % get BW mask for that ROI
% pos = getPosition(h);
% x1 =  round(min(pos(:,2)));
% y1 =  round(min(pos(:,1)));
% x2 =  round(max(pos(:,2)));
% y2 =  round(max(pos(:,1)));
%I2 = I.*uint8(BW); % apply mask to image


I2_outer = I(x1-Ws:x2+Ws,y1-Ws:y2+Ws);
I2_double_outer=im2double(I2_outer);
I2_inner=I(x1:x2,y1:y2);
[r c]=size(I2_inner);
[Lx, Ly]=gradient(I2_double_outer);

[Fx, Fy]=gradient(I2_double_outer);
K_roi=sqrt(Fx.*Fx+Fy.*Fy);

[I_gradient_inner, I_gradient_inner_mag, dominant_orientation_roi]=find_orientation_CoLlAGe(Lx,Ly,k,r,c);
BW1=mask(x1:x2,y1:y2);

figure(1)% plot arrows on intensities

imagesc(I2_inner); colormap(gray)
hold on
for i =1:2:r
    for j= 1:2:c
           %arrowlength=4*K_roi_outer(i,j)/max(max(K_roi_outer));
           if BW1(i,j)==1
           arrowlength=2;
           h= quiver(j, i, cos(dominant_orientation_roi(i,j)), sin(-dominant_orientation_roi(i,j)),arrowlength,'b','linewidth',3);
%            pause
           %adjust_quiver_arrowhead_size(h,5);
           end
           hold on
    end

end
hold off


% Find co-occurrence features of orientation
clear volfeats;
haralickfun=@haralick2mex;
vol=double(dominant_orientation_roi);
nharalicks=13;  % Number of Features
bg=-1;   % Background
ws=5;    % Window Size
hardist=1;   % Distance in a window
harN=64;     % Maximum number of quantization level
volN=round(rescale_range(vol,0,harN-1));   % Quantizing an image
addedfeats=0;  % Feature counter index
volfeats(:,:,addedfeats+(1:nharalicks))=haralickfun(volN,harN,ws,hardist,bg);

%Plot arrows on orientation-CM features
figure(2)
imagesc(volfeats(:,:,1).*BW1)
% entropy_reshape=reshape(volfeats(:,:,9),[r*c 1]);
% Med_entropy=[Med_entropy ; median(entropy_reshape)];
% Mean_entropy=[Mean_entropy;mean(entropy_reshape)];
hold on
for i =1:2:r
    for j= 1:2:c
           if BW1(i,j)==1
           %arrowlength=4*K_roi_outer(i,j)/max(max(K_roi_outer));
           arrowlength=1.5;
           h= quiver(j, i, cos(dominant_orientation_roi(i,j)), sin(-dominant_orientation_roi(i,j)),arrowlength,'b','linewidth',3);
%            pause
           %adjust_quiver_arrowhead_size(h, 5);
           hold on
           end
    end

end


colormap(hot)
axis off
%pause

OCM_feature=volfeats(:,:,1);
for i=1:r
    for j=1:c
         if BW1(i,j)==1
             feature_concat=[feature_concat OCM_feature(i,j)];
         end
    end
end

close all








