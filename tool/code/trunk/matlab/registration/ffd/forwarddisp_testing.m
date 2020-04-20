function [Iout,tlocalx,tlocaly] = forwarddisp_testing(I,tlocalx, tlocaly, pretlocalx, pretlocaly, fillwith,imethod)
% FORWARDDISP Apply a forward displacement field to an image.
%   IOUT = FORWARDDISP(I,TLOCALX,TLOCALY) applies the forward displacement
%   field (i.e. one that specifies the destination of points on the source
%   image, rather than the origin of points on the output image) to image I.
%
%   IOUT = FORWARDDISP(I,TLOCALX,LOCALY,PRETLOCALX,PRETLOCALY) first
%   applies the deformation field in PRETLOCALX and PRETLOCALY prior to
%   applying TLOCALX and LOCALY.
%
%   [IOUT,TLOCALXNEW,LOCALYNEW] = FORWARDDISP(...) returns the combined
%   deformation field that results from applying TLOCALX and LOCALY
%   following PRETLOCALX and PRETLOCALY.  If PRETLOCALX and PRETLOCALY are
%   not input, then TLOCALXNEW,LOCALYNEW == TLOCALX,LOCALY.
%
%JC

if nargin<7,
imethod='linear';
end

if nargin<6,
    fillwith=0;
end

Isize=size(I);
[nrows,ncols,nplanes]=size(I);
tlocalx=reshape(tlocalx,Isize(1:2));
tlocaly=reshape(tlocaly,Isize(1:2));

% Got PRETLOCALX and PRETLOCALY?
if nargin>3 && ~isempty(pretlocalx),
    % Make predef inputs into cells
    if ~iscell(pretlocalx)
        pretlocalx={reshape(pretlocalx,Isize(1:2))};
    end
    if ~iscell(pretlocaly),
        pretlocaly={reshape(pretlocaly,Isize(1:2))};
    end
    if length(pretlocalx) ~= length(pretlocaly),
        error('Predeformations PRETLOCALX and PRETLOCALY are not cells of equal size.');
    end
    
    ndefs=length(pretlocalx);
    for i=1:ndefs,
%         pretlocalx{i}(pretlocalx{i}<1)=1; pretlocalx{i}(pretlocalx{i}>size(I,2))=size(I,2);
%         pretlocaly{i}(pretlocaly{i}<1)=1; pretlocaly{i}(pretlocaly{i}>size(I,1))=size(I,1);
        
        if i<ndefs, % combine the pretlocalx and pretlocaly inputs
            % tlocalxnew=interp2(pretlocalx{i+1},pretlocalx{i},pretlocaly{i},imethod);
            % tlocalynew=interp2(pretlocaly{i+1},pretlocalx{i},pretlocaly{i},imethod);
            if size(pretlocalx{i+1},2)==1,
                pretlocalx{i+1}=reshape(pretlocalx{i+1},Isize(1:2));
                pretlocaly{i+1}=reshape(pretlocaly{i+1},Isize(1:2));
            end
            tlocalxnew=linterp2mex(pretlocalx{i+1},pretlocalx{i},pretlocaly{i},0);
            tlocalynew=linterp2mex(pretlocaly{i+1},pretlocalx{i},pretlocaly{i},0);
            pretlocaly{i+1}=tlocalynew;
            pretlocalx{i+1}=tlocalxnew;
        else % then modify tlocalx and tlocaly
            % tlocalx=interp2(tlocalx,pretlocalx{i},pretlocaly{i},imethod);
            % tlocaly=interp2(tlocaly,pretlocalx{i},pretlocaly{i},imethod);
            tlocalx=linterp2mex(tlocalx,pretlocalx{i},pretlocaly{i},0); % problem here with slightly negative numbers in pretlocalx,t (e.g. -6.6613e-016)
            tlocaly=linterp2mex(tlocaly,pretlocalx{i},pretlocaly{i},0);
        end
        
    end
%     tlocalx=tlocalxnew(:);
%     tlocaly=tlocalynew(:);
end

% Now trasform the actual image
% option to generate larger image
% if (strcmp('full',bbox))
%     yl = floor(min(tlocaly(:))); xl = floor(min(tlocalx(:)));
%     yh = ceil(max(tlocaly(:)));  xh = ceil(max(tlocalx(:)));
%     [X,Y] = meshgrid(xl:xh,yl:yh);
% elseif (strcmp('same',bbox)), % or not
    [X,Y] = meshgrid(1:size(I,2),1:size(I,1));
% else
%     error('illegal value for bbox');
% end;

% Get values at col_samples and row_samples
% dt = DelaunayTri(tlocalx,tlocaly);
% Finterpolant = TriScatteredInterp(dt,I(:),imethod);
% Finterpolant = TriScatteredInterp(tlocalx,tlocaly,I(:),imethod);
% Iout = reshape(Finterpolant(X,Y),size(I));

tlocalx=reshape(tlocalx,size(X));
tlocaly=reshape(tlocaly,size(Y));

outofrangelocs=tlocalx==0;
tlocalx(outofrangelocs)=X(outofrangelocs);
outofrangelocs=tlocaly==0;
tlocaly(outofrangelocs)=Y(outofrangelocs);

Fx=tlocalx-X; Fy=tlocaly-Y;

% Only perform F->B conversion for moving region of volume
moving_point_mask=abs(Fx)>1e-4 | abs(Fy)>1e-4;
mvinds=find(moving_point_mask);
if numel(mvinds)<1,
    Iout=I;
    return;
end
[ii,jj]=ind2sub(size(Fx),mvinds);
mpbox=[min(ii) min(jj); max(ii) max(jj)];
Fx_small=Fx(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2));
Fy_small=Fy(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2));

expandfactor=1.0;
% Hkern=[];
Hkern=fspecial('gaussian',[3 3],.5);
% Hkern=ones(2)/4;
% Hkern=1;
% [Bx,By]=ksplat2(Fx,Fy,Hkern,expandfactor);
[Bx_small,By_small]=ksplat2(Fx_small,Fy_small,Hkern,expandfactor);
Bx=zeros(size(Fx)); By=zeros(size(Fy));
Bx(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2))=Bx_small;
By(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2))=By_small;

% H=fspecial('gaussian',[7 7],5/6);
% Fx=expandfactor*imresize(Fx,expandfactor,'bilinear');
% Fy=expandfactor*imresize(Fy,expandfactor,'bilinear');
% [Bx,By]=ksplat2mex(Fx,Fy,H);
% Bx=(1/expandfactor)*imresize(Bx,1/expandfactor,'bilinear');
% By=(1/expandfactor)*imresize(By,1/expandfactor,'bilinear');
Iout=zeros(Isize,class(I));
for i=1:nplanes,
    if numel(I)>3e6 && exist('linterp2mexmt','file') && ~isempty(strfind(imethod,'linear')),
        Iout(:,:,i)=linterp2mexmt(I(:,:,i),X+Bx,Y+By,fillwith);
    elseif exist('linterp2mex','file') && ~isempty(strfind(imethod,'linear')),
        Iout(:,:,i)=linterp2mex(I(:,:,i),X+Bx,Y+By,fillwith);
    else
        Iout(:,:,i)=interp2(I(:,:,i),X+Bx,Y+By,imethod,cast(fillwith,class(I)));
    end
end

% if any(size(tlocalx)~=size(I)),
%     tlocalx=reshape(tlocalx,size(I));
%     tlocaly=reshape(tlocaly,size(I));
% end
% Iout=griddata(tlocalx,tlocaly,I,X,Y,imethod);
Iout(isnan(Iout))=cast(fillwith,class(Iout));
