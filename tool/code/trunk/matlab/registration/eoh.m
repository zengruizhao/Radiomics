function [eh]=eoh(Aregd,Bregd,N)
% MI Mutual information of two images or random variables.
%   MI(I1,I2,N) returns MI of random variables I1 and I2 for N graylevels.
%   MI is calculated using the joing entropy of the two arrays.
%
%   [MI,H,H1,H2] = MI(I1,I2,N) returns the joint entropy H and the marginal
%   entropies H1 and H2.
%
%   [MI,H,H1,H2,NMI,ECC] = MI(I1,I2,N) returns the normalized mutual
%   informtion NMI and the entropy correlation coefficient ECC.
%
%   [MI,H,H1,H2,NMI,ECC,P,P1,P2] = MI(I1,I2,N) returns the joint gray
%   level probability distribution estimate P and the individual pdfs P1
%   and P2.
%
%   See also HISTOGRAM2, ENTROPY, HISTOGRAM.
%
%JC

if ndims(Aregd)~=ndims(Bregd),
    error('Dimensions of Aregd and Bregd not equal.')
end

% Calculating joint histogram for two images
hAB=histogram2(Aregd,Bregd,N);

% Normalize to a PDF
pAB=hAB./numel(Aregd);
% nuniquesS3=sum(p(:)>0)

eh=sum(pAB(:).*pAB(:));
