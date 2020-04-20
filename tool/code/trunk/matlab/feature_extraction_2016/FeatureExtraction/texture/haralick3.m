function [features,feature_names] = haralick3(img3,graylevels,sig,dist,is_valid3,feature_names,comtx_type,varargin)
% [FEATURES,FEATURE_NAMES] = HARALICK3(IMG3,GRAYLEVELS,SIG,DIST,IS_VALID,feature_names)
%   HARALICK computes the Haralick features for the 3-D UINT16 image IMG3.
%   The dynamic range of the image is given by the scalar GRAYLEVELS.  
%   All values of IMG3 must lie in the interval [0:GRAYLEVELS-1]. The 
%   half window size HWS designates the area around each pixel in 
%   which the coocurrence matrix is calculated.  DIST determines 
%   the city-block distance (from the center point) to points
%   that cooccur with the center point.  IS_VALID3 is a 3-D map of valid pixels.
%   FEATURE_NAMES allows a subset of Haralick features to be selected.  The
%   results are returned in FEATURES, a 4-D matrix with dimensions
%   [size(IMG3,1) size(IMG3,2) length(feature_names) size(IMG3,3)].  
%   FEATURE_NAMES returns the names of the features that were calculated.
%
%   The Haralick features are computed on a slice by slice basis.  No
%   interaction across slices is considered.
%  
%   See also haralick.

if isempty(feature_names)
    feature_names = {'contrast_energy','contrast_inverse_moment', ...
        'contrast_ave','contrast_variance','contrast_entropy', ... 
        'intensity_ave','intensity_variance','intensity_entropy', ...
        'entropy','energy', ...
        'correlation', ...
        'information_measure1','information_measure2'};
end

features = zeros(size(img3,1),size(img3,2),size(img3,3),length(feature_names),'single');
for i = 1:size(img3,3)
    disp(['Processing slice ' num2str(i) ' of ' num2str(size(img3,3)) '.']);
    [feats,names] = haralick(img3(:,:,i),graylevels,sig,dist,is_valid3(:,:,i),feature_names,comtx_type,varargin{:});
    features(:,:,i,:) = feats;
end
feature_names = names;