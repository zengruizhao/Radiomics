function imgNext = pyramidexpand(imgPrev)
%pyramidexpand 高斯金字塔上采样
%   imgNext = pyramidreduce(imgPrev)
%       返回值：
%           imgNext:  对下层图像上采样后滤波的图像  
%       参数：
%           imgPrev: 上层金字塔图像
%
%   注意: 
%       - 边界处理方式对图像边缘有一定影响，采用对称补边较好
%       - 程序编码逻辑与底层语言C++基本一致，非常容易转换成
%         C++程序
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

[heightPrev, widthPrev] = size(imgPrev);
radiusFilter = 2; % 滤波器大小为奇数
kernelFilter = pyramidgaussian(0.375, 2*radiusFilter+1);
heightNext = 2*heightPrev;
widthNext = 2*widthPrev;
imgNext = zeros(heightNext,widthNext);
for iRow = 1:heightNext
    for jCol = 1:widthNext
        for iFilter = -radiusFilter:radiusFilter
            rowPrevImg = (iRow + iFilter)/2;
            if(rowPrevImg - floor(rowPrevImg) > 0)
                continue;
            end
            if(rowPrevImg < 1)
                rowPrevImg = 2*1 - rowPrevImg;
            elseif(rowPrevImg > heightPrev)
                rowPrevImg = 2*heightPrev - rowPrevImg;
            end % 采用对称像素补列边
            for jFilter = -radiusFilter:radiusFilter
                colPrevImg = (jCol + jFilter)/2;
                if(colPrevImg - floor(colPrevImg) > 0)
                    continue;
                end
                if(colPrevImg < 1)
                    colPrevImg = 2*1 - colPrevImg;
                elseif(colPrevImg > widthPrev)
                    colPrevImg = 2*widthPrev - colPrevImg;
                end % 采用对称像素补行边
                imgNext(iRow, jCol) = imgNext(iRow, jCol) + ...
                    4*kernelFilter(iFilter+radiusFilter+1, jFilter+radiusFilter+1)...
                    *imgPrev(rowPrevImg, colPrevImg);
            end
        end
    end
end