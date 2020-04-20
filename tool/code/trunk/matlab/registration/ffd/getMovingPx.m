function [minds,mPx,mPy,mPz] = getMovingPx(iPt,jPt,kPt,knotsx,knotsy,knotsz,Isize)

mPx_bounds = [Isize(2) 1];
mPy_bounds = [Isize(1) 1];
mPz_bounds = [Isize(3) 1];

tol = 1e-9;

for ii=1:numel(iPt),
    mPx_ii = [ceil(max(1+1,knotsx(jPt(ii)-2))) floor(min(knotsx(jPt(ii)+2)-tol,Isize(2)-1))];
    mPy_ii = [ceil(max(1+1,knotsy(iPt(ii)-2))) floor(min(knotsy(iPt(ii)+2)-tol,Isize(1)-1))];
    mPz_ii = [ceil(max(1+1,knotsz(kPt(ii)-2))) floor(min(knotsz(kPt(ii)+2)-tol,Isize(3)-1))];
    % mPx = round(max(1,knotsx(jPt-2)):min(knotsx(jPt+2),Isize(2)));
    % mPy = round(max(1,knotsy(iPt-2)):min(knotsy(iPt+2),Isize(1)));
    % mPz = round(max(1,knotsz(kPt-2)):min(knotsz(kPt+2),Isize(3)));
    
    mPx_bounds = [min(mPx_ii(1),mPx_bounds(1)) max(mPx_ii(2),mPx_bounds(2))];
    mPy_bounds = [min(mPy_ii(1),mPy_bounds(1)) max(mPy_ii(2),mPy_bounds(2))];
    mPz_bounds = [min(mPz_ii(1),mPz_bounds(1)) max(mPz_ii(2),mPz_bounds(2))];
end

% mPx = uint32(mPx_bounds(1)):uint32(mPx_bounds(2));
% mPy = uint32(mPy_bounds(1)):uint32(mPy_bounds(2));
% mPz = uint32(mPz_bounds(1)):uint32(mPz_bounds(2));
mPx = mPx_bounds(1):mPx_bounds(2);
mPy = mPy_bounds(1):mPy_bounds(2);
mPz = mPz_bounds(1):mPz_bounds(2);

% mbox = false(Isize);
% mbox(mPy,mPx,mPz)=true;
% minds=find(mbox);

% [Px,Py,Pz]=meshgrid(mPx,mPy,mPz);
% % minds = sub2ind(Isize,Py,Px,Pz);
% kB = cumprod(Isize);
% % minds = Py + (Px-1)*kB(1) + (Pz-1)*kB(2);
% minds = Py + Px*kB(1) + Pz*kB(2) - (kB(1) + kB(2));
% minds = minds(:);

minds = subbox2ind3mex(Isize, mPy, mPx, mPz);
