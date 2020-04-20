function KK = lawskerns
% LAWSKERNS Generate the 25 Law's 2D kernels from the 5 1D kernel bases.
%
%   KK = LAWSKERNS returns the kernenels in a 5-by-5-by-25 array KK.
%
% L5 = L3*L3
% E5 = L3*E3 = E3*L3
% S5 = L3*S3 = E3*E3
% W5 = E3*S3 = S3*E3
% R5 = S3*L3 = S3*S3
% use mtimesx for 3D (e.g. E3E3E3, E3S3S3, L3S3E3, etc.):
%   L3L3E3=mtimesx(L3',mtimesx(permute(L3,[3 1 2]),E3))  % 3x1x1 * 1x1x3 * 1x3x1
%   % OR
%   L3L3E3=mtimesx(permute(mtimesx(L3',L3),[1 3 2]),E3)  % (3x1x1 * 1x3x1)^Transpose(2<->3) * 1x3x1
%JCC

K=zeros(5);
K(1,:) = [ 1  4  6  4  1]; % L (Level)
K(2,:) = [-1 -2  0  2  1]; % E (Edge)
K(3,:) = [-1  0  2  0 -1]; % S (Spot)
K(4,:) = [-1  2  0 -2  1]; % W (Wave)
K(5,:) = [ 1 -4  6 -4  1]; % R (Ripple)

KK=zeros(5,5,5*5);
for i=1:5,
    for j=1:5,
        KK(:,:,j+(i-1)*5)=K(i,:)'*K(j,:);
    end
end
