function [cr,wc,nuniques] = corrrat(x,y)
% CORRRAT The correlation ratio of two random variables X and Y.
%   CR = CORRRAT(X,Y) calculates CR(X|Y) = var(E(Y|X)) / var(Y).
%
%   [CR,WC] = CORRRAT(X,Y) also returns WC, the Woods criterion.
%
%   Reference
%   ---------
%   Roche, A., Pennec, X. and Ayache, N. "The correlation ratio as a new
%   similarity measure for multimodal image registration." MICCAI 1999,
%   v1496, pp 1115-1124.
%
%JC

calcwc=nargout>1;

x=x(:);
y=y(:);

my=mean(y);
vary=mean(y.^2-my.^2);

i=unique(x);
nuniques=length(i);

Ni=zeros(1,nuniques); myxi=zeros(1,nuniques); varyxi=zeros(1,nuniques); sigyxi=zeros(1,nuniques);
% See Section 2.3 of Roche, MICCAI 1998, v1496:1115-1124
for ii=1:nuniques,
    omegai=x==i(ii);
    Ni(ii)=sum(omegai);
    myxi(ii)=mean(y(omegai));
    varyxi(ii)=mean(y(omegai).^2-myxi(ii).^2);
    if calcwc,
        sigyxi(ii)=sqrt(mean(y(omegai).^2-myxi(ii).^2));
    end
end

cr=1-sum(Ni.*varyxi)/(numel(y)*vary);
if calcwc,
    wc=-sum(Ni.*sigyxi./myxi)/numel(y);
end

% crsum=0;
% for ii=1:nuniques,
%     omegai=x==i(ii);
%     Ni=sum(omegai);
%     myxi=mean(y(omegai));
%     varyxi=mean(y(omegai).^2-myxi.^2);
%     crsum=crsum+Ni*varyxi;
% end
% 
% cr=1-crsum/(numel(y)*vary);

% xs=double(sort(X(:)));
% thecount=diff(find([1; diff(xs); 1]));
% countwhat=xs(find(diff([xs; max(xs)+1])));


