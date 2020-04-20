function rescalehistology(img,scale)

figure('units', 'pixels', 'Position',[0 0 size(img,2).*scale size(img,1).*scale]);
axes('units','normalized','position',[0 0 1 1]);
imagesc(img); axis ij; axis off; hold on; axis equal;
set(gcf,'Color',[1 1 1]);
