function imgNext = pyramidreduce(imgPrev)
%pyramidreduce ��˹�������²���
%   imgNext = pyramidreduce(imgPrev)
%       ����ֵ��
%           imgNext: ���ϲ�ͼ���˲����²�����ͼ�� 
%       ������
%           imgPrev: �ϲ������ͼ��
%
%   ע��: 
%       - �߽紦��ʽ��ͼ���Ե��һ��Ӱ�죬���öԳƲ��߽Ϻ�
%       - ��������߼���ײ�����C++����һ�£��ǳ�����ת����
%         C++����
%
%   ���ߣ�HeiMa2017
%   �汾��v0.0.0
%   ���䣺hujie#szangell.com
%   ��ַ��https://github.com/HeiMa2017/pyramid
%
% ================================================
%   ��־��
%       2017-11-13�����
% ================================================
%   Copyright (c) 2017 Angell.Co.Ltd. All rights reserved.

[heightPrev, widthPrev] = size(imgPrev);
radiusFilter = 2; % �˲�����СΪ����
kernelFilter = pyramidgaussian(0.375, 2*radiusFilter+1);
heightNext = heightPrev/2;
widthNext = widthPrev/2;
imgNext = zeros(heightNext,widthNext);
for iRow = 1:heightNext
    rowBase = 2*iRow;
    if(rowBase > heightPrev)
        rowBase = 2*heightPrev - rowBase;
    end % ���öԳƲ��������б߽�
    for jCol = 1:widthNext
        colBase = 2*jCol;
        if(colBase > widthPrev)
            colBase = 2*widthPrev - colBase;
        end
        for iFilter = -radiusFilter:radiusFilter
            rowPrevImg = rowBase + iFilter;
            if(rowPrevImg < 1)
                rowPrevImg = 2*1 - rowPrevImg;
            elseif(rowPrevImg > heightPrev)
                rowPrevImg = 2*heightPrev - rowPrevImg;
            end % ���öԳ����ز��б�
            for jFilter = -radiusFilter:radiusFilter
                colPrevImg = colBase + jFilter;
                if(colPrevImg < 1)
                    colPrevImg = 2*1 - colPrevImg;
                elseif(colPrevImg > widthPrev)
                    colPrevImg = 2*widthPrev - colPrevImg;
                end % ���öԳ����ز��б�
                imgNext(iRow, jCol) = imgNext(iRow, jCol) + ...
                    kernelFilter(iFilter+radiusFilter+1, jFilter+radiusFilter+1)...
                *imgPrev(rowPrevImg, colPrevImg);
            end
        end
    end
end