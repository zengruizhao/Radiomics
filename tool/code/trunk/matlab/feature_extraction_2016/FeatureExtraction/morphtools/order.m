function [xy] = order(c);

%puts the coordinates in order need by centroid code
%ci=c(:,2:size(c,2));
ci=c;
for i=1:size(ci,2);
    xy(size(ci,2)+1-i,1)=ci(1,i);
    xy(size(ci,2)+1-i,2)=ci(2,i);
end

