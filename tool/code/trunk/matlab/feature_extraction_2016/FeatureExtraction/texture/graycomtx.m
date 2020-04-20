% COMTX = GRAYCOMTX(BLOCK,OPTMX,GRAYLEVELS,IS_VALID)
%   GRAYCOMTX computes a GRAYLEVELSxGRAYLEVELS cooccurence matrix for the
%   UINT16 matrix BLOCK.  All values of BLOCK must lie in the interval 
%   [0:GRAYLEVELS-1]. OPTMX determines the region of coocurrence.
%   Though OPTMX can be a matrix of DOUBLES with arbitrary values, the 
%   typical usage is:
%   optmx = [1 1 1 
%            1 0 1
%            1 1 1];
%   Each value of OPTMX determines the amount added to the coocurrence
%   matrix when the designated pixel is considered with respect to the center
%   pixel.  IS_VALID (which is the same size as BLOCK) indicates which
%   elements of BLOCK are valid.  If IS_VALID is not specified, all pixels
%   are assumed valid.
%
%   See also haralick and haralick3.