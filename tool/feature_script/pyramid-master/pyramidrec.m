function imgRec = pyramidrec(imgDec)
%pyramidrec 高斯金字塔递重构
%   imgRec = pyramidrec(imgDec)
%       返回值：
%           imgRec: 重构的图像 
%       参数：
%           imgDec: 拉普拉斯金字塔分解图像
%
%   例子： 
%       img = imread('ngc6543a.jpg');
%       img = imresize(img, [512, 512]);
%       level = 5;
%       imgLaplacian = pyramiddec(img,level);
%       imgRec = pyramidrec(imgLaplacian);
%
%   注意: 
%       - 拉普拉斯金字塔层数毕高斯金字塔层数少1
%
%   编者：HeiMa2017
%   版本：v0.0.0
%   邮箱：hujie#szangell.com
%   网址：https://github.com/HeiMa2017/pyramid
%
% ================================================
%   日志：
%       2017-11-16：完成
% ================================================
%   Copyright (c) 2017 Angell.Co.Ltd. All rights reserved.

level = size(imgDec, 1);
imgRec = imgDec{level};
for i = level:-1:2
    imgRec = imgDec{i-1} + pyramidexpand(imgRec);
end
