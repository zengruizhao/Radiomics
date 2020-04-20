binmap = imread('temp3.tif');

fprintf('MATLAB Code: ');
tic; [N W pos]=callaghan(binmap,2,120,.5); toc;
fprintf('MEX Code: ');
tic; [N2 W2]=callaghan2(binmap,2,120,.5); toc;