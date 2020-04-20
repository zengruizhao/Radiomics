%code for perimeter to area ratio 
function [paratio,peri]=periarea(xy, area);
%xy being the matrix containing x and y coordinates
n=size(xy,1);%nx2 matrix
for i=1:n-1;
    dist(i)=sqrt((xy(i,1)-xy(i+1,1)).^2+(xy(i,2)-xy(i+1,2)).^2);
end
ld=sqrt((xy(n,1)-xy(1,1)).^2+(xy(n,2)-xy(1,2)).^2);


peri=sum(dist)+ld;

paratio=(peri.^2)/area;
