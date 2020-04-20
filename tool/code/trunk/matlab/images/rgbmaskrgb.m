function Irgb=rgbmaskrgb(Irgb,mask,colorspec)
%RGBMASKRGB
%   IMASKED=rgbmaskrgb(I,MASK,COLORSPEC)
%
%   Mask NxM gray scale image I with the NxM logical, MASK. Output image,
%   IMASKED, with the *exact* color specified by COLORSPEC. COLORSPEC is a
%   3 element array indicating R, G and B intensities. For example, [1 0 0]
%   specified red, [0 1 0] specified green, and [1 1 0] specifies yellow.
%   COLORSPEC reflects the scale of the data (if using uint8 rather than
%   double, use [255 0 0] for example).
%
%   Input may also be a color image of size NxMx3. Grayscale input is
%   converted to color.
%
%   Output, Imasked, is an NxMx3 RGB image.
%
%   Example, make a yellow circular ROI on an image:
%
%       Ig=mean(imread('ngc6543a.jpg'),3);
%       [X,Y]=meshgrid(1:size(Ig,2),1:size(Ig,1)); pts=[X(:) Y(:)];
%       ptmat=repmat([150 200],size(pts,1),1);
%       distfromcenter=sqrt(sum((pts-ptmat).^2,2));
%       distimage=reshape(distfromcenter,size(Ig));
%       Imasked=rgbmaskrgb(Ig,distimage<10,[255 255 0]);
%       imagesc(uint8(Imasked))
%
%JC

% check if input is color, if not make color
if size(Irgb,3)==1,
    Irgb=repmat(Irgb,[1 1 3]);
end

if ~islogical(mask), error('Mask must be logical.'); end

Ic=Irgb(:,:,1);
Ic(mask)=colorspec(1);
Irgb(:,:,1)=Ic;

Ic=Irgb(:,:,2);
Ic(mask)=colorspec(2);
Irgb(:,:,2)=Ic;

Ic=Irgb(:,:,3);
Ic(mask)=colorspec(3);
Irgb(:,:,3)=Ic;