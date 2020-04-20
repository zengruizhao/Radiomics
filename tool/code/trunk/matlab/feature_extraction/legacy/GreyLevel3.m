function grayfeats = GreyLevel3(vol,WindowSize)
% Calculate gray-level features using 3D window

[height, width, planes] = size(vol);

if numel(WindowSize)==1, WindowSize=WindowSize([1 1 1]); end
if numel(WindowSize)==2, error('WindowsSize must be 1 or 3 elements.'); end

nfeatures=17;
grayfeats=zeros(height, width, planes, nfeatures);

% Extract features
fprintf('Calculating Mean Image.\n');
grayfeats(:,:,:,1) = meanfilt3(vol,WindowSize);

fprintf('Calculating Median Image.\n');
grayfeats(:,:,:,2) = medianfilt3(vol,WindowSize);

fprintf('Calculating Standard Deviation Image.\n');
grayfeats(:,:,:,3) = stdfilt3(vol,WindowSize,grayfeats(:,:,:,2));

fprintf('Calculating Range Image.\n');
grayfeats(:,:,:,4) = rangefilt3(vol,WindowSize);

fprintf('Calculating x,y,z Sobel Edge Images.\n');
grayfeats(:,:,:,5) = sobelx3(vol);
grayfeats(:,:,:,6) = sobely3(vol);
grayfeats(:,:,:,7) = sobelz3(vol);

fprintf('Calculating diagonal Sobel Edge Images.\n');
grayfeats(:,:,:,8) = sobelxydiag3(vol);
grayfeats(:,:,:,9) = sobelyxdiag3(vol);
grayfeats(:,:,:,10) = sobelxzdiag3(vol);
grayfeats(:,:,:,11) = sobelzxdiag3(vol);
grayfeats(:,:,:,12) = sobelyzdiag3(vol);
grayfeats(:,:,:,13) = sobelzydiag3(vol);

% Features 14 to 17 do not use a window size
fprintf('Calculating Directional and Magnitude Gradients.\n');
[grayfeats(:,:,:,14),grayfeats(:,:,:,15), grayfeats(:,:,:,16)] = gradient(vol);

grayfeats(:,:,:,17)=grayfeats(:,:,:,14).^2;
grayfeats(:,:,:,17)=grayfeats(:,:,:,17)+grayfeats(:,:,:,15).^2;
grayfeats(:,:,:,17)=grayfeats(:,:,:,17)+grayfeats(:,:,:,16).^2;
grayfeats(:,:,:,17)=sqrt(grayfeats(:,:,:,17));
% grayfeats(:,:,:,17)=sqrt(grayfeats(:,:,:,14).^2 +
% grayfeats(:,:,:,15).^2+grayfeats(:,:,:,16).^2);

end

% Filter subroutines

function Y = meanfilt3(vol,WindowSize)

mask = repmat(1/prod(WindowSize),WindowSize);
Y = convn(vol,mask,'same');

end

function Y = medianfilt3(vol,WindowSize)

% Half window dimensions
hw = floor(WindowSize./2);
% volume dimensions
[Height,Width,planes] = size(vol);
% Output same size as input
Y = zeros([Height,Width,planes]);

% for every voxel, calculate median in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k)=median(wind(:));
        end
    end
    fprintf('.')
end
fprintf('\n');

end

function Y = stdfilt3(vol,WindowSize,meanimage)

[Height,Width,planes] = size(vol);
Y = zeros(Height, Width,planes);

hw = floor(WindowSize./2);

% for every voxel, calculate STD in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k) = sum((wind(:)-meanimage(i,j,k)).^2)/numel(wind);
        end
    end
    fprintf('.')
end
fprintf('\n');

end

function Y = rangefilt3(vol,WindowSize)

[Height, Width, planes] = size(vol);
Y = zeros(Height, Width, planes);

hw = floor(WindowSize./2);

% for every voxel, calculate STD in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k) = max(wind(:))-min(wind(:));
        end
    end
    fprintf('.')
end
fprintf('\n');

end

%%-------------------------------------------------------------------------

function Y = sobelxydiag3(Vol)

% sobel = [0 1 2; -1 0 1; -2 -1 0];
sobeli1 = [0 1 3; -1 0 1; -3 -1 0];
sobeli2 = [0 3 6; -3 0 3; -6 -3 0];
sobeli3 = [0 1 3; -1 0 1; -3 -1 0];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
Y = convn(Vol,sobel,'same');

end

function Y = sobelyxdiag3(Vol)

% sobel = [0 1 2; -1 0 1; -2 -1 0];
sobeli1 = [0 1 3; -1 0 1; -3 -1 0];
sobeli2 = [0 3 6; -3 0 3; -6 -3 0];
sobeli3 = [0 1 3; -1 0 1; -3 -1 0];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
sobel=flipdim(sobel,2);
Y = convn(Vol,sobel,'same');

end

function Y = sobelyzdiag3(Vol)

% sobel = [0 1 2; -1 0 1; -2 -1 0];
sobeli1 = [0 1 3; -1 0 1; -3 -1 0];
sobeli2 = [0 3 6; -3 0 3; -6 -3 0];
sobeli3 = [0 1 3; -1 0 1; -3 -1 0];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
sobel=permute(sobel,[1 3 2]);
Y = convn(Vol,sobel,'same');

end

function Y = sobelzydiag3(Vol)

% sobel = [0 1 2; -1 0 1; -2 -1 0];
sobeli1 = [0 1 3; -1 0 1; -3 -1 0];
sobeli2 = [0 3 6; -3 0 3; -6 -3 0];
sobeli3 = [0 1 3; -1 0 1; -3 -1 0];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
sobel=permute(sobel,[1 3 2]);
sobel=flipdim(sobel,1);
Y = convn(Vol,sobel,'same');

end

function Y = sobelxzdiag3(Vol)

% sobel = [0 1 2; -1 0 1; -2 -1 0];
sobeli1 = [0 1 3; -1 0 1; -3 -1 0];
sobeli2 = [0 3 6; -3 0 3; -6 -3 0];
sobeli3 = [0 1 3; -1 0 1; -3 -1 0];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
sobel=permute(sobel,[3 2 1]);
Y = convn(Vol,sobel,'same');

end

function Y = sobelzxdiag3(Vol)

% sobel = [0 1 2; -1 0 1; -2 -1 0];
sobeli1 = [0 1 3; -1 0 1; -3 -1 0];
sobeli2 = [0 3 6; -3 0 3; -6 -3 0];
sobeli3 = [0 1 3; -1 0 1; -3 -1 0];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
sobel=permute(sobel,[3 2 1]);
sobel=flipdim(sobel,2);
Y = convn(Vol,sobel,'same');

end

function Y = sobely3(Vol)

sobeli1 = [1 3 1; 0 0 0; -1 -3 -1];
sobeli2 = [3 6 3; 0 0 0; -3 -6 -3];
sobeli3 = [1 3 1; 0 0 0; -1 -3 -1];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
Y = convn(Vol,sobel,'same');

end

function Y = sobelx3(Vol)

sobeli1 =  [1  3  1; ... 
            0  0  0; ... 
           -1 -3 -1];
sobeli2 = [3 6 3; 0 0 0; -3 -6 -3];
sobeli3 = [1 3 1; 0 0 0; -1 -3 -1];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
% rotate y gradient kernel to be x gradient kernel
sobel=permute(sobel,[2 1 3]);
Y = convn(Vol,sobel,'same');

end

function Y = sobelz3(Vol)

sobeli1 =  [1  3  1; ... 
            0  0  0; ... 
           -1 -3 -1];
sobeli2 = [3 6 3; 0 0 0; -3 -6 -3];
sobeli3 = [1 3 1; 0 0 0; -1 -3 -1];
sobel=cat(3,sobeli1,sobeli2,sobeli3);
% rotate y gradient kernel to be z gradient kernel
sobel=permute(sobel,[3 2 1]);
Y = convn(Vol,sobel,'same');

end

% %%-------------------------------------------------------------------------
% 
% function Y = dx3(WindowSize)
% 
% global Vol
% mask = [-1 1];
% Y = convn(Vol,mask,'same');
% 
% %%-------------------------------------------------------------------------
% 
% function Y = dy3(WindowSize)
% 
% global Vol
% mask = [-1; 1];
% Y = convn(Vol,mask,'same');
% 
% %%-------------------------------------------------------------------------
% 
% function Y = dy3(WindowSize)
% 
% global Vol
% mask = permute([-1; 1],[3 2 1]);
% Y = convn(Vol,mask,'same');

% %%-------------------------------------------------------------------------
% 
% function [Vector,Matrix] = Get_DerivativeInDiagonalWindows(Image,WindowSize)
% 
% [Height,Width] = size(Image);
% mask = [-1 0;0 1];
% 
% Y = filter2(mask,Image,'same');
% [Height,Width] = size(Y);
% 
% Vector = reshape(Y',Height*Width,1);
% Matrix = Y;

% %%-------------------------------------------------------------------------
% 
% function [Vector,Matrix] = Get_Kirsch_1(Image,WindowSize)
% 
% sobel = [3 3 3;3 0 3; -5 -5 -5];
% 
% Y = filter2(sobel,Image,'same');
% [Height,Width] = size(Y);
% Vector = reshape(Y',Height*Width,1);
% Matrix = Y;
% 
% %%-------------------------------------------------------------------------
% 
% function [Vector,Matrix] = Get_Kirsch_2(Image,WindowSize)
% 
% sobel = [-5 3 3;-5 0 3; -5 3 3];
% 
% Y = filter2(sobel,Image,'same');
% [Height,Width] = size(Y);
% Vector = reshape(Y',Height*Width,1);
% Matrix = Y;
% 
% %%-------------------------------------------------------------------------
% 
% function [Vector,Matrix] = Get_Kirsch_3(Image,WindowSize)
% 
% sobel = [3 3 3;-5 0 3; -5 -5 3];
% 
% Y = filter2(sobel,Image,'same');
% [Height,Width] = size(Y);
% Vector = reshape(Y',Height*Width,1);
% Matrix = Y;
