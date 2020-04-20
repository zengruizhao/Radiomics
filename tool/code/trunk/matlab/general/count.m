function [thecount,countwhat,inds,sortinds,uniqueinds] = count(inlist,element,hist)
% COUNT Count the number of occurrences of the unique elements of an array.
%   COUNT(LIST) returns the count for each of the unique elements in
%   ascending order.
%
%   [THECOUNT,COUNTWHAT] = COUNT(LIST) returns THECOUNT, which refers to 
%   the number of occurences of the unique elements contained in COUNTWHAT.
%
%   THECOUNT = COUNT(LIST,ELEMENT) counts number of occurences of the 
%   specified value ELEMENT.
%
%   For example, histogram of samples from a gausian distribution:
%   samples=round(20*randn(6000,1));
%   [thecount,countwhat] = count(samples);
%   bar(countwhat,thecount)
%
%   Above is similar to:
%   Nbins=length(unique(samples));
%   hist(samples,Nbins);
%
% JC

% Straighten
inlist=inlist(:);

% Deal with NaNs and Infs
[x,sortinds]=sort(inlist);
x=double(x);
% Count, then remove nans
nnans=length(find(isnan(inlist)));
x(isnan(x))=[];
% Count, then remove +/- Infs
npinfs=length(find(inlist==Inf));
nninfs=length(find(inlist==-Inf));
x(abs(x)==Inf)=[];

% Count elements by run length encoding
dx=diff(x);
thecount=diff(find([1; dx; 1]));
% Add NaN and Inf counts back
thecount=[repmat(nninfs,[double(nninfs>0) 1]); thecount; repmat(npinfs,[double(npinfs>0) 1]); repmat(nnans,[double(nnans>0) 1])];

% What is being counted
uniqueinds=[find(dx); numel(x)];
countwhat=x(uniqueinds); % x([dx~=0; true])
countwhat=[-Inf([double(nninfs>0) 1]); countwhat; Inf([double(npinfs>0) 1]); NaN([double(nnans>0) 1])];

if nargout>2,
    inds=sortinds(uniqueinds);
end

% Counting a particular element or asking for histogram
if nargin>1,
    if ~isempty(element);
        thecount=thecount(countwhat==element);
    end
    % Wanted a histogram?
    if nargin>2 && strcmp(hist,'hist'),
        notnan=~isinf(countwhat) & ~isnan(countwhat);
        figure;
        bar(countwhat(notnan),thecount(notnan))
        xlabel(['NaNs: ' num2str(nnans) ', -Infs: ' num2str(nninfs) ', +Infs: ' num2str(npinfs) ', TOTAL Infs: ' num2str(nninfs+npinfs)]);
    end
end
