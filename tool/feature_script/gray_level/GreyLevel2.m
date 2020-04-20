function GreyLevelFeature = GreyLevel2(inVol,WindowSize)
% Calculate gray-level features using 2D window (can accept volume input)
% Edit 9/26/2011 -- Modified medianfilt2, stdfilt2, rangefilt2 to use 
%   built-in functions (SD)

global Vol
Vol=double(inVol);
clear inVol;

verbose_mode = 0;

[height, width, planes] = size(Vol);

nfeatures=15;
GrayLevelFeature=ones(height, width, planes, nfeatures);

%% ******************************************************
%% ** Grey-Level Feature Extraction *********************
%% ******************************************************
if(verbose_mode), fprintf('Calculating Mean Image... '); end;
GreyLevelFeature(:,:,:,1) = meanfilt2(WindowSize);

if(verbose_mode), fprintf('Median... ');end;
GreyLevelFeature(:,:,:,2) = medianfilt2(WindowSize);

if(verbose_mode), fprintf('Std. Dev.... ');end;
GreyLevelFeature(:,:,:,3) = stdfilt2(WindowSize);

if(verbose_mode), fprintf('Range... ');end;
GreyLevelFeature(:,:,:,4) = rangefilt2(WindowSize);

if(verbose_mode), fprintf('x,y Sobel...\n');end;
GreyLevelFeature(:,:,:,5) = sobelx(WindowSize);
GreyLevelFeature(:,:,:,6) = sobely(WindowSize);

if(verbose_mode), fprintf('\tdiagonal Sobel... ');end;
GreyLevelFeature(:,:,:,7) = sobelxydiag(WindowSize);
GreyLevelFeature(:,:,:,8) = sobelyxdiag(WindowSize);

GreyLevelFeature(:,:,:,9) = Get_Kirsch_1(WindowSize);
GreyLevelFeature(:,:,:,10) = Get_Kirsch_2(WindowSize);
GreyLevelFeature(:,:,:,11) = Get_Kirsch_3(WindowSize);

if(verbose_mode), fprintf('Derivative Gradients.\n'); end;
[GreyLevelFeature(:,:,:,12),GreyLevelFeature(:,:,:,13)] = gradient(Vol);
GreyLevelFeature(:,:,:,14)=sqrt(GreyLevelFeature(:,:,:,12).^2 + ... 
    GreyLevelFeature(:,:,:,13).^2);
GreyLevelFeature(:,:,:,15)=ddiag(WindowSize);

clear global Vol

%% ******************************************************
%% ** Sub-Functions that actually do the work ***********
%% ******************************************************
function Y = meanfilt2(WindowSize)

global Vol
[Height,Width,planes] = size(Vol);
mask = repmat(1/WindowSize.^2,WindowSize,WindowSize);
Y = convn(Vol,mask,'same');

[Height,Width,planes] = size(Y);
% Vector = Y(:);

%%-------------------------------------------------------------------------

function Y = medianfilt2(WindowSize)

global Vol

% Use medfilt2 with the symmetric padding technique
Y = medfilt2(Vol, [WindowSize WindowSize], 'symmetric');

% % Half window dimensions
% % hw = floor(WindowSize/2);
% hw1=floor(WindowSize/2);
% hw2=ceil(WindowSize/2);
% % Volume dimensions
% [Height,Width,planes] = size(Vol);
% % Output same size as input
% Y = zeros([Height,Width,planes]);
% 
% % for every voxel, calculate median in window
% for k=1:planes,
%     zinds=k;    % flat window
%     zinds(zinds<1 | zinds>planes)=[];
%     for i = 1:Height
%         % yinds=i-hw:i+hw-1;
%         yinds=i-hw1:i+hw2-1;
%         yinds(yinds<1 | yinds>Height)=[];
%         for j = 1:Width
%             % xinds=j-hw:j+hw-1;
%             xinds=j-hw1:j+hw2-1;
%             xinds(xinds<1 | xinds>Width)=[];
% 
%             wind=Vol(yinds,xinds,zinds);
%             Y(i,j,k)=median(wind(:));
%         end
%     end
% %     fprintf('.')
% end
% % fprintf('\n');

function medval=medcolfun(x)
% Column-wise function to perform median filtering.
%
% For use with colfilt and a M-by-N neighborhood.

medval=median(x);


%%-------------------------------------------------------------------------

function Y = stdfilt2(WindowSize)

global Vol

Y = stdfilt(Vol, ones(WindowSize));

% [Height,Width,planes] = size(Vol);
% Y = zeros(Height, Width,planes);
% 
% % fprintf('    preparing mean image again...\n');
% mask = repmat(1/WindowSize.^2,WindowSize,WindowSize);
% meanimage = convn(Vol,mask,'same');
% 
% % hw = floor(WindowSize/2);
% hw1=floor(WindowSize/2);
% hw2=ceil(WindowSize/2);
% 
% % fprintf('    calculating std image');
% % for every voxel, calculate STD in window
% for k=1:planes,
%     zinds=k;    % flat window
%     zinds(zinds<1 | zinds>planes)=[];
%     for i = 1:Height
%         % yinds=i-hw:i+hw-1;
%         yinds=i-hw1:i+hw2-1;
%         yinds(yinds<1 | yinds>Height)=[];
%         for j = 1:Width
%             % xinds=j-hw:j+hw-1;
%             xinds=j-hw1:j+hw2-1;
%             xinds(xinds<1 | xinds>Width)=[];
% 
%             wind=Vol(yinds,xinds,zinds);
%             Y(i,j,k) = sum((wind(:)-meanimage(i,j,k)).^2)/prod(size(wind));
%         end
%     end
% %     fprintf('.')
% end
% % fprintf('\n');

%%-------------------------------------------------------------------------

function Y = rangefilt2(WindowSize)

global Vol

Y = rangefilt(Vol, ones(WindowSize));

% [Height, Width, planes] = size(Vol);
% Y = zeros(Height, Width, planes);
% 
% % hw = floor(WindowSize/2);
% hw1=floor(WindowSize/2);
% hw2=ceil(WindowSize/2);
% 
% % for every voxel, calculate STD in window
% for k=1:planes,
%     zinds=k;    % flat window
%     zinds(zinds<1 | zinds>planes)=[];
%     for i = 1:Height
%         % yinds=i-hw:i+hw-1;
%         yinds=i-hw1:i+hw2-1;
%         yinds(yinds<1 | yinds>Height)=[];
%         for j = 1:Width
%             % xinds=j-hw:j+hw-1;
%             xinds=j-hw1:j+hw2-1;
%             xinds(xinds<1 | xinds>Width)=[];
% 
%             wind=Vol(yinds,xinds,zinds);
%             Y(i,j,k) = max(wind(:))-min(wind(:));
%         end
%     end
% %     fprintf('.')
% end
% % fprintf('\n');

%%-------------------------------------------------------------------------

function Y = Get_Kirsch_1(WindowSize)

global Vol
sobel = [3 3 3;3 0 3; -5 -5 -5];
Y = convn(Vol,sobel,'same');


%%-------------------------------------------------------------------------

function Y = Get_Kirsch_2(WindowSize)

global Vol
sobel = [-5 3 3;-5 0 3; -5 3 3];
Y = convn(Vol,sobel,'same');


%%-------------------------------------------------------------------------

function Y = Get_Kirsch_3(WindowSize)

global Vol
sobel = [3 3 3;-5 0 3; -5 -5 3];
Y = convn(Vol,sobel,'same');


%%-------------------------------------------------------------------------

function Y = sobelxydiag(Image,WindowSize)

global Vol
sobel = [0 1 2; -1 0 1; -2 -1 0];
Y = convn(Vol,sobel,'same');

%%-------------------------------------------------------------------------

function Y = sobelyxdiag(Image,WindowSize)

global Vol
sobel = [0 1 2; -1 0 1; -2 -1 0];
sobel = flip(sobel,2);
Y = convn(Vol,sobel,'same');

%%-------------------------------------------------------------------------

function Y = sobely(WindowSize)

global Vol
sobel = [1 2 1; 0 0 0; -1 -2 -1];
Y = convn(Vol,sobel,'same');

%%-------------------------------------------------------------------------

function Y = sobelx(WindowSize)

global Vol
sobel = [-1 0 1;-2 0 2;-1 0 1];
Y = convn(Vol,sobel,'same');

%%-------------------------------------------------------------------------

function Y = dx(WindowSize)

global Vol
mask = [-1 1];
Y = convn(Vol,mask,'same');

%%-------------------------------------------------------------------------

function Y = dy(WindowSize)

global Vol
mask = [-1; 1];
Y = convn(Vol,mask,'same');

%%-------------------------------------------------------------------------

function Y = ddiag(Image,WindowSize)

global Vol
mask = [-1 0;0 1];
Y = convn(Vol,mask,'same');
