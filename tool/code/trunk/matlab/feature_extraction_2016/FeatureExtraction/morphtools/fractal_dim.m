function frac_dim=fractal_dim(xy)
%fractal dimension
%xy being the matrix containin x and y coordinates

n=size(xy,1); %assuming 2 column vectors

for i=1:n/2;
    j=floor(n/i);
    
    for s=1:j;
        lx(s,1)=xy(1+(s-1)*i,1);
        ly(s,1)=xy(1+(s-1)*i,2);
    end
    
    lx(s+1)=xy(n,1); ly(s+1)=xy(n,2);
    lxy=[lx,ly];
    
    
    %for long distances
    for a=1:size(lx,1)-1;
        dislong(a)=sqrt((lx(a)-lx(a+1)).^2+(ly(a)-ly(a+1)).^2);
    end
    
    d(i)=sum(dislong); 
    clear lx ly lxy dislong
end

plot(log10(1./(1:n/2)),log10(d))
%plot((1:n/2),d,'o')
eq=polyfit(log10(1./(1:n/2)),log10(d),1);
%eq=polyfit((1:n/2),d,1)
frac_dim=eq(1);