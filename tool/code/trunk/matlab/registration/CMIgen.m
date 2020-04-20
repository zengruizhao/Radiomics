function [combMI,nuniqueSA1,nuniqueSA2,nuniqueSA3,SA1,SA2,SA3,CNMI,CECC,CEOH]=CMIgen(I,J,N)
% CMI Combined Mutual Information of an image and an image ensemble.
%   Combined Mutual Information is defined by:
%       CMI(I1...In, J1...Jn) = S(I1...In) + S(J1...Jn) + S(I1...InJ1...Jn)
%
%   CMI(I,J,N) calculates CMI for N gray levels as:
%       MI(I, J) = S(I) + S(J) + S([I J])
%   Row of I and J are semi-independent images/features/components and
%   columns are pixels/observations.
%
%   [COMBMI,NUNIQUES_S1,NUNIQUES_S2,NUNIQUES_S3]=CMI(I,J,N) returns the
%   number of unique non-zero elements in the histograms of terms 1, 2 & 3
%   of CMI.
%
%   [...,S1,S2,S3] = CMI(...) returns the entropy estimates for the three
%   terms of the CMI formula.
%
%   See also HISTOGRAMNSP, HISTOGRAM2, ENTROPY, ENTROPY2, MI.
%
%JC

% Number of observations, dimensions
[~,DI]=size(I);
[elements,DJ]=size(J);

% Parse inputs
if ndims(I)>2 || ndims(J)>2,
    error('I and J must be P-by-D where P are observations and D are components');
end
if DI<2 && DJ<2,
    fprintf('Just use MI.\n');
end

% Calculate the three terms of MI/CMI

% S(I)
% Calculate DI-dimensional sparse (unlabeled elements) histogram
[h,nuniqueSA1]=histogramnsp(I,N);
% Normalize histogram --> PDF
p=h./elements;
% Marginal entropy
SA1=-sum(p.*log2(p));

% S(J)
% Calculate DJ-dimensional sparse (unlabeled elements) histogram
[h,nuniqueSA2]=histogramnsp(J,N);
% Hist --> PDF
p=h./elements;
% Joint entropy
SA2=-sum(p.*log2(p));

% S(IJ)
% Calculate (DI+DJ)-dimensional sparse (unlabeled elements) histogram
[h,nuniqueSA3]=histogramnsp([I J],N);
% Hist --> PDF
p=h./elements;
% Joint entropy
SA3=-sum(p.*log2(p));

% MI (CMI) estimate
combMI = SA1 + SA2 - SA3;
if nargout>7,
    CNMI = (SA1 + SA2)/SA3;
    CECC = 2-2*SA3/(SA1 + SA2);
    if nargout>9,
        CEOH=sum(p(:).*p(:));
    end
end
