function subimdisp(Istack,aspectRatio,plottitle)

if nargin<2 || isempty(aspectRatio), aspectRatio=1; end
if nargin<3, plottitle=[]; end

if iscell(Istack)
    nimages=numel(Istack);
else
    if ndims(Istack)==4
        nimages=size(Istack,4);
        isRGB=true;
    elseif ndims(Istack)==3
        nimages=size(Istack,3);
        isRGB=false;
    else
        error('Cannot understand dimensions.');
    end
end

subplotRows = sqrt(nimages / aspectRatio);
subplotRows = round(subplotRows);
subplotCols = ceil(nimages / subplotRows);
subplotSize = [subplotRows subplotCols];

if iscell(Istack),
    for i=1:nimages,
        subplot(subplotSize(1),subplotSize(2),i)
        imdisp(Istack{i});
    end
elseif isRGB
    for i=1:nimages,
        subplot(subplotSize(1),subplotSize(2),i)
        imdisp(Istack(:,:,:,i));
    end
else
    for i=1:nimages,
        subplot(subplotSize(1),subplotSize(2),i)
        imdisp(Istack(:,:,i));
    end
end

subplot(subplot(subplotSize(1),subplotSize(2),floor(subplotSize(2)/2)))
title(plottitle)
