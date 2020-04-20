% Calculate gray-level features using 2D window (can accept Volume input)
% 
% Usage: GreyLevelFeatures = greylevel(inVol, WindowSize);
% 
% WindowSize is a vector of different window sizes.
% Created by Rob TOth 23-Jul-2010.
% 
function GreyLevelFeatures = greylevel(inVol, WindowSize)
    % define the filters
    i = 0;
    for j = 1:length(WindowSize)
        i = i + 1; myFilter{i} = repmat(1/WindowSize(j).^2,WindowSize(j),WindowSize(j)); % mean
    end
    i = i + 1; myFilter{i} = [3 3 3;3 0 3; -5 -5 -5]; % kirsch 1
    i = i + 1; myFilter{i} = [-5 3 3;-5 0 3; -5 3 3]; % kirsch 2
    i = i + 1; myFilter{i} = [3 3 3;-5 0 3; -5 -5 3]; % kirsch 3
    i = i + 1; myFilter{i} = [0 1 2; -1 0 1; -2 -1 0]; % sobel xy
    i = i + 1; myFilter{i} = flipdim([0 1 2; -1 0 1; -2 -1 0], 2); % sobel yx
    i = i + 1; myFilter{i} = [1 2 1; 0 0 0; -1 -2 -1]; % sobel y
    i = i + 1; myFilter{i} = [-1 0 1;-2 0 2;-1 0 1]; % sobel x
    i = i + 1; myFilter{i} = [-1 1]; % dx
    i = i + 1; myFilter{i} = [-1; 1]; % dy
    i = i + 1; myFilter{i} = [-1 0;0 1]; % ddiag

    filterFeatures = i;
    gradFeatures = 3;
    otherFeatures = 3 * length(WindowSize);
    numFeatures = filterFeatures + gradFeatures + otherFeatures;

    % run the filters
    [height, width, slices] = size(inVol);
    GreyLevelFeatures = ones(height, width, slices, numFeatures, 'single');
    for j = 1:filterFeatures
        GreyLevelFeatures(:, :, :, j) = convn(inVol, myFilter{j}, 'same');
    end
    
    % gradient features
    [GreyLevelFeatures(:, :, :, filterFeatures + 1), GreyLevelFeatures(:, :, :, i + 2)] = ...
        gradient(inVol);
    GreyLevelFeatures(:, :, :, filterFeatures + 3) =  sqrt( ...
        GreyLevelFeatures(:,:,:,filterFeatures + 1).^2 + ... 
        GreyLevelFeatures(:,:,:,filterFeatures + 2).^2);
    
    % mean std and range filters
    count = filterFeatures + gradFeatures + 1;
    for i = count:3:numFeatures
        wSize = WindowSize(floor((i - count) / 3) + 1);
        nSize = true(wSize);
        for j = 1:slices
            GreyLevelFeatures(:, :, j, i + 0) = medfilt2(inVol(:, :, j), [wSize, wSize]);
            GreyLevelFeatures(:, :, j, i + 1) = stdfilt(inVol(:, :, j), nSize);
            GreyLevelFeatures(:, :, j, i + 2) = rangefilt(inVol(:, :, j), nSize);
        end
    end % for i
    
end % function