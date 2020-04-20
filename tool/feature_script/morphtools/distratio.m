%Program for new distance ratio
function dratio= distratio(xy)
%xy being the matrix containin x and y coordinates
n=size(xy,1); %assuming 2 column vectors

if n >=800
    k=n/200;
    j=floor(k)*200;
    if abs(n-j) ==  0;
         j=j-199;
    else
        j=j+1;
    end
    
    s=(j-1)/200;
    for i=1:s+1;
        lx(i,1)=xy(1+(i-1)*200,1);
        ly(i,1)=xy(1+(i-1)*200,2);
    end
    lxy=[lx,ly];
end

if n > 400 & n < 800
    k=n/100;
    j=floor(k)*100;
    if abs(n-j) == 0;
       j=j-99;
    else
        j=j+1;
    end
    s=(j-1)/100;
    for i=1:s+1;
        lx(i,1)=xy(1+(i-1)*100,1);
        ly(i,1)=xy(1+(i-1)*100,2);
    end
    lxy=[lx,ly];
end

if n <= 400
    k=n/10;
    j=floor(k)*5;
    if abs(n-j) >= 51;
       j=j+51;
    else
        j=j+1;
    end
    s=(j-1)/5;
    for i=1:s+1;
        lx(i,1)=xy(1+(i-1)*5,1);
        ly(i,1)=xy(1+(i-1)*5,2);
    end
    lxy=[lx,ly];
end



%for long distances
for a=1:size(lxy,1)-1;
    dislong(a)=sqrt((lx(a)-lx(a+1)).^2+(ly(a)-ly(a+1)).^2);
end
dislong=dislong';

%for smaller distances
for b=1:j-1;
    disshort(b)=sqrt((xy(b,1)-xy(b+1,1)).^2+(xy(b,2)-xy(b+1,2)).^2);
end
disshort=disshort';
dl=sum(dislong);
ds=sum(disshort);
dratio=dl/ds;