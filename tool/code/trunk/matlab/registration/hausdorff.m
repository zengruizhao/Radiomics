function [hdist, meandist1, meandist2] = hausdorff(X,Y)
% HAUSDORFF
%   HDIST = HAUSDORFF(X,Y) where X and Y are DxM and DxN.
%
%JC

% Pairwise distances between points on each curve
distmat = edist(X,Y);

% Distance from each point on X to the closest point on Y
cp1=min(distmat);

% Distance from each point on Y to the closest point on X
cp2=min(distmat,[],2);

% The greater of the bigger distances
hdist=max(max(cp1),max(cp2));

% Also calculate means
if nargout>1,
    meandist1=mean(cp1);
    if nargout>2,
        meandist2=mean(cp2);
    end
end

function d = edist(a,b)
% EDIST Euclidean distance.

if size(a,1) ~= size(b,1),
   error('A and B should be of same dimensionality');
end

if ~(isreal(a) && isreal(b)),
   warning('edist:ImaginaryNums','Running with imaginary numbers.  Results may be off.'); 
end

if size(a,1) == 1,
  a = [a; zeros(1,size(a,2))]; 
  b = [b; zeros(1,size(b,2))]; 
end

% Works with uint32 (or whatever)
dataclass=class(a);
aa=sum(a.*a,'native'); bb=sum(b.*b,'native'); 
aabb=bsxfun(@plus,aa',bb);
clear aa bb

% Calculate a*b
if isinteger(a),
    a=single(a);b=single(b);
    ab=a'*b;
    ab=cast(ab,dataclass);
else % (if double or single)
    ab=a'*b;
end
clear a b

% calculate distance and return as double
d = double(aabb - 2*ab);
clear ab aabb a b
d=sqrt(d);
