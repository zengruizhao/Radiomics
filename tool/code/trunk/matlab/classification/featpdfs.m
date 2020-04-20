function [pdfs] = featpdfs(cancerfeaturevector,N)
% FEATPDFS
%   [pdfs] = featpdfs(cancerfeaturevector,N)
%
%   Build PDFs from a feature vector.
%
%   Input CANCERFEATUREVECTOR is D-by-N. Input N is a scalar indicating the
%   number of bins. CANCERFEATUREVECTOR must be pre-scaled to 0 to N-1.
%
%   Example:
%       cancerfeaturevector = getTPScancerfeaturesmultiset(histmask_tps,MRslicenums);
%       pdfs = featpdfs(cancerfeaturevector,256);
%

% Check # of feautres and observations
nfeats=size(cancerfeaturevector,1);
npts=size(cancerfeaturevector,2);

% Check data
if any(any(mod(cancerfeaturevector,1))),
    error('Input feature vector contains non-integer values.');
end

% Initialize PDF
pdfs=zeros(N,nfeats);
if max(max(cancerfeaturevector))>=N,
    error('Wrong number of intensity levels.');
end

% For each feature, make a histogram (a pdf)
for i=1:nfeats,
    % Count integer elements
    [thecount,countwhat]=count(cancerfeaturevector(i,:));
    % Normalize the histogram to produce a PDF estimage
    pdfs(countwhat+1,i)=thecount/npts;
end

% slicepermhash=num2str(sum(used_slices.*(1:nused_slices)));
