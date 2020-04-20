function [Iout,tlocalxnew,tlocalynew,tlocalznew,tlocalxlast,tlocalylast,tlocalzlast] = imffd3(I,Bproducts,tlis,Pxdispl,Pydispl,Pzdispl,pretlocalx,pretlocaly,pretlocalz,fillwith,Px,Py,Pz,Pxdispl_last,Pydispl_last,Pzdispl_last,tlocalxlast,tlocalylast,tlocalzlast,imethod)
% IMFFD3 Apply Free Form Deformations specified by mesh grid displacements 
%   to volume I.
%   
%   IOUT = IMFFD3(I,BPRODUCTS,TLIS,PXDISPL,PYDISPL,PZDISPL)
%
%   I is an image to be deformed.
%   Iout is the deformed image.
%   BPRODUCTS are the Bezier products calculated at every pixel.
%   TLIS are indexes into the mesh grid at each pixel and for each m and n.
%   BPRODUCTS and TLIS are calculated via imbeziers.m from PX and PY, the
%   coordinates of the initial mesh grid vertices.
%   PXDISPL, PYDISPL, and PZDISPL are the coordinates of the displaced mesh grid
%   vertices.
%
%   See also: imbeziers3, forwarddisp3, cpmesh3, ffdreg3.
%
%JC

%%% Inputs

npixels=numel(I);
if nargin>6 && ~isempty(pretlocalx) && ~isempty(pretlocaly) && ~isempty(pretlocalz) && ...
        any([numel(pretlocalx) numel(pretlocaly) numel(pretlocalz)]~=npixels),
    error('Initial local deformation fields are incorrect size.');
elseif nargin<7,
    pretlocalx=[];
    pretlocaly=[];
    pretlocalz=[];
end

if nargin<10 || isempty(fillwith),
    fillwith=cast(0,class(I));
end

if nargin<20 || isempty(imethod),
    imethod='linear';
end

if nargin>10 && ~isempty(Px) && ~isempty(Py) && ~isempty(Pz), % if undeformed Px,Py,Pz input, identify moving points
    maskeddef=true;
    if nargin==13 || (nargin>=19 && isempty(Pxdispl_last)),
        iteration=false;
        xdispl=abs(Pxdispl-Px);
        ydispl=abs(Pydispl-Py);
        zdispl=abs(Pzdispl-Pz);
    elseif nargin>=19 && ~isempty(Pxdispl_last) && ~isempty(tlocalxlast),
        iteration=true;
        xdispl=abs(Pxdispl_last-Pxdispl);
        ydispl=abs(Pydispl_last-Pydispl);
        zdispl=abs(Pzdispl_last-Pzdispl);
    else
        error('wrong number of input arguments');
    end
    
    moving_thresh=5e-3; % match this in forwarddisp3
    moving_vertex_mask=xdispl>moving_thresh | ydispl>moving_thresh | zdispl>moving_thresh;
    mvinds=find(moving_vertex_mask);
    
    if numel(mvinds)>0,
        [ii,jj,vv]=ind2sub(size(Px),mvinds);
        mvbox=[min(ii) min(jj) min(vv); max(ii) max(jj) max(vv)]+[-2 -2 -2; 2 2 2];
        last_movable_vert=[size(Py,1) size(Px,2) size(Pz,3)]-2;
        mvbox(mvbox<3)=3; % mvbox(1,mvbox(1,:)<3)=3;
        mvbox(mvbox(:,1)>last_movable_vert(1),1)=last_movable_vert(1);
        mvbox(mvbox(:,2)>last_movable_vert(2),2)=last_movable_vert(2);
        mvbox(mvbox(:,3)>last_movable_vert(3),3)=last_movable_vert(3);
        %     Pxdispl=Pxdispl(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Pydispl=Pydispl(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Pzdispl=Pzdispl(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Px_small=Px(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Py_small=Py(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Pz_small=Pz(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     mvimbox=[Py_small(1,1,1) Px_small(1,1,1) Pz_small(1,1,1); Py_small(end,1,1) Px_small(1,end,1) Pz_small(1,1,end)];
        %[Y,X,Z]=ndgrid(Py(mvbox(1,1),1,1):Py(mvbox(2,1),1,1),Px(1,mvbox(1,2),1):Px(1,mvbox(2,2),1),Pz(1,1,mvbox(1,3)):Pz(1,1,mvbox(2,3)));
        %[Y,X]=ndgrid(Py(mvbox(1,1),1,1):Py(mvbox(2,1),1,1),Px(1,mvbox(1,2),1):Px(1,mvbox(2,2),1));
        mvimmask=false(size(I));
        mvimmask(floor(Py(mvbox(1,1),1,1)):ceil(Py(mvbox(2,1),1,1)),floor(Px(1,mvbox(1,2),1)):ceil(Px(1,mvbox(2,2),1)),floor(Pz(1,1,mvbox(1,3))):ceil(Pz(1,1,mvbox(2,3))))=true;
        mviminds=find(mvimmask);
        Bproducts=Bproducts(mviminds,:,:,:);
        tlis=tlis(mviminds,:,:,:);
        nmovingpixels=numel(mviminds);
    else
        maskeddef=false; iteration=false;
        nmovingpixels=npixels;
    end
else
    maskeddef=false; iteration=false;
    nmovingpixels=npixels;
end

%%% local deformation field (tlocalx,tlocaly,tlocalz) from Basis function 
%%% products(Bproducts) and the displaced mesh grid verteces/control points
%%% (Pxdispl, Pydispl, Pzdispl)

if exist('bprodstotlocals3mex','file')
    [tlocalx,tlocaly,tlocalz]=bprodstotlocals3mex(Bproducts,Pxdispl,Pydispl,Pzdispl,tlis);
else
    % Initialize deformation field
    tlocalx=zeros(nmovingpixels,1);
    tlocaly=zeros(nmovingpixels,1);
    tlocalz=zeros(nmovingpixels,1);
    
    % Perform tensor product on all coordinates
    for l=1:4,
        for m=1:4,
            for n=1:4,
                % next control point inds
                % tli=sub2ind(PGridsize,jnow+n-2,inow+m-2);
                % tli=(j+n-2)+(i+m-2-1)*PGridsize(1);
                % tli=tlis(:,m,n);
                
                % Bproduct=Bms(:,m).*Bnt(:,n);
                % Bproduct=Bproducts(:,m,n);
                
                % tlocalx=tlocalx+Bproduct.*Pxdispl(tli);
                % tlocaly=tlocaly+Bproduct.*Pydispl(tli);
                tlocalx=tlocalx+Bproducts(:,m,n,l).*Pxdispl(tlis(:,m,n,l));
                tlocaly=tlocaly+Bproducts(:,m,n,l).*Pydispl(tlis(:,m,n,l));
                tlocalz=tlocalz+Bproducts(:,m,n,l).*Pzdispl(tlis(:,m,n,l));
            end
        end
    end
end

%%% Deform image
% The deformation field (tlocalx,tlocaly) specifies the destination of each
% points of the source image, rather than the origin of each point of the 
% output image. Calling this a *forward* deformation field. If we have the
% mex file ksplat2mex, we obtain the reversed deformation field in
% forwarddisp_testing.m by kernal splatting; if not, forwarddisp.m does a
% linear blending.

if ~maskeddef,
    [Iout,tlocalxnew,tlocalynew,tlocalznew] = forwarddisp3(I,tlocalx, tlocaly, tlocalz, ...
        pretlocalx, pretlocaly, pretlocalz, fillwith, imethod);
    tlocalxlast=[]; tlocalylast=[]; tlocalzlast=[];
else
    if ~iteration,
        [tlocalylast,tlocalxlast,tlocalzlast]=ndgrid(1:size(I,1),1:size(I,2),1:size(I,3));
        tlocalxlast=tlocalxlast(:);
        tlocalylast=tlocalylast(:);
        tlocalzlast=tlocalzlast(:);
    end
    
    tlocalxlast(mviminds)=tlocalx(:);
    tlocalylast(mviminds)=tlocaly(:);
    tlocalzlast(mviminds)=tlocalz(:);
    clear tlocalx tlocaly tlocalz
    [Iout,tlocalxnew,tlocalynew,tlocalznew] = forwarddisp3(I,tlocalxlast, tlocalylast, tlocalzlast, ...
        pretlocalx, pretlocaly, pretlocalz, fillwith, imethod);
end
