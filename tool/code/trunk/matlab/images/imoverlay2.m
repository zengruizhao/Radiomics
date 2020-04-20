function overlayim = imoverlay2(background,overlay,overlayweight)
% IMOVERLAY Generate an image that is an overlay of two images.
%   IMOVERLAY(BACKGROUND,OVERLAY) returns an image that is an overlay of
%   BACKGROUND and OVERLAY, two images of the same size.
%
%   IMOVERLAY(BACKGROUND,OVERLAY,OVERLAYWEIGHT) generates the overlay using
%   the weight OVERLAYWEIGHT, by BACKGROUND + OVERLAYWEIGHT*OVERLAY. The
%   default value of OVERLAYWEIGHT is 0.5.
%
%   Notes
%   -----
%   Both images are converted to double precision.
%
%   The values are not rescaled.  Make sure the images have comparable
%   intensities.
%
%   If one image is color (RGB) and the other is grayscale, the output will
%   be color.
%
%JC

% Set default overlay weight
if nargin<3,
    overlayweight=0.5;
end

% Ensure in-plane dimensions are the same
if any([size(background,1) size(background,2)]~=[size(overlay,1) size(overlay,2)]);
    error('Images are of different size.');
end

% If one images is color, convert the other to color too
if ndims(background)~=ndims(overlay)
    if size(background,3)>size(overlay,3),
        overlay=repmat(overlay,[1 1 size(background,3)]);
    else
        background=repmat(background,[1 1 size(overlay,3)]);
    end
end

if overlayweight>1, error('weight is between 0 and 1'); end

% overlayim=rescale(rescale(background)+overlayweight*rescale(overlay));
overlayim=(1-overlayweight)*double(background)+overlayweight*double(overlay);
