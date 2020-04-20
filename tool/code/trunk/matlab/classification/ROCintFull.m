function A = ROCintFull(xroc,yroc)
% ROCINTFULL Area under the ROC.
%   A = ROCINTFULL(xroc,yroc) integrates by calculating trapezoidal area.
%
%   xroc A.K.A. FP rate A.K.A. 1-specificity
%   yroc A.K.A. TP rate A.K.A. sensitivity
%
%   NOTE: ROCintFull, unlike ROCint, INTEGRATES TO A (POSSIBLY)FICTIONAL
%   POINT AT (1,1), THE TOP RIGHT CORNER.
%
%   See also ROCINT.
%
% 03/13/07 JC 
if sum(diff(xroc(:)))<0,
    xroc=[1; xroc(:)]; yroc=[1; yroc(:)];
else
    xroc(end+1)=1; yroc(end+1)=1;
end
width=abs(xroc(1:end-1)-xroc(2:end));
aveheight=(yroc(1:end-1)+yroc(2:end))/2;
A=sum(abs(width.*aveheight));