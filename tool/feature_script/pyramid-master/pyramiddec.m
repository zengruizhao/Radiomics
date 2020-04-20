function imgDec = pyramiddec(imgSrc, level)
%pyramiddec对图像进行拉普拉斯金字塔分解
%   imgDec = pyramiddec(imgSrc, level)
%       返回值
%           imgDec: 分解得到的拉普拉斯金字塔  
%       参数
%           imgSrc: 被分解的图像
%           level: 金字塔的分解层数
%
%   例子：
%       img = imread('ngc6543a.jpg');
%       img = imresize(img, [512, 512]);
%       level = 5;
%       imgLaplacian = pyramiddec(img,level);
%
%   注意: 
%       - 被分解图像的大小必须是2的n次幂
%
%   编者：HeiMa2017
%   邮箱：hujie#szangell.com
%   版本：v0.0.0
%   网址：https://github.com/HeiMa2017/pyramid
%
% ================================================
%   日志：
%       2017-11-16：完成
% ================================================
%   Copyright (c) 2017 Angell.Co.Ltd. All rights reserved.

[imgHeight, imgWidth, imgThickness] = size(imgSrc);
% 检查函数参数是否合法
if(log2(imgHeight) - floor(log2(imgHeight)) > 0 || log2(imgWidth) - floor(log2(imgWidth)) > 0)
    disp('图像大小必须是2的幂。');
    return;
end
dimMin = min([imgHeight, imgWidth]);
maxLevel = log2(dimMin);
if(level > maxLevel)
    disp('超出了金字塔最大分解层数。');
    return;
end

if(imgThickness > 1)
    imgSrc = rgb2gray(imgSrc);
end
imgSrc = mat2gray(imgSrc);

% 拉普拉斯金字塔分解
imgReduce = cell(level, 1);
imgDec = cell(level, 1);
imgReduce{1} = imgSrc;
for iLevel = 2:level
    imgReduce{iLevel} = pyramidreduce(imgReduce{iLevel-1});
    imgDec{iLevel-1} = imgReduce{iLevel-1} - pyramidexpand(imgReduce{iLevel});
end
imgDec{iLevel} = imgReduce{iLevel};



