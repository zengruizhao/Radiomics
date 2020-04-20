% addSlicesToMask uses shape based interpolation to interpolate slices into
% a mask
%
%Usage:
% addSlicesToMask(mask, whichSlices, options)
%
% mask is a 2D or 3D binary image.
%
% options is a struct contains the following optional fields:
%  input_voxel_size
%  output_voxel_size
%  interpmethod
%  distance_map
%
%
% Created by Rachel Sparks, 2013-8-23
% 

function [ newMask ] = addSlicesToMask( mask, whichSlicesToAdd, options )

  % set up parser
  ip=inputParser;
  ip.StructExpand  = true;
  ip.KeepUnmatched = true;

  % set default parameters    
  distmethods = {'chamfer','euclidean','cityblock','chessboard','quasi-euclidean'};
  ip.addParamValue('distance_map','euclidean',@(x)any(strcmpi(x,distmethods)));
    
  ip.addParamValue('input_voxel_size', [1, 1, 1], ...
    @(x) isnumeric(x) && isvector(x) && (~any(isnan(x))) && (~any(x == 0)) ...
    && (length(x) == 3 || length(x) == 2));
  ip.addParamValue('output_voxel_size', ...
    @(x) isnumeric(x) && isvector(x) && (~any(x == 0)) ...
    && (length(x) == 3 || length(x) == 2));

  scalar_attributes = {'scalar', 'positive', 'real'};
  ip.addParamValue('output_rows', size(mask,1),...
    @(x) validateattributes(x, {'numeric'}, scalar_attributes));
  ip.addParamValue('output_cols', size(mask,2), ...
    @(x) validateattributes(x, {'numeric'}, scalar_attributes));
  ip.addParamValue('output_slices', size(mask,3), ...
    @(x) validateattributes(x, {'numeric'}, scalar_attributes)); 

  %Get the parameters
  ip.parse(options);
  defaults = ip.UsingDefaults;  
  dist_type             = ip.Results.distance_map;
  input_voxel_size      = ip.Results.input_voxel_size;
  output_voxel_size     = ip.Results.output_voxel_size;
  no_output_rows        = ip.Results.output_rows;
  no_output_cols        = ip.Results.output_cols;
  no_output_slices      = ip.Results.output_slices;

  % determine the input vectors
  inputSpacingX = (1:size(mask,1))* input_voxel_size(1);
  inputSpacingY = (1:size(mask,2))* input_voxel_size(2);
    
  % for the z slices first determine those slices to exclude in the input
  original_z_slices = 1:size(mask,3);
  mask_z_slices = setdiff( original_z_slices, whichSlicesToAdd );
  inputSpacingZ = mask_z_slices*input_voxel_size(3); % add scaling
    
  % determine the output vectors
  outputSpacingX = (1:no_output_rows)* output_voxel_size(1);
  outputSpacingY = (1:no_output_cols)* output_voxel_size(2);
  outputSpacingZ = (1:no_output_slices)* output_voxel_size(3);
    
    % create mask without ignored slices
    mask_without_gaps = mask(:,:, mask_z_slices );

    %get the distance map
    if strcmp(dist_type,'chamfer')
        dist_map=chamfer_map(logical(mask_without_gaps));    
    else
        dist_map=mask2distances(logical(mask_without_gaps), options);
    end    
    
    %interpolate the distance map and convert it to a mask
    [newMask] = resizeimageUnequalSpacing(dist_map,...
        inputSpacingX, inputSpacingY, inputSpacingZ, ...
        outputSpacingX, outputSpacingY, outputSpacingZ, options); 
    newMask = newMask < 0;
end