function viewharalicks(volname,grays,win)
close all;

xdim = 256;ydim=256;nfeat=13;

names = {'contrast energy','contrast inverse moment', ...
    'contrast ave','contrast var','contrast entropy', ... 
    'intensity ave','intensity variance','intensity entropy', ...
    'entropy,','energy', ...
    'correlation', ...
    'information measure1','information measure2'};

v = volopen(['Z:\FinalImageSets\',vols{i},'_ax_t2_m-stn'],[xdim ydim],'single');
zdim = size(v,3); 
clear v;
	
Hf = volopen(['Z:\InvivoFeatures\',volname,'\HaralickFeat_',num2str(win),'_',num2str(grays)],[xdim ydim nfeat zdim],'single');
       
for i=1:zdim
    for f=1:nfeat
        dispimg(squeeze(Hf(:,:,f,i)));title(['Slice ',num2str(i),',',names{f},', ',num2str(grays),' graylevels, window = ',num2str(win)]);
        pause;close all;
    end
end
    
end
