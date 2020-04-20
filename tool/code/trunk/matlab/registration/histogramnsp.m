function [h,nuniqueelements,uniqueelements]=histogramnsp(arrayn,N)
% HISTOGRAMNSP N-dimensional sparse histogram.
%   HISTOGRAMNSP(ARRAYN,N) returns the non-zero counts of the D-dimensional
%   (N1-by-N2-by-...-by-ND) histogram of P-by-D array ARRAYN where each Ni 
%   is the number of bins N for each dimension.
%
%   [H,NUNIQUEELEMENTS]=HISTOGRAMNSP(...) returns the number of unique 
%   elements counted (number of non-zero bins) by H. A completely
%   disperse histogram occurs when NUNIQUEELEMENTS==size(ARRAYN,1).
%
%   [H,NUNIQUEELEMENTS,UNIQUEELEMENTS]=HISTOGRAMNSP(...) also returns the
%   values of the elements counted by histogram H. That is, UNIQUEELEMENTS
%   contains the linear index of the location in the histogram of the
%   counts in H. (i.e. [I,J,K....]=ind2sub(repmat(N,1,D),uniqueelements)
%   corresponds to the locations of the non-zere elements in H).
%
%   See also: ENTROPYN, CMI, ENTROPY, ENTROPY2.
%
%JC

[~, D]=size(arrayn);

% Joint histogram of N-D
if D~=1,
    % hinds=submat2ind2(repmat(N,1,D),double(arrayn)+1);
    
    % Number of elements traversed per dimension
    perdimelements=[1 cumprod(repmat(N,1,D-1))];
    
    % Treating values as coordinates, calculate histogram linear index
    hinds=1;
    for i=1:D,
        hinds=hinds+double(arrayn(:,i))*perdimelements(i);
    end
    
    % Sort linear indices 
    x=sort(hinds);
else % Histogram of 1D
    x=double(sort(double(arrayn)+1));
end

% Histogram by run length encoding of sorted indices
h=diff(find([1; diff(x); 1]));

% Output histogram coordinates if asked for
if nargout>1,
    nuniqueelements=length(h);
    if nargout>2,
        uniqueelements=x(find(diff([x; max(x)+1]))); %#ok<FNDSB>
    end
end
