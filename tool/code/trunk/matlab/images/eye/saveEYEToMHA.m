function [isSaved] =  saveEYEToMHA( directory, eyeFileName, volumeName )

isSaved = false;

files = dir([directory, '/',eyeFileName,'*.eye']);
for i = 1: length( files )
    if (i == 1)
        volume = ReadEYEImage( [directory, '/',files(i).name],1 );
    else
      imageSlice = ReadEYEImage( [directory, '/',files(i).name] ,1);
      
      if ( size(imageSlice,1) ~=size(volume,1) ||  size(imageSlice,2) ~=size(volume,2) )
          disp([directory, '/',eyeFileName,' contains images of different sizes.'] );
          isSaved = false;
          fclose all;
          return;
      end
      volume = cat(3, volume, imageSlice );
    end
    
  fclose all;
end

% what is the spacing/origin information
imageParameters = load( [directory, '/',eyeFileName,'_geometry.mat']);

% step values
r_step     =  imageParameters.Geometry.DepthEnd.value* 1e3./size(volume,1) ;
theta_step = ( imageParameters.Geometry.Width.value )./ size(volume,2);
beta_step  = (imageParameters.Geometry.ElevationWidth.value)/ 56;

% origin values 
r_start = 0;
theta_start =  imageParameters.Geometry.VectorAngles.value(1);
beta_start = (pi/2)-(imageParameters.Geometry.ElevationWidth.value)/2; 

try
    mha_write_volume( volumeName, volume, [ r_step theta_step beta_step], [r_start theta_start beta_start ] );
    isSaved = true;
catch 
    isSaved = false;
end
    
fclose all;
end