function [gradfeats,feat_names] = gradfilts3(vol)
% GRADFILTS3 Gradient and gradient-like kernel filter operations.
% function [gradfeats,feat_names] = gradfilts3(vol)
%
%JC

[height, width, planes] = size(vol);
nfeats=13;
gradfeats=zeros(height, width, planes, nfeats, class(vol));

fprintf('Calculating x,y,z Sobel Edge Images.\n');
gradfeats(:,:,:,1) = sobelx3(vol);
gradfeats(:,:,:,2) = sobely3(vol);
gradfeats(:,:,:,3) = sobelz3(vol);

fprintf('Calculating diagonal Sobel Edge Images.\n');
gradfeats(:,:,:,4) = sobelxydiag3(vol);
gradfeats(:,:,:,5) = sobelyxdiag3(vol);
gradfeats(:,:,:,6) = sobelxzdiag3(vol);
gradfeats(:,:,:,7) = sobelzxdiag3(vol);
gradfeats(:,:,:,8) = sobelyzdiag3(vol);
gradfeats(:,:,:,9) = sobelzydiag3(vol);

% Features 14 to 17 do not use a window size
fprintf('Calculating Directional and Magnitude Gradients.\n');
[gradfeats(:,:,:,10),gradfeats(:,:,:,11), gradfeats(:,:,:,12)] = gradient(vol);

gradfeats(:,:,:,13)=gradfeats(:,:,:,10).^2;
gradfeats(:,:,:,13)=gradfeats(:,:,:,13)+gradfeats(:,:,:,11).^2;
gradfeats(:,:,:,13)=gradfeats(:,:,:,13)+gradfeats(:,:,:,12).^2;
gradfeats(:,:,:,13)=sqrt(gradfeats(:,:,:,13));
% gradfeats(:,:,:,13)=sqrt(gradfeats(:,:,:,10).^2 + gradfeats(:,:,:,11).^2+gradfeats(:,:,:,12).^2);

feat_names = {'sobelx','sobely','sobelz','sobelxy','sobelyx','sobelxz','sobelzx','sobelyz','sobelzy','gradx','grady','gradz','grad_mag'};

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

