function [Bx,By]=ksplat2(Fx,Fy,H,expandfactor)
% KSPLAT2
% 

if nargin<4, expandfactor=2; end

Fsize=size(Fx);

if nargin<3 || isempty(H),
    % Gaussian kernel (Must be symetric and odd in size)
    % basews=7; basesigma=5/6;
    basews=5; basesigma=3/4;
    ws=round(basews*expandfactor/2);
    if ~mod(ws,2), ws=ws+1; end
    H=fspecial('gaussian',[ws ws],basesigma*(expandfactor/2));
end

if expandfactor>1,
    Fx=expandfactor*imresize(Fx,expandfactor,'bilinear');
    Fy=expandfactor*imresize(Fy,expandfactor,'bilinear');
end
if ~isa(Fx,'double') || ~isa(Fy,'double')
    Fx=double(Fx);
    Fy=double(Fy);
end

[Bx,By]=ksplat2mex(Fx,Fy,H);
% [Bx,By]=ksplat2mex_old(Fx,Fy,H);

if expandfactor>1,
    Bx=(1/expandfactor)*imresize(Bx,Fsize,'bilinear');
    By=(1/expandfactor)*imresize(By,Fsize,'bilinear');
end
