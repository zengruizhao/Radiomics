function [gradfeats feat_names] = gradfilts2(img)
% function gradfeats = gradfilts2(img)
% Calculate gradient features and gradient-like kernel filter operations (Sobel/Kirsch etc).
% IMG can only be a 2D matrix
% Satish Viswanath, Jul 2008
% Updated to use GRADFILTS3 as template, Jul 2013
%
% see also: gradfilts3.m

[height, width, planes] = size(img);
if planes > 1, error('only 2D image supported, else see GRADFILTS3'); end;

nfeatures=13;
gradfeats=ones(height, width, nfeatures,class(img));

fprintf('Calculating x,y Sobel Edge Images.\n');
gradfeats(:,:,1) = sobelx(img);
gradfeats(:,:,2) = sobely(img);

fprintf('Calculating diagonal Sobel Edge Images.\n');
gradfeats(:,:,3) = sobelxydiag(img);
gradfeats(:,:,4) = sobelyxdiag(img);

gradfeats(:,:,5) = Get_Kirsch_1(img);
gradfeats(:,:,6) = Get_Kirsch_2(img);
gradfeats(:,:,7) = Get_Kirsch_3(img);

fprintf('Calculating Directional and Magnitude Gradients.\n');
[gradfeats(:,:,8),gradfeats(:,:,9)] = gradient(img);
gradfeats(:,:,10)=sqrt(gradfeats(:,:,8).^2 + ... 
    gradfeats(:,:,9).^2);
gradfeats(:,:,11)=dx(img);
gradfeats(:,:,12)=dy(img);
gradfeats(:,:,13)=ddiag(img); 

feat_names = {'sobelx','sobely','sobelz','sobelxy','sobelyx','sobelxz','sobelzx','sobelyz','sobelzy','gradx','grady','gradz','grad_mag'};

%% ******************************************************

function Y = sobelx(img)

sobel = [-1 0 1;-2 0 2;-1 0 1];
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = sobely(img)

sobel = [1 2 1; 0 0 0; -1 -2 -1];
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = sobelxydiag(img)

sobel = [0 1 2; -1 0 1; -2 -1 0];
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = sobelyxdiag(img)

sobel = [0 1 2; -1 0 1; -2 -1 0];
sobel=flipdim(sobel,2);
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = Get_Kirsch_1(img)

sobel = [3 3 3;3 0 3; -5 -5 -5];
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = Get_Kirsch_2(img)

sobel = [-5 3 3;-5 0 3; -5 3 3];
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = Get_Kirsch_3(img)

sobel = [3 3 3;-5 0 3; -5 -5 3];
Y = conv2(img,sobel,'same');

%% -------------------------------------------------------------------------

function Y = dx(img)

mask = [-1 1];
Y = conv2(img,mask,'same');

%% -------------------------------------------------------------------------

function Y = dy(img)

mask = [-1; 1];
Y = conv2(img,mask,'same');

%% -------------------------------------------------------------------------

function Y = ddiag(img)

mask = [-1 0;0 1];
Y = conv2(img,mask,'same');