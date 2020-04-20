I = imread('E:\git\Zzr_feature\data\8 Body 3.0 C.png');
if ndims(I) > 2
    I = rgb2gray(I);
end
I=im2double(I);
angleRange = 'pi';

% function [G,a] = Gradient(I, angleRange)
%Gradient of I
%
%INPUT
%I:             a grey-scale image
%angleRange:    the angular span (see below)
%
%OUTPUT
%G:             gradient intensity
%a:             gradient direction. Values in the range
%                   [-pi/2,pi/2]   if angleRange = 'pi'
%                   [-pi,pi]       if angleRange = '2pi'
%               Contain NaNs where direction is undefined
%

% Version 0.1
% Author: Francesco Bianconi

%Define Sobel's masks
h=[ -1 0 1;-2 0 2;-1 0 1];
v=[ 1 2 1; 0 0 0; -1 -2 -1];

%Compute the gradient
Gh = imfilter (I,h,'circular');
Gv = imfilter (I,v,'circular');

%Compute the angle
switch angleRange
    case 'pi'
        a = atan(Gv./Gh);
    case '2pi'
        a = atan2(Gv,Gh);
    otherwise
        error('Undefined option');
end

%Compute the intensity
G = sqrt(Gv.^2 + Gh.^2);
% end

% imshow(G>0.8);
% K = edge(I,'sobel');
% figure,imshow(K);