function [data_clipped,altdata_clipped] = topclip(data,altdata,numstd)
% function [data_clipped,altdata_clipped] = topclip(data,altdata,numstd)
% Clipping the top end of the histogram/distribution
% Can optionally pass in a second distribution as 'altdata' if you want both to
% be clipped in a similar manner
% For a single distribution you can call as data_clipped = topclip(data,[],numstd)
% Default for numstd = 5
% Satish Viswanath, Nov 2011

if ~exist('numstd','var')||isempty(numstd)
    numstd=5;
end

topclipval=median(data(data~=0))+numstd*std(data(data~=0));
data_clipped=data;

numvals_top = length(find(data_clipped>topclipval));
data_clipped(data_clipped>topclipval)=round(linspace(topclipval,topclipval-std(data(data~=0)),numvals_top));
if exist('altdata','var') && ~isempty(altdata)
    % do NOT independently rescale data!
    topclipval=topclipval*max(altdata(altdata~=0))/max(data(data~=0));
    altdata_clipped=altdata;
    %altdata_clipped(altdata_clipped>topclipval)=round(topclipval);
    numvals_top = length(find(altdata_clipped>topclipval));
    altdata_clipped(altdata_clipped>topclipval)=round(linspace(topclipval-std(data(data~=0)),topclipval,numvals_top));
end

end