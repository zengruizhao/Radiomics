function [Itargetw,is,it,CPs,CPt,testgriddef,testgrid]  = tpsreg(Itarget,Itempl)
%TPSREG
% [Itargetw,is,it,CPs,CPt]  = tpsreg(Itarget,Itempl)
%
%   Register images ITARGET (moving) and ITEMPLATE (fixed) using thin plate
%   splines (TPS).
%
%   THIS IS INTERACTIVE.
%
%   See also TPSCPWARP.
%
%JC

nplanes=size(Itarget,3);

%%% interactive control point deformation
h1=figure; axis; set(gca,'Position',[0 0 1 1]);
h2=figure; axis; set(gca,'Position',[0 0 1 1]);
    % show original slices (adjacent images)
    figure(h1); imdisp(Itarget)
    figure(h2); imdisp(Itempl)
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
    CPt=[Xp' Yp'];
    CPs=[Xg' Yg'];
    
%%% warp
% test mesh
testgrid=zeros(size(Itarget,1),size(Itarget,2));
testgrid(4:20:size(Itarget,1),:)=1;
testgrid(:,4:20:size(Itarget,2))=1;

% do the image warp
Itargetw=zeros(size(Itarget));
% Itargetw_alt=zeros(size(Itarget));
for i=1:nplanes,
    [Itargetw(:,:,i),is,it] = tpscpwarp(Itarget(:,:,i),Itempl(:,:,i),CPt,CPs);
%     [Itargetw_alt(:,:,i),is_alt,it_alt] = tpscpwarp_alt(Itarget(:,:,i),Itempl(:,:,i),CPt,CPs);
end

% also warp the test mesh
testgriddef=zeros(size(Itarget));
testgriddef(it)=testgrid(is);

% display the results
figure;imdisp(rescale(Itargetw)); title('Deformed Target')
figure;imdisp(testgriddef); title('Deformation Field on Grid');
