function [uniquestrs,ustrinds,strlabels,ustrcounts]=strunique(cellofstrs,quiet)
% STRUNIQUE Find unique strings within a cell array of strings.
%   STRUNIQUE(CELLOFSTRS) returns the unique strings in a cell array
%   UNIQUESTRS of length <= length(CELLOFSTRS).
%
%   [UNIQUESTRS,USTRINDS] = STRUNIQUE(CELLOFSTRS) also returns a cell array
%   USTRINDS of the same size as UNIQUESTRS that contains the locations of
%   each unique string in CELLOFSTRS (e.g. the cell(s) returned by 
%   CELLOFSTRS{USTRINDS{1}} all contain the string in UNIQUESTRS{1}).
%
%   [UNIQUESTRS,USTRINDS,STRLABELS] = STRUNIQUE(CELLOFSTRS) returns a
%   double array STRLABELS of the same size as CELLOFSTRS that contains the
%   integer index of each string into UNIQUESTRS.
%
%   [UNIQUESTRS,USTRINDS,STRLABELS,USTRCOUNTS] = STRUNIQUE(CELLOFSTRS)
%   retunrs a double array USTRCOUNTS of the same size as UNIQUESTRS that
%   contains the number of occurences of each unique string in UNIQUESTRS.
%
%JC

if nargin<2,
    quiet=true;
end

% strinds=1:length(cellofstrs);
strlabels=zeros(size(cellofstrs));
% remaininginds=strinds;
nstrs=numel(cellofstrs);
remaininginds=1:nstrs;

ustrcount=0;npctchars=0;
while numel(remaininginds)>0,
    % Grab next string from first of remaining strings
    currentstr=cellofstrs{remaininginds(1)};
    uniquestrs{ustrcount+1}=currentstr; %#ok<*AGROW>

    % Check which of all remaining strings matches this string
    iscurrentstr=cellfun(@(x) strcmp(x,currentstr), cellofstrs(remaininginds));
    currentstrinds=remaininginds(iscurrentstr);
    strlabels(currentstrinds)=ustrcount+1;
    ustrinds{ustrcount+1}=currentstrinds;
    remaininginds=setdiff(remaininginds,currentstrinds);
    
    if ~quiet,
        pctchecked=100*(1-numel(remaininginds)/nstrs);
        pcttext=sprintf('%5.1f%%',pctchecked);
        backspaces=repmat('\b',1,npctchars);
        fprintf([backspaces '%s'],pcttext);
        npctchars=numel(pcttext);
    end
    
    ustrcount=ustrcount+1;
end

ustrcounts=cellfun(@numel, ustrinds);
