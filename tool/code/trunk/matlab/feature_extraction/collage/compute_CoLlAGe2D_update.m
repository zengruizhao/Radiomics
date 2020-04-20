function [feature_concat, collage_description, param,display]=compute_CoLlAGe2D_update(origImage, tumorMask, MPP, showfigures)
% Original code by Prateek Prassana 7.1.2015
% modifications by George Lee 9.16.2015
% - Accounts for multi-resolution histology images
% - ability to disable visualization
% - save parameters used
% - 10x faster implementation of orientation calculation
% - 4x faster implementation of roi concatentation of values
% - automatically saves features for all haralick gradients


if exist('showfigures','var')
    showfigures = 'yes';
end

if ~exist('MPP','var')
    MPP = 0.5; % default assumes 20x magnification
end

factor = 0.5/MPP;

gws = 5;
hws = 5;
hdist = 1;
N = 64;

k = round(gws * factor);
if k < 1, k = 1; end
hws = round(hws*factor);
if hws < 1, hws = 1; end
hdist = round(hdist*factor);
if hdist < 1, hdist = 1; end

img = origImage;
if ndims(img) < 3
    gimg = img;
elseif ndims(img) == 3
    gimg = rgb2gray(img); % assume img is rgb
else
    fprintf('Unidentified image format')
end

if ~exist('tumorMask','var')
    mask(:,:,1) = (gimg ~= max(max(gimg)));
else
    img_mask = tumorMask;
    if gws > hws, maxdilate = sqrt(gws^2+gws^2); else maxdilate = sqrt(hws^2+hws^2); end
    buffer = ceil( (maxdilate+1) * factor);
    mask = imdilate(img_mask, strel('disk', buffer )); %doesn't affect feature
    mask = logical(mask);
end

origImage = gimg;

%% -------------- start Prateek code ----------------------------------
%close all
Med_entropy=[];
Mean_entropy=[];
index_array=[];
val_hist=[];
feature_concat=[];

%k=2;
Ws=k;
fprintf('\nGradient Window Size: %i,',Ws)
fprintf(' Haralick Window Size: %i,',hws)
fprintf(' Haralick Distance: %i,',hdist)
fprintf(' Haralick Quantitization: %i',N)


%change as necessary
%I=origImage;
I=origImage.*uint16(mask); % buffered nuclei image masked region
tumorMask;


%imshow(mask)
[r_mask,c_mask]=find(tumorMask==1);
c_min=min(c_mask);c_max=max(c_mask);r_min=min(r_mask);r_max=max(r_mask);

x1=r_min+buffer;
x2=r_max-buffer;
y1=c_min+buffer;
y2=c_max-buffer;

% h = imfreehand; % now pick ROI
% BW = createMask(h); % get BW mask for that ROI
% pos = getPosition(h);
% x1 =  round(min(pos(:,2)));
% y1 =  round(min(pos(:,1)));
% x2 =  round(max(pos(:,2)));
% y2 =  round(max(pos(:,1)));
%I2 = I.*uint8(BW); % apply mask to image


%fprintf('\nCalculating gradients...')
%tStart = tic;
I2_outer = I(x1-Ws:x2+Ws,y1-Ws:y2+Ws);
I2_double_outer=im2double(I2_outer);
I2_inner=I(x1:x2,y1:y2);
[r c]=size(I2_inner);
[Lx, Ly]=gradient(I2_double_outer);
Fx = Lx;
Fy = Ly;
%[Fx, Fy]=gradient(I2_double_outer);
K_roi=sqrt(Fx.*Fx+Fy.*Fy);
%tElapsed = toc(tStart);
%fprintf('%0.2fs',tElapsed);

fprintf('\nCalculating dominant orientation...')
tStart = tic;
%[I_gradient_inner, I_gradient_inner_mag, dominant_orientation_roi]=find_orientation_CoLlAGe(Lx,Ly,k,r,c);
%[~, ~, dominant_orientation_roi]=find_orientation_CoLlAGe(Lx,Ly,k,r,c);
[dominant_orientation_roi]=find_orientation_CoLlAGe_optimized(Lx,Ly,k,r,c);

BW1=mask(x1:x2,y1:y2);
BWorig=double(tumorMask(x1:x2,y1:y2));

% if exist('showfigures','var')
%     
%     figure(1)% plot arrows on intensities
%     
%     imagesc(I2_inner); colormap(gray)
%     hold on
%     for i =1:2:r
%         for j= 1:2:c
%             %arrowlength=4*K_roi_outer(i,j)/max(max(K_roi_outer));
%             if BW1(i,j)==1
%                 arrowlength=2;
%                 h= quiver(j, i, cos(dominant_orientation_roi(i,j)), sin(-dominant_orientation_roi(i,j)),arrowlength,'b','linewidth',3);
%                 %            pause
%                 %adjust_quiver_arrowhead_size(h,5);
%             end
%             hold on
%         end
%         
%     end
%     hold off
% end
tElapsed = toc(tStart);
fprintf('%0.2fs',tElapsed);

fprintf('\nCalculating Haralick...')
tStart = tic;
% Find co-occurrence features of orientation
clear volfeats;
haralickfun=@haralick2mex;
vol=double(dominant_orientation_roi);
nharalicks=13;  % Number of Features
bg=-1;   % Background
ws=hws;    % Window Size
hardist=hdist;   % Distance in a window
harN=N;     % Maximum number of quantization level
volN=round(rescale_range(vol,0,harN-1));   % Quantizing an image
addedfeats=0;  % Feature counter index
volfeats(:,:,addedfeats+(1:nharalicks))=haralickfun(volN,harN,ws,hardist,bg);

if exist('showfigures','var')
    %Plot arrows on orientation-CM features
    figure(2)
    imagesc(volfeats(:,:,1).*BWorig)
    % entropy_reshape=reshape(volfeats(:,:,9),[r*c 1]);
    % Med_entropy=[Med_entropy ; median(entropy_reshape)];
    % Mean_entropy=[Mean_entropy;mean(entropy_reshape)];
    hold on
    for i =1:2:r
        for j= 1:2:c
            if BWorig(i,j)==1%BW1(i,j)==1
                %arrowlength=4*K_roi_outer(i,j)/max(max(K_roi_outer));
                
                if sum(sum(BWorig)) < 2000 % threshold for plotting quiver arrows
                arrowlength=1.5;
                h= quiver(j, i, cos(dominant_orientation_roi(i,j)), sin(-dominant_orientation_roi(i,j)),arrowlength,'b','linewidth',3);
                end
                
                %            pause
                %adjust_quiver_arrowhead_size(h, 5);
                hold on
            end
        end
        
    end
    
    colormap(hot)
    axis off
    %pause
    
end

tElapsed = toc(tStart);
fprintf('%0.2fs',tElapsed);

for i = 1:numel(size(volfeats,3))
    OCM_feature=volfeats(:,:,i).*BWorig;
    %tic
    vecOCM = OCM_feature(:);
    %vecBWorig = BWorig(:);
    feature_concat = vecOCM(BWorig(:)==1)';
    %toc
end

haralick_description = [
    {'entropy','energy','inertia','idm','correlation','info1','info2','sum_av','sum_var','sum_ent','diff_av','diff_var','diff_ent'}
    ];

n = 0;
for i = 1:numel(size(volfeats,3))
    OCM_feature=volfeats(:,:,i).*BWorig;
    %tic
    vecOCM = OCM_feature(:);
    %vecBWorig = BWorig(:);
    BWorig_temp = logical(BWorig);
    feature_concat = vecOCM(BWorig_temp(:)==1)';
   
    display.texture_img{i} = volfeats(:,:,i).*BWorig;
    
    collagefeats(n+1) = max(feature_concat);
    collagefeats(n+2) = mean(feature_concat);
    collagefeats(n+3) = median(feature_concat);     
    collagefeats(n+4) = min(feature_concat);
    collagefeats(n+5) = std(feature_concat);
    
    collage_description(n+1) = {['Collage:' 'max gradient ' haralick_description{i}]};
    collage_description(n+2) = {['Collage:' 'mean gradient ' haralick_description{i}]};
    collage_description(n+3) = {['Collage:' 'median gradient ' haralick_description{i}]};
    collage_description(n+4) = {['Collage:' 'min gradient ' haralick_description{i}]};
    collage_description(n+5) = {['Collage:' 'standard deviation gradient ' haralick_description{i}]};
    
    n = n + 5;
end

%collage_description
%eval(['{' '''' strjoin(collage_description,[''',\n''']) '''' '}' ])


% tic
% for i=1:r
%     for j=1:c
%          if BWorig(i,j)==1
%              feature_concat=[feature_concat OCM_feature(i,j)];
%          end
%     end
% end
% toc

param.gws = Ws;
param.hws = ws;
param.hdist = hardist;
param.hgrays = harN;

display.fimg = volfeats;
display.mask = BWorig;
display.r = r;
display.c = c;
