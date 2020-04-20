function [volume] =  CylindricalToCartesian( directory, eyeFileName, isLogCompressed )


% parse all eye files belonging to a given study into a volume
files = dir([directory, '/',eyeFileName,'*.eye']);
for i = 1: length( files )
    if (i == 1)
      volume = ReadEYEImage( [directory, '/',files(i).name], isLogCompressed );
    else
      imageSlice = ReadEYEImage( [directory, '/',files(i).name], isLogCompressed  );
      
      if ( size(imageSlice,1) ~=size(volume,1) ||  size(imageSlice,2) ~=size(volume,2) )
          disp([directory, '/',eyeFileName,' contains images of different sizes.'] );
          return;
      end
      volume = cat(3, volume, imageSlice );
    end
    
end

% what is the spacing/origin information -- these files were given to me by
% RRI
imageParameters = load( [directory, '/',eyeFileName,'_geometry.mat']);

% pixel spacing
r_step     =  imageParameters.Geometry.DepthEnd.value* 1e3./size(volume,1) ;
theta_step = ( imageParameters.Geometry.Width.value )./ size(volume,2);
beta_step  = (imageParameters.Geometry.ElevationWidth.value)/ 56;

% study origin
r_start = 0;
theta_start =  imageParameters.Geometry.VectorAngles.value(1);
beta_start = (pi/2)-(imageParameters.Geometry.ElevationWidth.value)/2; 

% write to a temporary file
try
    mha_write_volume( './temp.mha', volume, [ r_step theta_step beta_step], [r_start theta_start beta_start ] );
catch 
    disp('Unable to write image to ./temp.mha');
    return;
end

% run the executable
system( 'TestLoadEye.exe "./temp.mha" isLogCompressed "./" 0');

% load in the result of the exectuable
volume = mha_read_volume( './volume.mha');
end