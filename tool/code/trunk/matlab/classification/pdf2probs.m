function probs = pdf2probs(pdfs,featurevector)
% PDF2PROBS
%   Generate probabilities from a N-by-D pdf and a D-by-P feature vector.
%
%   probs = pdf2probs(pdfs,featurevector)
%
%   Output PROBS is P-by-D. Input PDFS is N-by-D and FEATUREVECTOR is
%   D-by-P.
%

%% Make probability data from pdfs
% Check dimensions of pdfs and multifeaturevector
[nfeats,nobs]=size(featurevector);
if nfeats~=size(pdfs,2); error('Dimensions don''t match'); end
fprintf('PDF->probs. -- Observations: %d, Features: %d.\n', nobs, nfeats );

% Graylevels
N=size(pdfs,1);

% Straighten multifeaturevector and scale from 1 to N 
featurevector=uint16(featurevector(:));
featurevector=featurevector+1;
if max(featurevector)>N, error('Number of intensities inconsistent with pdf size.'); end

% (features, pdf) -> probabilities
probs=pdfs(sub2ind(size(pdfs),featurevector,repmat(uint16(1:nfeats)',nobs,1)));
clear featurevector N pdfs
probs=reshape(probs,[nfeats nobs]);
probs=probs';

%% Do boosting
% addpath k:\code\boosting\
% 
% [boosting.minimumerr, boosting.ht_T, boosting.alphaT, boosting.bestthresh] = ...
%     bayesboost(probs, multilabels(:));
