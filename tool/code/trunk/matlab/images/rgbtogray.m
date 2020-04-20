function I = rgbtogray(Irgb)
% RGBTOGRAY Convert RGB image to grayscale.
%   RGBTOGRAY(IRGB) returns a grayscale image representation of IRGB.
%
%   This for people without image processing toolbox.
%
%JC

if ~isa(Irgb,'double'),
    Irgb=double(Irgb);
end

T = inv([1.0 0.956 0.621; 1.0 -0.272 -0.647; 1.0 -1.106 1.703]);
weights=T(1,:)';

imsize=size(Irgb);

Irgb=reshape(Irgb(:),imsize(1)*imsize(2),3);

I=Irgb*weights;

I = min(max(I,0),1);

I = reshape(I,[imsize(1), imsize(2)]);
