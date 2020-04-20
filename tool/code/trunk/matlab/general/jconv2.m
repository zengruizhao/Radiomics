function cI = jconv2(I,G)
% JCONV2 2D convolution in Fourier domain.
%   CI = JCONV2(I,G) convolves I with kernel G and returns CI. The
%   dimensions of CI are the same as I. The array G can be of equal or
%   smaller size than I.
%
%   This function is the same as conv2(I,G,'same'), but computed in the
%   Fourier domain. The advantages are speed and robustness to edge
%   effects.
%
%JC

[rows,cols]=size(I);
[grows,gcols] = size(G);

% FFT size must be large to avoid anti-aliasing
fI = fft2(I,rows+grows-1,cols+gcols-1);
fG = fft2(G,rows+grows-1,cols+gcols-1);

% Multiply in z space and return to cartesian space
cI=fI.*fG;
clear fI fG
cI=ifft2(cI);
cI=abs(cI);
% cI = abs(ifft2(fI.*fG));
% Crop off padding
cI = cI(1+floor(grows/2):rows+floor(grows/2),...
    1+floor(gcols/2):cols+floor(gcols/2));
