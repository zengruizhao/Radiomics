function wc = woodsc(X,Y)
% WOODSC The Woods criterion two random variables X & Y.
%   WC = WOODSC(X,Y) calculates WC(X|Y) = 
%

x=X(:);
y=Y(:);

% my=mean(y);
% sigy=mean(y.^2-my.^2);

i=unique(x);
nuniques=length(i);

Ni=zeros(1,nuniques); myxi=zeros(1,nuniques); sigyxi=zeros(1,nuniques);
for ii=1:nuniques,
    omegai=x==i(ii);
    Ni(ii)=sum(omegai);
    myxi(ii)=mean(y(omegai));
    sigyxi(ii)=sqrt(mean(y(omegai).^2-myxi(ii).^2));
end

wc=-sum(Ni.*sigyxi./myxi)/numel(y);

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


