function b = makemontage(a,aspectRatio)
% MAKEMONTAGE Make a montage from several images
%   MAKEMONTAGE(A) outputs a 2D montage of the M-by-N-by-O array A.
%
%   Thanks to MATLAB's image processing toolbox!
%
%JC

% % a must be M-by-N-by-1-by-P
% if ndims(a)==3,
%     a=permute(a,[1 2 4 3]);
% end
% 
% siz = [size(a,1) size(a,2) size(a,4)];
% nn = sqrt(prod(siz))/siz(2);
% mm = siz(3)/nn;
% if (ceil(nn)-nn)+10*eps >= (ceil(mm)-mm), % >= to prefer wide to tall
%   nn = ceil(nn); mm = ceil(siz(3)/nn);
% else
%   mm = ceil(mm); nn = ceil(siz(3)/mm);
% end
% 
% b = a(1,1); % to inherit type 
% b(1,1) = 0; % from a
% b = repmat(b, [mm*siz(1), nn*siz(2), size(a,3), 1]);
% 
% rows = 1:siz(1); cols = 1:siz(2);
% for i=0:mm-1,
%   for j=0:nn-1,
%     k = j+i*nn+1;
%     if k<=siz(3),
%       b(rows+i*siz(1),cols+j*siz(2),:) = a(:,:,:,k);
%     end
%   end
% end

if nargin<2, aspectRatio=1; end

if ndims(a)==3,
    a=permute(a,[1 2 4 3]);
end

[nRows,nCols,nBands,nFrames]=size(a);
indices=1:nFrames;

% Estimate nMontageColumns and nMontageRows given the desired
% ratio of Columns to Rows to be one (square montage).
% aspectRatio = 1.3;
montageCols = sqrt(aspectRatio * nRows * nFrames / nCols);

% Make sure montage rows and columns are integers. The order in
% the adjustment matters because the montage image is created
% horizontally across columns.
montageCols = ceil(montageCols);
montageRows = ceil(nFrames / montageCols);
% montageSize = [montageRows montageCols];

sizeOfBigImage = [montageRows*nRows montageCols*nCols nBands 1];
if islogical(a)
    b = false(sizeOfBigImage);
else
    b = zeros(sizeOfBigImage, class(a));
end

rows = 1:nRows;
cols = 1:nCols;
k = 1;

for i = 0 : montageRows-1
    for j = 0 : montageCols-1,
        if k <= nFrames
            b(rows + i * nRows, cols + j * nCols, :) = ...
                a(:,:,:,indices(k));
        else
            return;
        end
        k = k + 1;
    end
end
