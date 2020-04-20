function [Iout,tlocalxnew,tlocalynew,tlocalxlast,tlocalylast] = imffd(I,Bproducts,tlis,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith,Px,Py,Pxdispl_last,Pydispl_last,tlocalxlast,tlocalylast)
% IMFFD Apply Free Form Deformations specified by grid displacemnets to image I.
%  Iout = IMFFD(I,Bproducts,tlis,Pxdispl,Pydispl)
%
%   I is an image to be deformed.
%   Iout is the deformed image.
%   BPRODUCTS are the Bezier products calculated at every pixel.
%   TLIS are indexes into the mesh grid at each pixel and for each m and n.
%   BPRODUCTS and TLIS are calculated via imbeziers.m from PX and PY, the
%   coordinates of the initial mesh grid vertices.
%   PXDISPL and PYDISPL are the coordinates of the displaced mesh grid
%   vertices.
%
%   See also: imbeziers, forwarddisp, forwardisp_testing.
%
%JC

%%% Inputs

[nrows,ncols,nplanes]=size(I); % nplanes includes all higher dimensions
npixels=nrows*ncols;
if nargin>6 && ~isempty(pretlocalx) && ~isempty(pretlocaly) && ...
        any([numel(pretlocalx) numel(pretlocaly)]~=npixels),
    error('Initial local deformation fields are incorrect size.');
elseif nargin<7,
    pretlocalx=[];
    pretlocaly=[];
end

if nargin<8 || isempty(fillwith),
    fillwith=cast(0,class(I));
end

% Calculate deformation only near moving points OR differential deformation
got_base_args=nargin>=10 && ~isempty(Px) && ~isempty(Py);
got_diff_args=nargin==14 && ~isempty(Pxdispl_last) && ~isempty(Pydispl_last) ...
    && ~isempty(tlocalxlast) && ~isempty(tlocalylast) ...
    && got_base_args;
if got_diff_args  || got_base_args,
    % if undeformed Px,Py input, identify moving points
    maskeddef=true;
    
    if got_diff_args,
        iteration=true;
        xdispl=abs(Pxdispl_last-Pxdispl);
        ydispl=abs(Pydispl_last-Pydispl);
    elseif got_base_args,
        iteration=false;
        xdispl=abs(Pxdispl-Px);
        ydispl=abs(Pydispl-Py);
    else
        error('wrong number of input arguments');
    end
    
    moving_thresh=1e-6;
    moving_vertex_mask=xdispl>moving_thresh | ydispl>moving_thresh;
    mvinds=find(moving_vertex_mask);
    
    if numel(mvinds)>0,
        [ii,jj]=ind2sub(size(Px),mvinds);
        mvbox=[min(ii) min(jj); max(ii) max(jj)]+[-2 -2; 2 2];
        last_movable_vert=[size(Py,1) size(Px,2)]-2;
        mvbox(mvbox<3)=3; % mvbox(1,mvbox(1,:)<3)=3;
        mvbox(mvbox(:,1)>last_movable_vert(1),1)=last_movable_vert(1);
        mvbox(mvbox(:,2)>last_movable_vert(2),2)=last_movable_vert(2);
        % mvbox(2,mvbox(2,:)>last_movable_vert)=last_movable_vert(mvbox(2,:)>last_movable_vert);
        %     influence_box=[min(ii) min(jj); max(ii) max(jj)]+[-4 -4; 4 4];
        % Points in influence box are accounted for since the entire Pxdispl
        % and Pydispl are passed to bprodstotlocals2mex and referenced by tlis
        
        %     Pxdispl=Pxdispl(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Pydispl=Pydispl(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Px_small=Px(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     Py_small=Py(mvbox(1,1):mvbox(2,1),mvbox(1,2):mvbox(2,2),mvbox(1,3):mvbox(2,3));
        %     mvimbox=[Py_small(1,1,1) Px_small(1,1,1); Py_small(end,1,1) Px_small(1,end,1)];
        %[Y,X]=ndgrid(Py(mvbox(1,1),1,1):Py(mvbox(2,1),1,1),Px(1,mvbox(1,2),1):Px(1,mvbox(2,2),1));
        mvimmask=false(size(I));
        mvimmask(floor(Py(mvbox(1,1),1)):ceil(Py(mvbox(2,1),1)),floor(Px(1,mvbox(1,2))):ceil(Px(1,mvbox(2,2))))=true;
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

%%% local deformation field (tlocalx,tlocaly) from Bezier products
%%% (Bproducts) and the displaced mesh grid verteces (Pxdispl,Pydispl)

if exist('bprodstotlocals2mex','file')
    [tlocalx,tlocaly]=bprodstotlocals2mex(Bproducts,Pxdispl,Pydispl,tlis);
else
    % Initialize deformation field
    tlocalx=zeros(nmovingpixels,1);
    tlocaly=zeros(nmovingpixels,1);
    
    % Perform tensor product on all coordinates
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
            tlocalx=tlocalx+Bproducts(:,m,n).*Pxdispl(tlis(:,m,n));
            tlocaly=tlocaly+Bproducts(:,m,n).*Pydispl(tlis(:,m,n));
        end
    end
end

% deal with precision issues
if maskeddef,
    [Y,X]=ndgrid(floor(Py(mvbox(1,1),1)):ceil(Py(mvbox(2,1),1)),floor(Px(1,mvbox(1,2))):ceil(Px(1,mvbox(2,2))));
else
    [Y,X]=ndgrid(1:size(I,1),1:size(I,2));
end
X=X(:); Y=Y(:);
practically_zero=abs(tlocalx-X)<1e-5;
tlocalx(practically_zero)=X(practically_zero);
practically_zero=abs(tlocaly-Y)<1e-5;
tlocaly(practically_zero)=Y(practically_zero);

%%% Deform image
% The deformation field (tlocalx,tlocaly) specifies the destination of each
% points of the source image, rather than the origin of each point of the 
% output image. Calling this a *forward* deformation field. If we have the
% mex file ksplat2mex, we obtain the reversed deformation field in
% forwarddisp_testing.m by kernal splatting; if not, forwarddisp.m does a
% linear blending.

if exist('ksplat2mex','file'),
    forwarddispfun=@forwarddisp_testing;
else
    forwarddispfun=@forwarddisp;
end

if ~maskeddef,
	[Iout,tlocalxnew,tlocalynew] = forwarddispfun(I,tlocalx, tlocaly, pretlocalx, pretlocaly,fillwith); % now using ksplat2mex!
    tlocalxlast=[]; tlocalylast=[];
else
    if ~iteration, % if not iterating, not using a previous/last deformation
        [tlocalylast,tlocalxlast]=ndgrid(1:size(I,1),1:size(I,2));
        tlocalxlast=tlocalxlast(:);
        tlocalylast=tlocalylast(:);
    end
    
    tlocalxlast(mviminds)=tlocalx(:);
    tlocalylast(mviminds)=tlocaly(:);
    clear tlocalx tlocaly
    [Iout,tlocalxnew,tlocalynew] = forwarddispfun(I,tlocalxlast, tlocalylast, ...
        pretlocalx, pretlocaly, fillwith);
end
% tlocalxlast and tlocalylast do not include pretlocalx and pretlocaly
