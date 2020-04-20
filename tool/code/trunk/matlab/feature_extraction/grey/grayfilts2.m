function [grayfeats, feat_names] = greyfilts2(img,WindowSize)
%function grayfeats = greyfilts2(img,WindowSize)
% Calculate 1st order gray-level features using 2D window
% img must be a 2D matrix
% Satish Viswanath, Jul 2008
% Further modded for arguments, Nov 2009
% Updated to use GRAYFILTS3 as template and more efficient code, Jul 2013

[height, width, planes] = size(img);

if planes > 1, error('Only 2D images supported, see GRAYFILTS3 otherwise.'); end;

nfeatures=4;
grayfeats=zeros(height, width, nfeatures, class(img));

% Extract features
fprintf('Calculating Mean Image.\n');
grayfeats(:,:,1) = meanfilt2(img,WindowSize);

fprintf('Calculating Median Image.\n');
if isa(img,'single'),
    imgd=double(img);
    tempfeat = medfilt2(imgd,[WindowSize,WindowSize]);
    grayfeats(:,:,2) = single(tempfeat);
else
    grayfeats(:,:,2) = medfilt2(img,[WindowSize,WindowSize]);
end

fprintf('Calculating Standard Deviation Image.\n');
if isa(img,'single'),
    imgd=double(img);
    tempfeat = stdfilt(imgd,true(WindowSize));
    grayfeats(:,:,3) = single(tempfeat);
else
    grayfeats(:,:,3) = stdfilt(img,true(WindowSize));
end

fprintf('Calculating Range Image.\n');
if isa(img,'single'),
    imgd=double(img);
    tempfeat = rangefilt(imgd,true(WindowSize));
    grayfeats(:,:,4) = single(tempfeat);
else
    grayfeats(:,:,4) = rangefilt(img,true(WindowSize));
end

feat_names = {'mean','median','std_dev','range'};


end

% Filter subroutines
function Y = meanfilt2(img,WindowSize)

kern = fspecial('average',[WindowSize,WindowSize]);
Y = filter2(kern,img,'same');

end