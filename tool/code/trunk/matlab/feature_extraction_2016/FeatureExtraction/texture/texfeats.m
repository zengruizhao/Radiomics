function texture = texfeats(I)
% Calculate texture features

% % Covert to greyscale, since most of these features are for 2-d images
% % only. You can cycle through each slice individually if you'd like.
% Img = rgb2gray(I);

texture = [];
for slice = 1:3
    Img = I(:,:,slice);
    
    % Grayscale Texture Features
    %   [texture_grayscale] = GreyLevel2(Img, WindowSize)
    texture_grayscale = GreyLevel2(Img, 3);
    texture_grayscale = reshape(texture_grayscale,...
        size(texture_grayscale,1), size(texture_grayscale,2),...
        size(texture_grayscale,3)*size(texture_grayscale,4));
    
    % Gabor Texture Features
    %   [texture_gabors_cos,texture_gabors_sin] = gaborfilt(Img,theta,lambda,b,psi,gamma)
    %   Suggested Parameters:
    %       lambda = [1 2];
    %       theta = (0:5)*pi/6;
    %       b = 1; psi = 0; gamma = 1;
    [texture_gabors_cos, texture_gabors_sin] = gaborfilt(Img, (0:5)*pi/6, [1 2], 1, 0, 1);
    texture_gabors_cos = reshape(texture_gabors_cos, ...
        size(texture_gabors_cos,1), size(texture_gabors_cos,2), ...
        size(texture_gabors_cos,3)*size(texture_gabors_cos,4));
    
    texture_gabors_sin = reshape(texture_gabors_sin, ...
        size(texture_gabors_sin,1), size(texture_gabors_sin,2), ...
        size(texture_gabors_sin,3)*size(texture_gabors_sin,4));
    
    % Fix the zero dimensions
    cos_idx = squeeze(any(any(texture_gabors_cos(:,:,1:144))));
    temp = texture_gabors_cos(:,:,cos_idx);
    texture_gabors_cos = temp; clear temp cos_idx
    
    
    sin_idx = squeeze(any(any(texture_gabors_sin(:,:,1:144))));
    temp = texture_gabors_sin(:,:,sin_idx);
    texture_gabors_sin = temp; clear temp sin_idx
    
%     % Haralick Texture Features:
%     %   [texture_haralick, haralick_names] = haralick(Img,GRAYLEVELS,SIG,DIST,IS_VALID,feature_names,COMTX_TYPE)
%     %   Suggested Parameters (see help in haralick.m for more info):
%     %       GRAYLEVELS = 256;
%     %       SIG = 1;
%     %       DIST = 1;
%     %       IS_VALID = Img < 225;
%     %       feature_names = [];
%     %       COMTX_TYPE = 'ring';
%     [texture_haralick, ~] = haralick(Img,256,1,1,Img < 256,[],'ring');                                                                                                            
    
    % Laws Texture Features:
    [texture_laws] = lawsfilter(Img);
    
    % Local Binary Pattern Features:
    %   [texture_lbp] = lbp(Img,startoffset,ccw);
    %   See help in lbp.m for info on "startoffset" and "ccw"
    texture_lbp = [];
    for jj = 0:7
        [texture_lbp] = cat(3, texture_lbp, lbp(Img,jj));
    end
    for jj = 0:7
        [texture_lbp] = cat(3, texture_lbp, lbp(Img,jj,false));
    end
    
% %     texture = cat(3,texture_features,texture_grayscale,texture_haralick,...
% %         texture_gabors_cos, texture_gabors_sin, texture_laws, texture_lbp);
%     texture = cat(3,texture,texture_grayscale,texture_haralick,...
%             texture_gabors_cos, texture_gabors_sin);
    texture = cat(3,texture,texture_grayscale,texture_gabors_cos, texture_gabors_sin,texture_laws,texture_lbp);
%     texture = cat(3,texture,texture_grayscale,texture_laws,texture_lbp);%texture_gabors_cos, texture_gabors_sin
end
