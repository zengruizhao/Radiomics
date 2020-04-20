function dicomdirdataordered = dicomdir_sort(dicomdirdata)

dicomseriesnums=[dicomdirdata(:).SeriesNumber];
[dicomseriesnums,di,dj]=unique(dicomseriesnums);

nseries=numel(di);

for i=1:nseries,
    
    seriesi_inds=find(dj==i);
    nseriesimages=numel(seriesi_inds);
    
    dicomdirdataordered.series(i).SeriesNumber=dicomseriesnums(i);
    dicomdirdataordered.series(i).SeriesDescription=dicomdirdata(seriesi_inds(1)).SeriesDescription;
    
    series_instance_nums=zeros(nseriesimages,1);
    for j=1:nseriesimages,
        instancenum=double(dicomdirdata(seriesi_inds(j)).InstanceNumber);
        if isempty(instancenum), instancenum=NaN; end
        series_instance_nums(j)=instancenum;
    end
    [~,seriesimagesorder]=sort(series_instance_nums);
    
    for j=1:nseriesimages,
        dicomdirdataordered.series(i).image(j)=dicomdirdata(seriesi_inds(seriesimagesorder(j)));
    end
end
