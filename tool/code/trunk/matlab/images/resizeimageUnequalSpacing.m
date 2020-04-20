% Usage:
%   [resized, output_voxel_size] = resizeimage(input_image, inputSpacingX, inputSpacingY, inputSpacingZ, outputSpacingX, ouputSpacingY, outputSpacingZ, options)
% 
% input_image is a 2D or 3D image.
% 
% inputSpacingX, inputSpacingY, inputSpacingZ are 1xN vectors describing
% the input spacing for the image.
%
% outputSpacingX, outputSpacingX, outputSpacingX are 1xN vectors describing
% the ouput spacing for the image.
%
% options is a struct which can contain the following fields:
%
% interpmethod: which interpolator to use
%
% If you pass in a 2D image, all z information is ignored.
% 
% Created by Rachel Sparks, 2013-8-23
% 
function [volume] = resizeimageUnequalSpacing(input_image, ...
                     inputSpacingX, inputSpacingY, inputSpacingZ,...
                     outputSpacingX, outputSpacingY, outputSpacingZ, options)
          
parser = inputParser;     
parser.StructExpand  = true;
parser.KeepUnmatched = true;

interpmethods={'Linear','Cubic','Nearest'};
parser.addParamValue('interpmethod', 'Linear', ...
    @(x) any(strcmpi(x,interpmethods)) );

parser.parse(options);
input_results     = parser.Results;
interpmethod      = input_results.interpmethod;
  
  [input_mesh_x, input_mesh_y, input_mesh_z] = meshgrid( ...
    inputSpacingY, ...    
    inputSpacingX, ...    
    inputSpacingZ );
  
  % where are the output voxels?
  [output_mesh_x, output_mesh_y, output_mesh_z] = meshgrid( ...
    outputSpacingY, ...    
    outputSpacingX, ...    
    outputSpacingZ );

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
    % we must use the matlab implementation
    % the mex file ba_interp3 assumes equal spacing
      resized = interp3( ...
        input_mesh_x, input_mesh_y, input_mesh_z, ...
        input_image, ...
        output_mesh_x, output_mesh_y, output_mesh_z, ...
        interpmethod);
    
  end % 2D or 3D
  
  % get the output
  volume = cast(resized, old_class);

end
