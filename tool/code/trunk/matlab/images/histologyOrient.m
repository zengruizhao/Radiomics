close all;
number_of_iterations = 100;  

name='C:\Users\Rachel\Documents\MATLAB\DATA\WholeMountHistology\BMC_PR_0012\BMCP012US B2_cropped.jpg';

% rescale intensities
io=imread(name);
io=rescale(double(io));
iog=rgb2gray(io);
    
% threshold value
gthresh=graythresh(imresize(iog,[100 NaN]));
m=(iog<gthresh);

m=bwmorph(m,'dilate',5);    
m=removeAllButBiggestFromMask(m);

% get mask
mask=region_seg(iog,m,number_of_iterations,.2,false);
mask=removeAllButBiggestFromMask(mask);

%determine edges

B = bwboundaries(m);
bounds.r= B{1}(:,1)'- mean(B{1}(:,1));
bounds.c= B{1}(:,2)'- mean(B{1}(:,2));

[major_axis, minor_axis]= fitEllipseToBoundary(bounds);

%show contour
figure,imshow(io)
hold on
contour(mask, [ 0 0], 'g', 'LineWidth',4)


centered_r = ((bounds.r-min(bounds.r)))+1;
centered_c = ((bounds.c-min(bounds.c)))+1;
plot( [0  range(centered_r)*major_axis(2)]+mean(centered_r), [0  range(centered_c)*major_axis(1)]+mean(centered_c),'-b','LineWidth',4);

angle = 180/pi*( tan(major_axis(1)/major_axis(2) ) );

newIO = imrotate((io).*repmat(mask,[1 1 3]),-angle);
figure,imshow(newIO)