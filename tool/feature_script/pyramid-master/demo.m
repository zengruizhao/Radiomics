%demo 利用pyramiddec和pyramidrec对图像进行分解和
%重构，并验证分解和重构的可逆性
%
%   注意：
%       - 如果图像大小不是2的n此幂，应当采取适当
%         方式调整
%
%   编者：HeiMa2017
%   版本：v0.0.0
%   邮箱：hujie#szangell.com
%   网址：https://github.com/HeiMa2017/pyramid
%
% ================================================
%   日志：
%       2017-11-13：完成
% ================================================
%   Copyright (c) 2017 Angell.Co.Ltd. All rights reserved.

clear;
clc;
close all;

img = imread('ngc6543a.jpg');
img = rgb2gray(img);
img = imresize(img, [512, 512]);

% 分解
level = 9;
imgLaplacian = pyramiddec(img,level);
figure, 
subplot(221), imshow(mat2gray(imgLaplacian{1}));
subplot(222), imshow(mat2gray(imgLaplacian{2}));
subplot(223), imshow(mat2gray(imgLaplacian{3}));
subplot(224), imshow(mat2gray(imgLaplacian{4}));

% 重构
imgRec = pyramidrec(imgLaplacian);
figure, 
subplot(121),imshow(imgRec);
subplot(122),imshow(img);

% 验证重构图像是否正确
imgRec = im2uint8(mat2gray(imgRec));
delta = sum(sum(img - imgRec))
