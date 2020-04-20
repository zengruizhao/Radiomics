function [volfeats,featcount] = textures3(vol,bg)

if ~isfloat(vol), error('Volume must be float.'); end
if nargin<2, bg=-1; end

nintensity=1;
ngrays=4;
ngrads=13;
nharalicks=13;
% ngabors=12;
% nlaws=10; % 3D subset
% nlaws=25; % 2D
nlaws=15; % 2D unique
% nlaws=125; % 3D
% nlbps=1;
nlbps=2; % 2 for the log lbp image
% nfeats=ngrays*2 + ngrads + nharalicks*3 + ngabors*2;
nfeats=nintensity+ngrays*2 + ngrads + nharalicks*3 + nlaws + nlbps; % no Gabor
volfeats=zeros([size(vol) nfeats],class(vol));

addedfeats=0;
volfeats(:,:,:,1:nintensity) = vol;
addedfeats=addedfeats+nintensity;

% 4 gray level (first order statistical) feautures
volfeats(:,:,:,addedfeats+(1:ngrays)) = grayfilts3(vol,5);
addedfeats=addedfeats+ngrays;
volfeats(:,:,:,addedfeats+(1:ngrays)) = grayfilts3(vol,7);
addedfeats=addedfeats+ngrays;

% 13 gradient features
volfeats(:,:,:,addedfeats+(1:ngrads)) = gradfilts3(vol);
addedfeats=addedfeats+ngrads;

% 13 Haralick features
fprintf('Calculating Haralick features:\n');

if isa(vol,'double'), haralickfun=@haralick3mexmt;
else haralickfun=@haralick3smexmt; end

ws=5; hardist=1; harN=64;
volN=round(rescale_range(vol,0,harN-1));
volfeats(:,:,:,addedfeats+(1:nharalicks))=haralickfun(volN,harN,ws,hardist,bg);
addedfeats=addedfeats+nharalicks;

ws=7; hardist=1; harN=64;
volN=round(rescale_range(vol,0,harN-1));
volfeats(:,:,:,addedfeats+(1:nharalicks))=haralickfun(volN,harN,ws,hardist,bg);
addedfeats=addedfeats+nharalicks;

ws=7; hardist=1; harN=128;
volN=round(rescale_range(vol,0,harN-1));
volfeats(:,:,:,addedfeats+(1:nharalicks))=haralickfun(volN,harN,ws,hardist,bg);
addedfeats=addedfeats+nharalicks;

% 25 2D or 125 3D Laws features
% 2D
fprintf('Calculating Laws features:\n');

% remove local mean first
meanwindow=[15 15 5];
meanmask = repmat(1/prod(meanwindow),meanwindow);
vol0 = convn(vol,meanmask,'same');
vol0 = vol-vol0;

alllaws=zeros([size(vol) 25],class(vol));
for j=1:size(vol,3),
    alllaws(:,:,j,:)=lawsfilter(vol0(:,:,j)); % operate on image with local mean removed
end
% get the 15 unique ones
for j=1:5,
    for i=j:5,
        addedfeats=addedfeats+1;
        volfeats(:,:,:,addedfeats) = alllaws(:,:,:,i+(j-1)*5);
        if i~=j,
            % average of symmetric pairs
%             volfeats(:,:,:,addedfeats) = volfeats(:,:,:,addedfeats) + ...
%                 alllaws(:,:,:,j+(i-1)*5);
%             volfeats(:,:,:,addedfeats) = volfeats(:,:,:,addedfeats)/2;
            % magnitude symmetric pairs
            volfeats(:,:,:,addedfeats) = sqrt(volfeats(:,:,:,addedfeats).^2  +alllaws(:,:,:,j+(i-1)*5).^2);
        end
    end
end
% volfeats(:,:,:,addedfeats+(1:nlaws))=alllaws; % if nlaws=25;
% 3D
% tic
% volfeats(:,:,:,addedfeats+(1:nlaws)) = lawsfilter3(vol);
% volfeats(:,:,:,addedfeats+(1:nlaws)) = lawsfilter3(vol,true);
% toc
% addedfeats=addedfeats+nlaws;

fprintf('Calculating LBP feature:\n');
for j=1:size(vol,3),
    volfeats(:,:,j,addedfeats+1) = lbp(vol(:,:,j));
    volfeats(:,:,j,addedfeats+2) = log(volfeats(:,:,j,addedfeats+1)+1);
end
featcount=addedfeats+nlbps;
if featcount~=nfeats, warning('the expected and actual feature counts do not match.'); end

return

% Gabors (2 * numel(lambda) * numel(theta) features)
fprintf('Calculating Gabor features:\n');
% lambda=[1 2];
lambda=pi./[4,4*sqrt(2),8];
theta=(0:5)*pi/6;
b=0.3;

% gabors_cos=zeros([size(vol) numel(lambda)*numel(theta)]);
% gabors_sin=zeros([size(vol) numel(lambda)*numel(theta)]);

psi=0; gamma=1;
ig=1;
for i=1:numel(lambda),
    for j=1:numel(theta),
        [gb_c,gb_s]=gaborkerns(theta(j),lambda(i),b,psi,gamma);
        for k=1:size(vol,3),
            % gabors_cos(:,:,k,ig)=jconv2(vol(:,:,k),gb_c);
            % gabors_sin(:,:,k,ig)=jconv2(vol(:,:,k),gb_s);
            volfeats(:,:,k,addedfeats+ig)=jconv2(vol(:,:,k),gb_c);
            volfeats(:,:,k,addedfeats+ig+ngabors)=jconv2(vol(:,:,k),gb_s);
        end
        ig=ig+1;
        fprintf('.');
    end
end
fprintf('\n');

% volfeats(:,:,:,ngrays+ngrads+nharalicks+(1:ngabors/2))=gabors_cos;
% volfeats(:,:,:,ngrays+ngrads+nharalicks+ngabors/2+(1:ngabors/2))=gabors_sin;

% addedfeats=addedfeats+ngabors*2;
