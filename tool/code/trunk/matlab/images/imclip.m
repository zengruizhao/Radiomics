function imclip(I)
image(I,'CDataMapping','scaled')
colormap gray
axis equal
axis off
set(gca,'CLim',[0 1])