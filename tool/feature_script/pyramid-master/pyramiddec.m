function imgDec = pyramiddec(imgSrc, level)
%pyramiddec��ͼ�����������˹�������ֽ�
%   imgDec = pyramiddec(imgSrc, level)
%       ����ֵ
%           imgDec: �ֽ�õ���������˹������  
%       ����
%           imgSrc: ���ֽ��ͼ��
%           level: �������ķֽ����
%
%   ���ӣ�
%       img = imread('ngc6543a.jpg');
%       img = imresize(img, [512, 512]);
%       level = 5;
%       imgLaplacian = pyramiddec(img,level);
%
%   ע��: 
%       - ���ֽ�ͼ��Ĵ�С������2��n����
%
%   ���ߣ�HeiMa2017
%   ���䣺hujie#szangell.com
%   �汾��v0.0.0
%   ��ַ��https://github.com/HeiMa2017/pyramid
%
% ================================================
%   ��־��
%       2017-11-16�����
% ================================================
%   Copyright (c) 2017 Angell.Co.Ltd. All rights reserved.

[imgHeight, imgWidth, imgThickness] = size(imgSrc);
% ��麯�������Ƿ�Ϸ�
if(log2(imgHeight) - floor(log2(imgHeight)) > 0 || log2(imgWidth) - floor(log2(imgWidth)) > 0)
    disp('ͼ���С������2���ݡ�');
    return;
end
dimMin = min([imgHeight, imgWidth]);
maxLevel = log2(dimMin);
if(level > maxLevel)
    disp('�����˽��������ֽ������');
    return;
end

if(imgThickness > 1)
    imgSrc = rgb2gray(imgSrc);
end
imgSrc = mat2gray(imgSrc);

% ������˹�������ֽ�
imgReduce = cell(level, 1);
imgDec = cell(level, 1);
imgReduce{1} = imgSrc;
for iLevel = 2:level
    imgReduce{iLevel} = pyramidreduce(imgReduce{iLevel-1});
    imgDec{iLevel-1} = imgReduce{iLevel-1} - pyramidexpand(imgReduce{iLevel});
end
imgDec{iLevel} = imgReduce{iLevel};



