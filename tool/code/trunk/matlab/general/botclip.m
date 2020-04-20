function [data_clipped,altdata_clipped] = botclip(data,altdata,numstd)
% function [data_clipped,altdata_clipped] = botclip(data,altdata,numstd)
% Clipping the bottom end of the histogram/distribution
% Can optionally pass in a second distribution as 'altdata' if you want both to
% be clipped in a similar manner
% For a single distribution you can call as data_clipped = botclip(data,[],numstd)
% Default for numstd = 5
% Satish Viswanath, Nov 2011

if ~exist('numstd','var')||isempty(numstd)
    numstd=5;
end

botclipval=median(data(data~=0))-numstd*std(data(data~=0));
data_clipped=data;

numvals_bot = length(find(data_clipped(data_clipped~=0)<botclipval));
data_clipped((data_clipped(data_clipped~=0))<botclipval)=round(linspace(botclipval+std(data(data~=0)),botclipval,numvals_bot));
if exist('altdata','var') && ~isempty(altdata)
    % do NOT independently rescale data!
    botclipval=botclipval*max(altdata(:))/max(data(:));
    altdata_clipped=altdata;
    %altdata_clipped(altdata_clipped>topclipval)=round(topclipval);
    numvals_bot = length(find(altdata_clipped(altdata_clipped~=0)<botclipval));
    altdata_clipped((altdata_clipped(altdata_clipped~=0))<botclipval)=round(linspace(botclipval+std(data(data~=0)),botclipval,numvals_bot));
end


end