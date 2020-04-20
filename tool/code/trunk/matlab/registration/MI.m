function [MIval,h_xy,Hx,Hy,NMI,ECC,p,px,py,eoh]=MI(I1,I2,N,measure)
% MI Mutual information of two images or random variables.
%   MI(I1,I2,N) returns the MI between random variables I1 and I2, 
%   calculated using N histogram bins (values ranging from 0 to N-1).
%
%   [MI,H,H1,H2] = MI(I1,I2,N) returns the joint entropy H and the marginal
%   entropies H1 and H2 of I1 and I2, respectively.
%
%   [MI,H,H1,H2,NMI,ECC] = MI(I1,I2,N) returns the normalized mutual
%   informtion NMI and the entropy correlation coefficient ECC. By
%   definition, NMI ranges from 0 to 2, and ECC ranges from 0 to 1.
%
%   [MI,H,H1,H2,NMI,ECC,P,P1,P2] = MI(I1,I2,N) returns the joint
%   probability distribution estimate P and the individual pdfs P1 and P2
%   of I1 and I2. P will be of size N-by-N. P1 and P2 are both 1-by-N. The 
%   value of P[i,j] reflects the probability of simultaneously observing 
%   the value i in I1 AND the value j in I2.
%
%   [...,EOH] = MI(...) returns the "energy" of the joint histogram in the
%   10th output argument, EOH.
%
%   Implementation Details
%   ----------------------
%   MI is calculated using the Shannon entropy of the two arrays by the
%   equation MI = S(I1) + S(I2) - S(I1,I2).  You can verify this by
%   calculating H1 + H2 - H.
%
%   Shannon entropy is calculated by sum(p*log2(p)). Therefore, the units
%   of MI are bits. This is intuitive in terms of coding complexity.
%
%   This function is used in registration techniques as an objective
%   function that is often evaluated MANY times. As such, certain checks
%   are omitted. Important omissions for speed include,
%     (1) N is not automatically calculated from the data.
%     (2) Values of I1 and I2 are not rounded or verified to be integers.
%
%   Examples
%   --------
%       N=256; x=round((N-1)*rand(1,10000));
%       [mival,hxy,hx,hxx,nmi,ecc]=MI(x,x,N)
%       % mival will be around 8 (2^8 equals 256)
%       % hxy, hx, and hxx will all be the same
%       % nmi will be 2 and ecc will be 1
%
%       N2=128; y=round((N2-1)*x/(N-1)); % y is x rescaled onto 0 to N2-1
%       [mival,hxy,hx,hy,nmi,ecc]=MI(x,y,N)
%       % mival will be around 7 (2^7 equals 128, x explains 7 bits of y)
%       % hx and hxy will be around 8 (x contains all uncertainty)
%       % hy will be around 7
%
%       y2=x; y2(mod(y2,2)>0)=y2(mod(y2,2)>0)-1;  % reduce complexity of y
%       [mival,hxy,hx,hy,nmi,ecc]=MI(x,y2,N) % same results as previous!
%
%   See also HISTOGRAM2, ENTROPY, ENTROPY2, HISTOGRAM.
%
%JC

if ndims(I1)~=ndims(I2),
    error('Dimensions of I1 and I2 not equal.')
end

if nargin==4 && nargout>4, error('only calculating requested measure'); end

% Calculating joint histogram for two images
a=histogram2mex(I1,I2,N);

% Normalize to a PDF
p=a./numel(I1);
% nuniquesS3=sum(p(:)>0)

% Histogram of each image/array
py=sum(p);  % sum rows
% nuniquesS2=sum(py>0)
px=sum(p,2)'; % sum cols
% nuniquesS1=sum(px>0)

% Marginal Entropies
Hy=sum(-(py(py>0).*(log2(py(py>0)))));
Hx=sum(-(px(px>0).*(log2(px(px>0)))));

% Joint Entropy
% Option 1:
% h_xy = -sum(sum(p.*(log2(p+(p==0)))));
% Option 2:
% nonzeros=p>0;
% h_xy = -sum(sum(p(nonzeros).*log2(p(nonzeros))));
% Option3:
h_xy = -sum(sum(p.*full(spfun(@log2,p))));

% Hx, Hy, H_xy --> MI, NMI, ECC, EOH
if nargin==4,
    switch lower(measure)
        case 'mi'
            MIval=Hx + Hy - h_xy;
        case 'nmi'
            MIval=(Hx + Hy)/h_xy;
        case 'ecc'
            MIval=2-2*h_xy/(Hx + Hy);
        case 'eoh'
            MIval=sum(p(:).*p(:));
    end
else
    MIval = Hx + Hy - h_xy;
    if nargout>4,
        NMI = (Hx + Hy)/h_xy;
        if nargout>5,
            ECC = 2-2*h_xy/(Hx + Hy);
            if nargout>9,
                eoh=sum(p(:).*p(:));
            end
        end
    end
end
