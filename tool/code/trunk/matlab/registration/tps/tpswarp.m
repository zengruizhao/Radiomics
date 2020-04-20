function [Iwarped,is,it,Xw0,Yw0,X,Y,testgrid] = tpswarp(I)
% TPSWARP
%   Interactive TPS warping of and image I.
%   
%   [IWARPED,IS,IT,XW0,YW0,X,Y,TESTGRID] = TPSWARP(I)
%
%   You will be prompted for control points and instructed on what to do.
%
%   Outputs warped image IWARPED, rounded linear index mappings IS-->IT, 
%   the unrounded x,y coordinate deformation fields XW0 and YW0, the usual 
%   reference meshgrids X and Y, and a warped binary grid TESTGRID.
%
%   Calls TPSCPWARP once control points are selected.
%
%   See also TPSCPWARP.
%
%
%JC

nplanes=size(I,3);

% interactive control point deformation
h1=figure; axis; set(gca,'Position',[0 0 1 1]);
h2=figure; axis; set(gca,'Position',[0 0 1 1]);
    % show original slices (adjacent images)
    figure(h1); imdisp(I(:,:,1)); title('Target')
    figure(h2); imdisp(I(:,:,1)); title('Template')
    % get control points on adacent pairs of images
    but=1;
    count=1;
    while but~=3;
        figure(h1);
        [xi1,yi1]=getpoints(1);
        figure(h2);
        [xi2,yi2,but]=getpoints(1);
        if ~isempty(xi1),
            Xp(count)=xi1; Yp(count)=yi1; %#ok<AGROW>
            Xg(count)=xi2; Yg(count)=yi2; %#ok<AGROW>
        end
        count=count+1;
    end
    
% warp
% test mesh
Iheight=size(I,1);
Iwidth=size(I,2);
testim=zeros(Iheight,Iwidth);
testim(4:20:size(I,1),:)=1;
testim(:,4:20:size(I,2))=1;

% do the warp
Iwarped=zeros(size(I));
for i=1:nplanes,
    [Iwarped(:,:,i),is,it,Xw0,Yw0,X,Y] = tpscpwarp(I(:,:,i),I(:,:,i),[Xp' Yp'],[Xg' Yg']);
end

% also warp the test mesh
imethod='linear';
% testgrid=interp2(reshape(X,Iheight,Iwidth),reshape(Y,Iheight,Iwidth),...
%     testim,...
%     reshape(Xw0,Iheight,Iwidth),reshape(Yw0,Iheight,Iwidth),...
%     imethod);
testgrid=griddata(reshape(Xw0,Iheight,Iwidth),reshape(Yw0,Iheight,Iwidth),...
    testim,...
    reshape(X,Iheight,Iwidth),reshape(Y,Iheight,Iwidth),imethod);
% testgrid=zeros(size(I,1),size(I,2));
% testgrid(it)=testim(is);

% display the results
figure;imdisp(rescale(Iwarped(:,:,1)));
figure;imdisp(testgrid)

return

function Iout = rescale(I)

if ~isa(I,'double'),
    I=double(I);
end

if (max(I(:)) - min(I(:))) > eps,
    Iout = (I-min(I(:)))/(max(I(:))-min(I(:)));
else
    Iout=I;
end
