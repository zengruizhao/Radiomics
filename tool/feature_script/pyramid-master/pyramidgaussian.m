function filter = pyramidgaussian(a, sizeFilter)
%pyramidgaussian 金字塔的低通滤波器
%   filter = pyramidgaussian(a, sizeFilter)
%       返回值：
%           filter:  生成的滤波器核
%       参数：
%           a: 滤波器生成参数，一般设为0.375
%           sizeFilter: 滤波器大小
%
%   注意: 
%       - 下层图像大小是上层图像大小的一半
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

filterCol = zeros(sizeFilter, 1);
filterCol(1:end) = [1/4-a/2; 1/4; a; 1/4; 1/4-a/2];
filterRow = filterCol';
filter = filterCol*filterRow;