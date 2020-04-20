function [Ipic] = impicasso(I,npx,npy,picasoness,fillwith)

if nargin<4,
    picasoness=.1;
end

if nargin<5,
    fillwith=cast(0,class(I));
end

[Px,Py,spyx]=ffdmesh(npx,npy,[size(I,1) size(I,2)]);

xgriddispli=randn(size(Px)-4)*(spyx(2)*picasoness);
ygriddispli=randn(size(Px)-4)*(spyx(1)*picasoness);

% x and y components of padded displacement matrix
xgriddispl=zeros(size(Px)); ygriddispl=xgriddispl;
xgriddispl(3:end-2,3:end-2)=xgriddispli;
ygriddispl(3:end-2,3:end-2)=ygriddispli;

Pxdispl=Px+xgriddispl;
Pydispl=Py+ygriddispl;

Ipic=FFDimmesh(I,Px,Py,Pxdispl,Pydispl,[],[],fillwith);
% Ipic_testing=FFDimmesh_testing(I,Px,Py,Pxdispl,Pydispl,[],[],fillwith);
