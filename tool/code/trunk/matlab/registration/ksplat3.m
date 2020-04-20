function [Bx,By,Bz,H]=ksplat3(Fx,Fy,Fz,H,expandfactor)
% KSPLAT3 Convert forward to reverse deformation field by kernel splatting.
% 
%   [BX,BY,BZ]=KSPLAT3(FX,FY,FZ,H) converts forward deformation field
%   specified by components FX, FY and FZ into a reverse deformation field
%   specified by components BX, BY and BZ, using the 3D kernel H.
%
%   A "forward" deformation field refers to displacements of each pixel on 
%   an undeformed image, hence specifying fractional coordinates of the 
%   *destination* of each pixel.  A "reverse" deformation field refers to 
%   displacements of each pixel on a deformed image, hence specifying 
%   fractional coordinates of the *origin* of each pixel.
%
%   This function uses a MEX file, ksplat3mex.  You may need to compile it.
%
%   See also: forwardisp3.m.
%
%JC

if nargin<4, expandfactor=2; end
if expandfactor<1, error('expand factor must be >=1'); end

Fsize=size(Fx);

if nargin<4 || isempty(H),
    % Gaussian kernel (Must be symetric and odd in size)
    basews=[5 5 3];
    basesigma=[3/4 3/4 1/3];
    sigma=basesigma*(expandfactor/2);
    ws=round(basews*expandfactor/2);
    if any(~mod(ws,2)), ws(~mod(ws,2))=ws(~mod(ws,2))+1; end
    
    H = gauss3(sigma,ws);

end

if expandfactor>1,
    Fx=expandfactor*imresize(Fx,expandfactor,'bilinear');
    Fy=expandfactor*imresize(Fy,expandfactor,'bilinear');
    Fz=expandfactor*imresize(Fz,expandfactor,'bilinear');
end

if ~isa(Fx,'double') || ~isa(Fy,'double') || ~isa(Fz,'double'),
    Fx=double(Fx);
    Fy=double(Fy);
    Fz=double(Fz);
end

if exist('ksplat3mexmt','file'),
    [Bx,By,Bz]=ksplat3mexmt(Fx,Fy,Fz,H);
else
    [Bx,By,Bz]=ksplat3mex(Fx,Fy,Fz,H);
end


if expandfactor>1,
    Bx=(1/expandfactor)*imresize(Bx,Fsize,'bilinear');
    By=(1/expandfactor)*imresize(By,Fsize,'bilinear');
    Bz=(1/expandfactor)*imresize(Bz,Fsize,'bilinear');
end
