function saveAllEYEToMHA( pathname )

folders = dir( pathname );

for i=21%9:length(folders) % ignore the first two files which will be thumbnails
    % do we have the goemetric data
    geometryFiles = dir([folders(i).name,'/US_EYE/*geometry.mat']); 
    
    for j=1:length( geometryFiles )
      %do we have 56 eye files
      eyeFiles = dir([folders(i).name, '/US_EYE/',geometryFiles(j).name(1:22),'*.eye']);
      if ( length(eyeFiles) == 56 )
          if( ~isdir([pathname,'/Preprocessed/',folders(i).name]) )
              mkdir([pathname,'/Preprocessed/',folders(i).name]);
          end
        isSaved = saveEYEToMHA([folders(i).name, '/US_EYE/'],geometryFiles(j).name(1:22),...
            [pathname,'/Preprocessed/A-Mode',folders(i).name,'/',geometryFiles(j).name(1:22),'_volume.mha']);
        
        if (~isSaved)
            disp(['Unable to save ', folders(i).name, '/US_EYE/',geometryFiles(j).name(1:22)] )
        end
      end
      
    end
end % for i

end