% Usage:
%   [resized, output_voxel_size] = resizeimage(input_image, options);
% 
% input_image is a 2D or 3D image.
% 
% options is a struct which can contain the following fields:
% 
% display (default = false) will show progress if display is non-zero.
% 
% input_voxel_size (default = [1, 1, 1]) the size of each voxel in the
% input image.  Note that it goes [x, y, z], not [rows, cols, slices].
% 
% output_voxel_size (default is automatically calculated)
% the size of each voxel in the output image.
% 
% output_rows   (default is automatically calculated)
% output_cols   (default is automatically calculated)
% output_slices (default is automatically calculated)
% 
% interpmethod (default = 'Linear') either 'Linear', 'Nearest', 'Cubic'
% 
% If you just pass in the output_voxel_size, then resizeimage will
% calculate how many rows, cols, and slices are required so that every
% output voxel has an interpolated value.
% 
% If you pass in just the output_rows (or cols or slices), and no
% output_voxel_size, then it will automatically be calculated.
% 
% If you pass in both the output_voxel_size and output_rows, then things
% get a bit tricky.  The resulting rows might not all contain data from the
% input image, which could result in "black" regions if you go outside the
% input_image.
% 
% For output_voxel_size, you can pass in NaN for any of the dimensions, and
% for that dimension it will act like you passed nothing in, and it will be
% calculated for you.
% 
% If you pass in a 2D image, all z information is ignored.
% 
% See the following website for a description of input_voxel_size
% and output_voxel_size:
% http://tinyurl.com/89ga76o
% 
% Created by Rob Toth, 2011-11-22
% 
function [resized, output_voxel_size] = resizeimage(input_image, options)

  % check our inputs
  parser = inputParser;
  parser.StructExpand  = true;
  parser.KeepUnmatched = true;
  parser.addRequired('input_image', ...
    @(x)isnumeric(x) && (ndims(x) == 2 || ndims(x) == 3) ); 
  parser.addParamValue('display', false, ...
    @(x) isscalar(x) && isnumeric(x) && x >= 0);
	interpmethods={'Linear','Cubic','Nearest'};
  parser.addParamValue('interpmethod', 'Linear', ...
    @(x) any(strcmpi(x,interpmethods)) );
  parser.addParamValue('input_voxel_size', [1, 1, 1], ...
    @(x) isnumeric(x) && isvector(x) && (~any(isnan(x))) && (~any(x == 0)) ...
    && (length(x) == 3 || length(x) == 2));
  parser.addParamValue('output_voxel_size', ...
    @(x) isnumeric(x) && isvector(x) && (~any(x == 0)) ...
    && (length(x) == 3 || length(x) == 2));
  scalar_attributes = {'scalar', 'positive', 'real'};
  parser.addParamValue('output_rows', ...
    @(x) validateattributes(x, {'numeric'}, scalar_attributes));
  parser.addParamValue('output_cols', ...
    @(x) validateattributes(x, {'numeric'}, scalar_attributes));
  parser.addParamValue('output_slices', ...
    @(x) validateattributes(x, {'numeric'}, scalar_attributes));  
  parser.parse(input_image, options);
  
  % extract our input values
  input_results     = parser.Results;
  defaults = parser.UsingDefaults;
  no_output_voxel_size = any(strcmpi('output_voxel_size', defaults));
  no_output_rows       = any(strcmpi('output_rows'      , defaults));
  no_output_cols       = any(strcmpi('output_cols'      , defaults));
  no_output_slices     = any(strcmpi('output_slices'    , defaults));
  display              = input_results.display > 0;
  input_voxel_size     = input_results.input_voxel_size;
  output_voxel_size    = input_results.output_voxel_size;
  interpmethod         = input_results.interpmethod;
  is2d                 = (ndims(input_image) == 2);

  % get the size of the image
  [input_rows, input_cols, input_slices] = size(input_image);

  % get the output size
  [output_rows, output_max_y, input_max_y] = GetOutputElements( ...
    no_output_rows, ...
    (no_output_voxel_size || isnan(output_voxel_size(2))), ...
    output_voxel_size(2), input_voxel_size(2), ...
    input_results.output_rows, input_rows);
  [output_cols, output_max_x, input_max_x] = GetOutputElements( ...
    no_output_cols, ...
    (no_output_voxel_size || isnan(output_voxel_size(1))), ...
    output_voxel_size(1), input_voxel_size(1), ...
    input_results.output_cols, input_cols);
  [output_slices, output_max_z, input_max_z] = GetOutputElements( ...
    no_output_slices, ...
    (no_output_voxel_size || isnan(output_voxel_size(3))), ...
    output_voxel_size(3), input_voxel_size(3), ...
    input_results.output_slices, input_slices);
  
  % where are the input voxels?
  % the first voxel is considered to be at [0mm, 0mm, 0mm]
  [input_mesh_x, input_mesh_y, input_mesh_z] = meshgrid( ...
    linspace(0, input_max_x, input_cols), ...
    linspace(0, input_max_y, input_rows), ...
    linspace(0, input_max_z, input_slices));
  
%   [input_mesh_x, input_mesh_y, input_mesh_z] = meshgrid( ...
%     ((1:input_cols  ) - 1) * input_voxel_size(1), ...
%     ((1:input_rows  ) - 1) * input_voxel_size(2), ...
%     ((1:input_slices) - 1) * input_voxel_size(3) );
  
  % where are the output voxels?
  [output_mesh_x, output_mesh_y, output_mesh_z] = meshgrid( ...
    linspace(0, output_max_x, output_cols), ...
    linspace(0, output_max_y, output_rows), ...
    linspace(0, output_max_z, output_slices) );
  
  % what is the output voxel size?
  output_voxel_x = output_mesh_x(1, 2, 1);
  output_voxel_y = output_mesh_y(2, 1, 1);
  if is2d
    output_voxel_z = input_voxel_size(3);
  else
    output_voxel_z = output_mesh_z(1, 1, 2);
  end % is2d
  output_voxel_size = [output_voxel_x, output_voxel_y, output_voxel_z];

  if display
    fprintf('Resizing image from %dx%dx%d to %dx%dx%d...', ...
      input_rows , input_cols , input_slices, ...
      output_rows, output_cols, output_slices );
  end % display

  % resize the image
  old_class = class(input_image);
  new_class = 'double';
  input_image = cast(input_image, new_class);
  if ndims(input_image) == 2
    % 2D
    resized = interp2( ...
      input_mesh_x(:, :, 1), ...
      input_mesh_y(:, :, 1), ...
      input_image, ...
      output_mesh_x(:, :, 1), ...
      output_mesh_y(:, :, 1), ...
      interpmethod );
  else
    % 3D
    
    % do we have the ba_interp3 mex file compiled?
    use_ba = (exist('ba_interp3', 'file') == 3);

    if use_ba
      resized = ba_interp3( ...
        input_mesh_x, input_mesh_y, input_mesh_z, ...
        input_image, ...
        output_mesh_x, output_mesh_y, output_mesh_z, ...
        lower(interpmethod), false);
    else % use matlab's built in
      resized = interp3( ...
        input_mesh_x, input_mesh_y, input_mesh_z, ...
        input_image, ...
        output_mesh_x, output_mesh_y, output_mesh_z, ...
        interpmethod);
    end % use_ba
    
  end % 2D or 3D
  
  % get the output
  resized = cast(resized, old_class);
  if display, fprintf('done.\n'); end
      
end % resizeimage

% this is just a helper function to calculate the number of output
% elements (either rows, cols, or slices)
function [output_elements, output_max_value, input_max_value] = GetOutputElements( ...
  no_output_elements, no_output_voxel_size, ...
  output_voxel_size, input_voxel_size, ...
  output_elements, input_elements)

  input_max_value = (input_elements - 1) * input_voxel_size;

  % what info do we have about the output
  if no_output_elements
    if no_output_voxel_size
      % we have no information
      % so just use the input
      output_elements = input_elements;
      output_max_value = input_max_value;
    else
      % we know the output voxel size only
      output_elements = 1 + floor(input_max_value / output_voxel_size);
      output_max_value = output_voxel_size * (output_elements - 1);
    end
  else
    if no_output_voxel_size
      % we know just the output # of elements
      output_max_value = input_max_value;
    else
      % we know both the output elements and output voxel size.
      output_max_value = (output_elements - 1) * output_voxel_size;
    end
  end

end % GetOutputElements