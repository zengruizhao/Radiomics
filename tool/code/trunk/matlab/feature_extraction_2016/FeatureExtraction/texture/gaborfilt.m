function [gabors_cos,gabors_sin] = gaborfilt(vol,theta,lambda,b,psi,gamma)

% Gabors (2 * numel(lambda) * numel(theta) features)
% lambda=[1 2];
% theta=(0:5)*pi/6;
% b=1; psi=0; gamma=1;
if nargin<6, gamma=1; if nargin<5, psi=0; end; end

ngabors=numel(lambda)*numel(theta);
gabors_cos=zeros([size(vol) ngabors]);
gabors_sin=zeros([size(vol) ngabors]);

ig=1;
fprintf('Calculating Gabor features:');
for i=1:numel(lambda),
    for j=1:numel(theta),
        [gb_c,gb_s]=gaborkerns(theta(j),lambda(i),b,psi,gamma);
        for k=1:size(vol,3),
            gabors_cos(:,:,k,ig)=jconv2(vol(:,:,k),gb_c);
            gabors_sin(:,:,k,ig)=jconv2(vol(:,:,k),gb_s);
        end
        ig=ig+1;
        fprintf('.');
    end
end
fprintf('\n');


function cI = jconv2(I,G);
% conv2(I,G,'same') in fourier domain
[rows,cols]=size(I);
[grows,gcols] = size(G);
fI = fft2(I,rows+grows-1,cols+gcols-1);
fG = fft2(G,rows+grows-1,cols+gcols-1);
cI = abs(ifft2(fI.*fG));
cI = cI(1+floor(grows/2):rows+floor(grows/2),...
    1+floor(gcols/2):cols+floor(gcols/2));