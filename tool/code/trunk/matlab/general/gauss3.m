function H = gauss3(sigma,ws)

if numel(sigma)==1, sigma=sigma([1 1 1]); end
if numel(sigma)~=3, error('Sigma must be either a scalar or a 3 element vector.'); end

if nargin<2, ws=ceil(6*sigma); end

% Gaussian Window
% sigma=sqrt(nrows/2)/4; % cut-off freq. = fs/sigma = fs*4*sqrt(2)/sqrt(nrows)
% sigmasqrd=nrows/32;
% twosigmasqrd=nrows/16;
% twosigmasqrdinv=16/nrows;
twosigmasqrdinv=1./(2*sigma.^2);
y=1:ws(1); y0=(ws(1)+1)/2;
x=1:ws(2); x0=(ws(2)+1)/2;
z=1:ws(3); z0=(ws(3)+1)/2;
[X,Y,Z]=meshgrid(x,y,z);
H=exp(-((twosigmasqrdinv(1)*(Y-y0).^2) + ...
    (twosigmasqrdinv(2)*(X-x0).^2) + ...
    (twosigmasqrdinv(3)*(Z-z0).^2)));
H(H<eps*max(H(:))) = 0;
Hs = sum(H(:));
if Hs, H  = H/Hs; end
