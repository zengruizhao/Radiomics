function m=compute_m(F);

[M,N]=size(F);
[x,y]=meshgrid(1:N,1:M);

%Turn x,y, and F into column vectors to make the summations a bit easier to
%compute in the following;
x=x(:);
y=y(:);
F=F(:);

%DIP equation (11.3-12);
m.m00=sum(F);
%protect against divide by zero warnings.
if (m.m00==0)
    m.m00=eps;
end
%The other central moments
m.m10=sum(x.*F);
m.m01=sum(y.*F);
m.m11=sum(x.*y.*F);
m.m20=sum(x.^2.*F);
m.m02=sum(y.^2.*F);
m.m30=sum(x.^3.*F);
m.m03=sum(y.^3.*F);
m.m12=sum(x.*y.^2.*F);
m.m21=sum(x.^2.*y.*F);
