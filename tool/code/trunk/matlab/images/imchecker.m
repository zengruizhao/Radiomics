function Ichecker = imchecker(I1,I2,nchecks)
% IMCHECKER
%   Compare alignment of two images in a checkerboard pattern.
%
%   ICHECKER = IMCHECKER(I1,I2,NCHECKS)
%
%JC

dims=size(I1);
% nchecks=17;
[X,Y]=meshgrid(linspace(1,nchecks^2,dims(2)),linspace(1,nchecks^2,dims(1)));
checkpattern=logical(mod(ceil(X/nchecks)+ceil(Y/nchecks),2));
% Ichecker=I1;
Ichecker=zeros(size(I1));
for i=1:size(I1,3),
    I2i=I2(:,:,i);
    Icheckeri=I1(:,:,i);
    Icheckeri(~checkpattern)=I2i(~checkpattern);
    Ichecker(:,:,i)=Icheckeri;
end

