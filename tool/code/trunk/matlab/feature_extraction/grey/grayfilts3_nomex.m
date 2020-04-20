function grayfeats = grayfilts3_nomex(vol,WindowSize)
% Calculate gray-level features using 3D window

[height, width, planes] = size(vol);

if numel(WindowSize)==1, WindowSize=WindowSize([1 1 1]); end
if numel(WindowSize)~=3, error('WindowsSize must be 1 or 3 elements.'); end

nfeatures=4;
grayfeats=zeros(height, width, planes, nfeatures,class(vol));

% Extract features
fprintf('Calculating Mean Image.\n');
grayfeats(:,:,:,1) = meanfilt3(vol,WindowSize);

% fprintf('Calculating Median Image.\n');
% grayfeats(:,:,:,2) = medianfilt3(vol,WindowSize);
% 
% fprintf('Calculating Standard Deviation Image.\n');
% grayfeats(:,:,:,3) = stdfilt3(vol,WindowSize,grayfeats(:,:,:,1));
% 
% fprintf('Calculating Range Image.\n');
% grayfeats(:,:,:,4) = rangefilt3(vol,WindowSize);

fprintf('Calculating Median, Std, and Range Images.\n');
grayfeats(:,:,:,2:4) = statsfilt3(vol,WindowSize,grayfeats(:,:,:,1));

end

% Filter subroutines
function Y = meanfilt3(vol,WindowSize)

mask = repmat(1/prod(WindowSize),WindowSize);
Y = convn(vol,mask,'same');

end

function Y = statsfilt3(vol,WindowSize,meanimage)

% Half window dimensions
hw = floor(WindowSize./2);
% volume dimensions
[Height,Width,planes] = size(vol);
% Output same size as input
Y = zeros([Height,Width,planes,3]);

% for every voxel, calculate median in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k,1) = median(wind(:));
            Y(i,j,k,2) = sqrt(sum((wind(:)-meanimage(i,j,k)).^2)/numel(wind));
            Y(i,j,k,3) = max(wind(:))-min(wind(:));
        end
    end
    fprintf('.')
end
fprintf('\n');

end

function Y = medianfilt3(vol,WindowSize)

% Half window dimensions
hw = floor(WindowSize./2);
% volume dimensions
[Height,Width,planes] = size(vol);
% Output same size as input
Y = zeros([Height,Width,planes]);

% for every voxel, calculate median in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k)=median(wind(:));
        end
    end
    fprintf('.')
end
fprintf('\n');

end

function Y = stdfilt3(vol,WindowSize,meanimage)

[Height,Width,planes] = size(vol);
Y = zeros(Height, Width,planes);

hw = floor(WindowSize./2);

% for every voxel, calculate STD in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k) = sum((wind(:)-meanimage(i,j,k)).^2)/numel(wind);
        end
    end
    fprintf('.')
end
fprintf('\n');

end

function Y = rangefilt3(vol,WindowSize)

[Height, Width, planes] = size(vol);
Y = zeros(Height, Width, planes);

hw = floor(WindowSize./2);

% for every voxel, calculate STD in window
for k=1:planes,
    zinds=k-hw(3):k+hw(3);
    zinds(zinds<1 | zinds>planes)=[];
    for i = 1:Height
        yinds=i-hw(1):i+hw(1);
        yinds(yinds<1 | yinds>Height)=[];
        for j = 1:Width
            xinds=j-hw(2):j+hw(2);
            xinds(xinds<1 | xinds>Width)=[];

            wind=vol(yinds,xinds,zinds);
            Y(i,j,k) = max(wind(:))-min(wind(:));
        end
    end
    fprintf('.')
end
fprintf('\n');

end
