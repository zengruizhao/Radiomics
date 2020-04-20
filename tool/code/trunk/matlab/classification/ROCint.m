function A = ROCint(xroc,yroc)
% ROCINT Area under the ROC.
%   A = ROCint(xroc,yroc) integrates by calculating trapezoidal area.
%
%   xroc A.K.A. FP rate A.K.A. 1-specificity
%   yroc A.K.A. TP rate A.K.A. sensitivity
%
%   See also ROCINTFULL.
%
% 03/13/07 JC 
% xroc(end+1)=1; yroc(end+1)=1;
width=abs(xroc(1:end-1)-xroc(2:end));
aveheight=(yroc(1:end-1)+yroc(2:end))/2;
A=sum(abs(width.*aveheight));