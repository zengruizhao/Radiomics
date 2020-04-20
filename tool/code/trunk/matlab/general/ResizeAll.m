% Usage: resized = ResizeAll(images, options);
% 
% This functions samples all images in the cell array images, so that
% num_rows, num_cols, and num_slices are the same for each image in resized
% 
% options (optional) is a struct which can contain the following fields:
%  - num_rows
%  - num_cols
%  - num_slices
%  - interpolation (see documentation for interp3)
% 
% if num_* isn't specified, then the largest value will be determined.
%
% Created by Rob Toth, 2010-11-18.
% 
function resized = ResizeAll(images, options)

    if ~exist('options', 'var') || isempty(options) || ~isstruct(options), options = struct; end
    if ~isfield(options, 'interpolation'), options.interpolation = 'linear'; end
    if ~isfield(options, 'display'      ), options.display       = true    ; end
    
    % do we have the ba_interp3 mex file compiled?
    use_ba = (exist('ba_interp3', 'file') == 3);
    
    % if it's not a cell, we just consider it one big image
    no_cell = ~iscell(images);
    if no_cell, images = {images}; end
    
    % get the size of the images
    num_images = length(images);
    rows     = ones(size(images));
    cols     = ones(size(images));
    slices   = ones(size(images));
    features = ones(size(images));
    for i = 1:num_images
        [rows(i), cols(i), slices(i), features(i)] = size(images{i});
    end % for i
    
    % are the values specified?  if not, use the max
    if isfield(options, 'num_rows'  ), num_rows   = options.num_rows  ;
    else                               num_rows   = max(rows)         ; end
    if isfield(options, 'num_cols'  ), num_cols   = options.num_cols  ;
    else                               num_cols   = max(cols)         ; end
    if isfield(options, 'num_slices'), num_slices = options.num_slices;
    else                               num_slices = max(slices)       ; end
    
    % resize each image
    resized = cell(size(images));
    for i = 1:num_images
        
        num_features = features(i);
        if ndims(images{i}) > 4 && ~use_ba
            error('Cannot handle images with >4 dimensions.');
        end
        
        if cols(i) ~= num_cols || rows(i) ~= num_rows || slices(i) ~= num_slices || features(i) ~= num_features

            if options.display
                fprintf('Resizing image %d of %d from %dx%dx%d to %dx%dx%d...', ...
                    i, num_images, ...
                    cols(i), rows(i), slices(i), ...
                    num_cols, num_rows, num_slices);
            end

            % where are we sampling?
            [mesh_x, mesh_y, mesh_z] = meshgrid( ...
                linspace(1, cols    (i), num_cols  ), ...
                linspace(1, rows    (i), num_rows  ), ...
                linspace(1, slices  (i), num_slices));

            % resize the image
            resized{i} = ones(num_rows, num_cols, num_slices, num_features, class(images{i}));
            if use_ba % ba_interp3 can handle >3 dimensions
                new_class = 'double';
                resized{i} = ba_interp3( ...
                    cast(images{i}, new_class), ...
                    cast(mesh_x   , new_class), ...
                    cast(mesh_y   , new_class), ...
                    cast(mesh_z   , new_class), ...
                    options.interpolation);
                resized{i} = cast( resized{i}, old_class );
            else
                % interp3 can't handle >3 dimensions
                for k = 1:num_features % each feature is independent, so don't include them with interpolation
                    resized{i}(:, :, :, k) = interp3(double(images{i}(:, :, :, k)), mesh_x, mesh_y, mesh_z, options.interpolation);
                end % for k
            end
            
            if options.display, fprintf('done.\n'); end

        else
            % already that size
            resized{i} = images{i};
            
        end
        
        
    end % for i
    
    % convert back to a non-cell
    if no_cell, resized = resized{1}; end

end