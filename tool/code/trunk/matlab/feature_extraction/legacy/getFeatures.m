% Wrapper to extract features from a set of images.
% 
% Usage: featureImages = getFeatures(images);
% 
% images can be a matrix of a 2D or 3D intensity image.
% if images is a cell array of 2D or 3D intensity images, 
%   then featureImages will also be a cell array.
%   It should be noted that if images is a cell array, 
%   each image need not necessarily be the same size.
% 
% the fourth dimension of featureImages will be the extracted features.
% 
% Requires an 'fextract' function.
% 
% Created by Rob Toth, May-2010.
% 
function featureImages = getFeatures(images)

	fprintf('\ngetFeatures running.\n'); tStartAll1 = tic;
	
    % check if there's the feature extraction code
    if (exist('fextract', 'file') ~= 2)
        fprintf('\nSorry, but there''s no ''fextract'' function! Aborting.\n\n');
        return;
    end
    
    % initialize the images    
    noCell = ~iscell(images);
    if noCell, images = {images}; end
    numImages = length(images);
    featureImages = cell(numImages, 1);

    % initialize the features       
    doGrey = true; doHaralick = false; doGabor = true;
    doFeatures = [doGrey, doGabor, doHaralick];
    temp = fextract(ones(3, 3, 1), doFeatures); % sample feature extraction
    numFeatures = size(temp, 3);

    % convert to uint16 to save memory.  this is possible if we know
    % that the data will range from 0 to 1 for every value.
    featureClass = 'single'; % for extracting the features
    newClass = 'int16'; % for outputing the features
    maxValue = double(intmax(newClass));
    minValue = double(intmin(newClass));

    % ask the user if (s)he can handle the memory requirements
    temp = cast(0, newClass); % try out the new class
    temp = whos('temp'); % get info about the variable
    numBytes = temp.bytes; % how many bytes is each element
    numElements = 0;
    for i = 1:numImages
        numElements = numElements + numel(images{i});
    end
    numElements = numElements * numFeatures;
    numBytes = numBytes * numElements;
    numBytes = numBytes / 1024^3; % convert to gb
    temp = [sprintf('\nFinal matrix of features will '), ...
        sprintf('be %.2f GB of %s.\n', numBytes, newClass), ...
        'To cancel feature extraction, type ''quit''.  ', ...
        'Otherwise, press enter. ']; % get the message
    temp = input(temp, 's'); % get user input
    fprintf('\n');
    if strcmp(temp, 'quit'), return; end % exit    
    
    if matlabpool('size') == 0 % not in parallel!
        temp = ['It is recommended that you run getFeatures in parallel.', ...
            '\nWould you like to turn on the parallel computing toolbox?', ...
            '\nType ''yes'' to run ''matlabpool open'',\n', ...
            'type ''quit'' to exit getFeatures,\n', ...
            'or press enter to continue: '];
        temp = input(sprintf(temp), 's');
        fprintf('\n');
        if strcmp(temp, 'yes')
            matlabpool open;
        elseif strcmp(temp, 'quit')
            return;
        end
    end % matlabpool

    % initialize the output
    for i = 1:numImages
        featureImages{i} = zeros(size(images{i}, 1), ...
            size(images{i}, 2), size(images{i}, 3), ...
            numFeatures, newClass);
    end % for o

    % will need this later
    allMin = zeros(numImages, numFeatures, featureClass);
    allMax = allMin;
    tSum = 0;

    % preprocess images (bias field correct, normalization, etc.)
    if exist('preprocess', 'file') == 2 % there is a preprocess function
        images = preprocess(images);
    end

    % texture features
    fprintf('\nExtracting %d features from %d images:\n', numFeatures, numImages);
	tStartAll2 = tic;
    for i = 1:numImages
        fprintf('\tImage %d of %d...', i, numImages); tStart = tic;

        % initialize
        [numRows, numCols, numSlices] = size(images{i});
        tempFeatures = ones(numRows, numCols, numSlices, ...
            numFeatures, featureClass);

        % extract the feature images
        tempImage = images{i}; % slice it.
        parfor slice = 1:numSlices
            tempFeatures(:, :, slice, :) = ...
                fextract(tempImage(:, :, slice), ...
                [doGrey, doGabor, doHaralick]);
        end % for slice
            
        parfor f = 1:numFeatures
             
            % save these for later
            allMin(i, f) = min(min(min(tempFeatures(:, :, :, f))));
            allMax(i, f) = max(max(max(tempFeatures(:, :, :, f))));
            
            % scale from 0 to 1
            tempFeatures(:, :, :, f) = jrescale(tempFeatures(:, :, :, f));
        end
            
        % stretch and cast to new class
        featureImages{i}(:, :, :, 1:numFeatures) = ...
            cast(tempFeatures * ...
            (maxValue - minValue) + minValue, newClass);
        
        % display progress
        tSum = tSum + toc(tStart);
        tLeft = round(tSum * numImages / i - tSum);
        fprintf('done in %.1f seconds, %d seconds remaining.\n', toc(tStart), tLeft);

    end % for i

    fprintf('Feature extraction done in %d seconds.\n\n', round(toc(tStartAll2)));

    % linearly rescale the data to the correct range    
    fprintf('Rescaling data...'); tStart = tic;
        
    lowestMin = min(allMin, 1); % across all images
    highestMax = max(allMax, 1); % across all images
    largestRange = highestMax - lowestMin;
    finalRange = double(maxValue - minValue); % how will the final data be scaled?
        
    for i = 1:numImages % parfor makes it slower, but feel free to try
        for f = 1:numFeatures
                
            % how far "into" the range should the new min and max be?
            relativeMin = (allMin(i, f) - lowestMin(f)) / largestRange(f);
            relativeMax = (allMax(i, f) - lowestMin(f)) / largestRange(f);
                
            % what should the actual values for the new min and max be?
            newMin = cast(relativeMin * finalRange + minValue, newClass);
            newMax = cast(relativeMax * finalRange + minValue, newClass);
            newRange = double(newMax - newMin);
                
            % currently, the min and max are minValue and maxValue
            % this is where we actually rescale the data
            featureImages{i}(:, :, :, f) = (newRange / finalRange) * ...
                (featureImages{i}(:, :, :, f) - minValue) + newMin;
                
        end % for f
    end % for i
    fprintf('done in %d seconds.\n\n', round(toc(tStart))); % rescaling
    
    fprintf('getFeatures finished running in %d seconds.\n\n', round(toc(tStartAll1)));
    if noCell, featureImages = featureImages{1}; end
    
end % function
