function [combMI,nuniqueSA2,nuniqueSA3,SA1,SA2,SA3,CNMI,CECC,CEOH]=CMI(Is,N)
% CMI Combined Mutual Information of an image and an image ensemble.
%   Combined Mutual Information is defined by:
%       CMI(I1, I2...In) = S(I1) + S(I2...In) + S(I1...In)
%
%   CMI(IS,N) calculates CMI for N gray levels as:
%       MI(Is(:,1), Is(:,2:end)) = S(Is(:,1)) + S(Is(:,2:end)) + S(Is)
%   Row of Is are semi-independent images/features/components and columns
%   are pixels/observations.
%
%   [COMBMI,NUNIQUES_S2,NUNIQUES_S3]=CMI(IS,N) returns the number of
%   unique non-zero elements in the histograms of terms 2 and 3 of CMI.
%
%   [...,S1,S2,S3] = CMI(...) returns the entropy estimates for the three
%   terms of the CMI formula.
%
%   See also HISTOGRAMNSP, HISTOGRAM2, ENTROPY, ENTROPY2, MI.
%
%JC

% Number of observations, dimensions
[elements,D]=size(Is);

% Parse inputs
if ndims(Is)>2,
    error('IS must be P-by-D where P are observations and D are components');
end
if D<3,
    fprintf('Just use MI.\n');
end

% Calculate the three terms of MI/CMI

% S(A1)
% Make a quick histogram
x=sort(Is(:,1));
h=diff(find([1; diff(x); 1]));
% Normalize histogram --> PDF
p1=h./elements;
% Marginal entropy
SA1=-sum(p1.*log2(p1));

% S(A2A3..An)
% Calculate D-1 dimensional sparse (unlabeled elements) histogram
% [h,nuniqueSA2]=joint_hNdsp_test(Is(:,2:end),N);
[h,nuniqueSA2]=histogramnsp(Is(:,2:end),N);
% Hist --> PDF
pD2=h./elements;
% Joint entropy
SA2=-sum(pD2.*log2(pD2));

% S(A1A2A3..An)
% Histogram
% [h,nuniqueSA3]=joint_hNdsp_test(Is,N);
[h,nuniqueSA3]=histogramnsp(Is,N);
% Hist --> PDF
pD1=h./elements;
% Joint entropy
SA3=-sum(pD1.*log2(pD1));

% MI (CMI) estimate
combMI = SA1 + SA2 - SA3;
if nargout>6,
    CNMI = (SA1 + SA2)/SA3;
    CECC = 2-2*SA3/(SA1 + SA2);
    if nargout>=9,
        CEOH=sum(pD1(:).*pD1(:));
    end
end
