clear; close all;

img = imread('G:\MATLAB\data\ibris_data\highres_data\original_images\8913_500_f00003_original.tif');

[A,~,~] = colour_deconvolution(img,'H&E');

A = intlut(A,uint8(255:-1:0));
A = imfilter(A,fspecial('gaussian',[10 10],3));
% A = im2bw(A,graythresh(A));
B = im2bw(A,0.5);

centroids = regionprops(B,'Centroid');
c = [];
for ii = 1:length(centroids)
    c = [c;centroids(ii).Centroid];
end

[edges, params] = ccg_construct(c,0.5,0.17);
% a: 0.15 or 0.17 (more sparse)

[x,y] = find(edges);
hosoya_return = hosoya_c([x,y]);

