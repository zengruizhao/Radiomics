% Usage:
%   resized = resizeimages(images);
%   resized = resizeimages(images, options);
% 
% images is a cell array of images
% 
% Type "help resizeimage" to see all the options.
% 
% If no options are given, then the largest number of rows,
% cols, and slices are calculated, and used for all images.
% 
% Created by Rob Toth, 2010-11-18.
% 
function resized = resizeimages(images, varargin)

  % must have 1-2 inputs
  min_args = 1;
  max_args = 2;
  if ( verLessThan('matlab', '7.12.1') )
    % deprecated
    error(nargchk(min_args, max_args, nargin));
  else
    narginchk(min_args, max_args);
  end
  
  % check the inputs
  parser = inputParser;
  parser.StructExpand  = true;
  parser.KeepUnmatched = true;
  parser.addRequired('images', ...
    @(x)(isnumeric(x) && (ndims(x) == 2 || ndims(x) == 3) || ...
    iscell(x) && length(x) >= 1  && ...
    all(cellfun(@ndims, x) >= 2) && ...
    all(cellfun(@ndims, x) <= 3) && ...
    all(cellfun(@isreal, x))));
  parser.parse(images, varargin{:});
  
  % do we have a cell array as our input?
  no_cell = ~iscell(images);
  if no_cell, images = {images}; end
    
  num_images = length(images);
  
  if isempty(varargin)
    
    if no_cell
      % nothing to do
      resized = images;
      return;
    end
    
    % calculate the largest rows, cols, slices
    options = struct;
    max_rows = 0; max_cols = 0; max_slices = 0;
    for i = 1:num_images
      [rows, cols, slices] = size( images{i} );
      if rows   > max_rows  , max_rows   = rows  ; end
      if cols   > max_cols  , max_cols   = cols  ; end
      if slices > max_slices, max_slices = slices; end
    end % for i
    
    % set our options
    options.num_rows   = max_rows  ;
    options.num_cols   = max_cols  ;
    options.num_slices = max_slices;
    
  else
    
    % valid input, just use the input options
    options = varargin{1};  
    
  end % length
  
  % now, resize each image
  resized = cell(num_images, 1);
  for i = 1:num_images
    
    % get our current image
    current_image = images{i};
    
    % resize our current image
    resized{i} = resizeimage(current_image, options);
    
  end % for i
  
  % output
  if no_cell
    resized = resized{1};
  end
  
end