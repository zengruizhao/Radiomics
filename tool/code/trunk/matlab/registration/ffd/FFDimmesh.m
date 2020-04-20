function [Iout,Px,Py,Pxdispl,Pydispl,tlocalxnew,tlocalynew] = FFDimmesh(I,Px,Py,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith)
% FFDimmesh Apply Free Form Deformations specified by grid displacemnets to image I.
%  Iout = FFDimmesh(I,Px,Py,Pxdispl,Pydispl)
%
%   I is an image to be deformed.
%   Iout is the deformed image.
%   PX and PY are the coordinates of the initial mesh grid vertices.
%   PXDISPL and PYDISPL are the coordinates of the displaced mesh grid
%   vertices.
%
%JC

warning('FFDimmesh.m is not up-to-date.  Use imffd!');

%%% Inputs

ninplanepixels=prod([size(I,1),size(I,2)]);
if nargin>6 && ~isempty(pretlocalx) && ~isempty(pretlocaly) && ...
    any([numel(pretlocalx) numel(pretlocaly)]~=ninplanepixels),
        error('Initial local deformation fields are incorrect size.');
elseif nargin<7,
    pretlocalx=[];
    pretlocaly=[];
end

if nargin<8,
    fillwith=cast(0,class(I));
end

% image dimensions
Xmax=size(I,2);
Ymax=size(I,1);

ncpsy=size(Px,1); ncpsx=size(Px,2);
PGridsize=[ncpsy ncpsx];
xcpstart=Px(1); ycpstart=Py(1);
Pxdiff=diff(Px,1,2); Pydiff=diff(Py,1,1); 
spx=Pxdiff(1);       spy=Pydiff(1);

%%% Define Deformations

% Define Bezier blending functions
B{1} = @(t) (1-t).^3/6;
B{2} = @(t) (3*t.^3-6*t.^2+4)/6;
B{3} = @(t) (-3*t.^3+3*t.^2+3*t+1)/6;
B{4} = @(t) (t.^3)/6;

% Pdispl=permute(cat(3,Pxdispl,Pydispl),[3 1 2]);

% all image points
[X,Y]=meshgrid(1:Xmax,1:Ymax);
% local def region location
s=mod((X(:)-xcpstart)/spx,1);
t=mod((Y(:)-ycpstart)/spy,1);
% def region
i=floor((X(:)-xcpstart)/spx)+1;
j=floor((Y(:)-ycpstart)/spy)+1;

% Initialize deformation field
tlocalx=zeros(ninplanepixels,1); 
tlocaly=zeros(ninplanepixels,1); 

% Arbitrary control points - not actually implemented
% if nargin>8,
%     [Xcp,Ycp]=deal(cpoints(1,:),cpoints(2,:));
% 
%     scp=mod((Xcp(:)-xcpstart)/spx,1);
%     tcp=mod((Ycp(:)-ycpstart)/spy,1);
%     % def region
%     icp=floor((Xcp(:)-xcpstart)/spx)+1;
%     jcp=floor((Ycp(:)-ycpstart)/spy)+1;
%     
%     cpointsout=cpoints;
% else
%     cpointsout=[];
% end

% Flag points that don't have required neighboring control points
oobmask=true(size(tlocalx));
for m=[1 4],
    for n=[1 4],
%         oobmask=i+m-2 > 0 & i+m-2 <= ncpsx & j+n-2 > 0 & j+n-2 <= ncpsy;
        oobmask=oobmask & i+m-2 > 0 & i+m-2 <= ncpsx & j+n-2 > 0 & j+n-2 <= ncpsy;
    end
end

% Following code uses out-of-bounds checking during tensor product - SLOW
% % reduce indexes and parameters
% inow=i(oobmask); jnow=j(oobmask);
% snow=s(oobmask); tnow=t(oobmask);
% 
% % points without the required neighboring control points don't move
% tlocalx(~oobmask)=X(~oobmask);
% tlocaly(~oobmask)=Y(~oobmask);
% 
% % precalculate the bezier blending functions for all s and t
% for mn=1:4,
%     Bms{mn}=B{mn}(snow);
%     Bnt{mn}=B{mn}(tnow);
% end
% 
% for m=1:4,
%     % Bms=B{m}(snow);
%     % Bms=bezier(m,snow);
%     for n=1:4,
%         % next control point inds
%         % tlxi=sub2ind(size(Pdispl),repmat(1,size(jnow)),jnow+n-2,inow+m-2);
%         % tlyi=sub2ind(size(Pdispl),repmat(2,size(jnow)),jnow+n-2,inow+m-2);
%         % tli=sub2ind(PGridsize,jnow+n-2,inow+m-2);
%         tli=(jnow+n-2)+(inow+m-2-1)*PGridsize(1);
% 
%         % sum for these cps
%         % Bnt=B{n}(tnow);
%         % Bnt=bezier(n,tnow);
%         Bproduct=Bms{m}.*Bnt{n}; % or Bms.*Bnt if calculating in loop
%         % tlocalx(oobmask)=tlocalx(oobmask)+Bproduct.*Pdispl(tlxi);
%         % tlocaly(oobmask)=tlocaly(oobmask)+Bproduct.*Pdispl(tlyi);
%         tlocalx(oobmask)=tlocalx(oobmask)+Bproduct.*Pxdispl(tli);
%         tlocaly(oobmask)=tlocaly(oobmask)+Bproduct.*Pydispl(tli);
%         
%         % To run with no deformation:
%         % tlocalx=tlocalx+B{m}(s).*B{n}(t).*(xcpstart+((i+m-2)-1)*spx);
%         % tlocaly=tlocaly+B{m}(s).*B{n}(t).*(ycpstart+((j+n-2)-1)*spy);
%         
%         % To loop through x,y (i,j,s,t):
%         % if i+m-2 > 0 && i+m-2 <= ncpsx && j+n-2 > 0 && j+n-2 <= ncpsy,
%         %    tlocalx=tlocalx+B{m}(s).*B{n}(t).*Pdispl(1,i+m-2,j+n-2);
%         %    tlocaly=tlocaly+B{m}(s).*B{n}(t).*Pdispl(2,i+m-2,j+n-2);
%         % end
%     end
% end

% Faster code below: no oob checking -- be sure to pad with control points
if ~all(all(oobmask)),
    fprintf('ERROR!\n');
    display(Px)
    display(Py)
    display(Pxdispl)
    display(Pydispl)
    error('Pad control point mesh first/more.');
end

% Precalculate the Bezier blending functions for all s and t
Bms=zeros(numel(s),4);
Bnt=zeros(numel(s),4);
for mn=1:4,
    Bms(:,mn)=B{mn}(s);
    Bnt(:,mn)=B{mn}(t);
%     Bms(:,mn)=bezier(mn,s);
%     Bnt(:,mn)=bezier(mn,t);
end

% Desired arbitrary control points, along for the ride
% if nargin>5,
%     Bmscp=zeros(numel(scp),4);
%     Bntcp=zeros(numel(scp),4);
%     for mn=1:4,
%         Bmscp(:,mn)=B{mn}(scp);
%         Bntcp(:,mn)=B{mn}(tcp);
%     end
% end

% Perform tensor product on all coordinates
for m=1:4,
    % Bms=B{m}(snow);
    % Bms=bezier(m,snow);
    for n=1:4,
        % next control point inds
        % tli=sub2ind(PGridsize,jnow+n-2,inow+m-2);
        tli=(j+n-2)+(i+m-2-1)*PGridsize(1);

        % sum for these cps
        % Bnt=B{n}(tnow);
        % Bnt=bezier(n,tnow);
        Bproduct=Bms(:,m).*Bnt(:,n); % or Bms.*Bnt if calculating in loop
        tlocalx=tlocalx+Bproduct.*Pxdispl(tli);
        tlocaly=tlocaly+Bproduct.*Pydispl(tli);
        
        % To run with no deformation:
        % tlocalx=tlocalx+Bproduct.*(xcpstart+((i+m-2)-1)*spx);
        % tlocaly=tlocaly+Bproduct.*(ycpstart+((j+n-2)-1)*spy);
        
        % Arbitrary Control Points
        % if nargin>8,
        %     tli=(jcp+n-2)+(icp+m-2-1)*PGridsize(1);
        %     Bproduct=Bmscp(:,m).*Bntcp(:,n); % or Bms.*Bnt if calculating in loop
        %     cpointsout(1,:)=cpointsout(1,:)+Bproduct.*Pxdispl(tli);
        %     cpointsout(2,:)=cpointsout(2,:)+Bproduct.*Pydispl(tli);
        % end
    end
end

%%% Deform image

[Iout,tlocalxnew,tlocalynew] = forwarddisp(I,tlocalx, tlocaly, pretlocalx, pretlocaly,fillwith);

%%% display result
% figure
% imdisp(uint8(round(Iout)))
% hold on
% plot(Pxdispl(:),Pydispl(:),'r.'); hold on
% plot(Pxdispl,Pydispl,'r')
% plot(Pxdispl',Pydispl','r')
