function [Iout,tlocalx,tlocaly,tlocalz,Bx,By,Bz] = forwarddisp3(I,tlocalx, tlocaly, tlocalz, pretlocalx, pretlocaly, pretlocalz, fillwith,imethod)
% FORWARDDISP3 Apply a forward displacement field to an 3D image/volume.
%   IOUT = FORWARDDISP3(I,TLOCALX,TLOCALY,TLOCALZ) applies the forward 
%   displacement field (i.e. one that specifies the destinations for points 
%   on the source image, rather than the origin of points on the output 
%   image) to 3D volume I.
%
%   IOUT = FORWARDDISP3(...,PRETLOCALX,PRETLOCALY,PRETLOCALZ) first
%   applies the deformation field in PRETLOCALX, PRETLOCALY and PRETLOCALZ
%   prior to applying TLOCALX, TLOCALY and TLOCALZ.
%
%   [IOUT,TLOCALXNEW,TLOCALZNEW,TLOCALZNEW] = FORWARDDISP3(...) returns the
%   combined deformation field that results from applying TLOCAL* following
%   PRETLOCAL*. If PRETLOCAL* variables are NOT input, then each 
%   TLOCAL[X,Y,Z]NEW == TLOCAL[X,Y,Z].
%
%   See also: imffd3, imbeziers3, cpmesh3, ksplat3, ffdreg3.
%
%JC

if nargin<9 || isempty(imethod),
imethod='linear';
end

if nargin<8 || isempty(fillwith),
    fillwith=0;
end

if numel(I)==3, % option to input a volume size instead of the volume
    Isize=I;
else
    Isize=size(I);
end
tlocalx=reshape(tlocalx,Isize);
tlocaly=reshape(tlocaly,Isize);
tlocalz=reshape(tlocalz,Isize);

% Got PRETLOCALX and PRETLOCALY?
if nargin>=5 && ~isempty(pretlocalx),
    % Make predef inputs into cells
    if ~iscell(pretlocalx)
        pretlocalx={reshape(pretlocalx,Isize)};
    end
    if ~iscell(pretlocaly),
        pretlocaly={reshape(pretlocaly,Isize)};
    end
    if ~iscell(pretlocalz),
        pretlocalz={reshape(pretlocalz,Isize)};
    end
    if length(pretlocalx) ~= length(pretlocaly) ~= length(pretlocalz),
        error('Predeformations PRETLOCALX, PRETLOCALY and PRETLOCALZ are not cells of equal size.');
    end
    
    ndefs=length(pretlocalx);
    for i=1:ndefs,
%         pretlocalx{i}(pretlocalx{i}<1)=1; pretlocalx{i}(pretlocalx{i}>size(I,2))=size(I,2);
%         pretlocaly{i}(pretlocaly{i}<1)=1; pretlocaly{i}(pretlocaly{i}>size(I,1))=size(I,1);
        
        if i<ndefs, % combine the pretlocalx and pretlocaly inputs
            if size(pretlocalx{i+1},2)==1,
                pretlocalx{i+1}=reshape(pretlocalx{i+1},Isize);
                pretlocaly{i+1}=reshape(pretlocaly{i+1},Isize);
                pretlocalz{i+1}=reshape(pretlocalz{i+1},Isize);
            end
            tlocalxnew=linterp3mex(pretlocalx{i+1},pretlocalx{i},pretlocaly{i},pretlocalz{i},double(fillwith));
            tlocalynew=linterp3mex(pretlocaly{i+1},pretlocalx{i},pretlocaly{i},pretlocalz{i},double(fillwith));
            tlocalznew=linterp3mex(pretlocalz{i+1},pretlocalx{i},pretlocaly{i},pretlocalz{i},double(fillwith));
            pretlocaly{i+1}=tlocalynew;
            pretlocalx{i+1}=tlocalxnew;
            pretlocalz{i+1}=tlocalznew;
        else % then modify tlocalx and tlocaly
            % tlocalx=interp3(tlocalx,pretlocalx{i},pretlocaly{i},pretlocalz{i},imethod);
            % tlocaly=interp3(tlocaly,pretlocalx{i},pretlocaly{i},pretlocalz{i},imethod);
            % tlocalz=interp3(tlocalz,pretlocalx{i},pretlocaly{i},pretlocalz{i},imethod);
            tlocalx=linterp3mex(tlocalx,pretlocalx{i},pretlocaly{i},pretlocalz{i},double(fillwith));
            tlocaly=linterp3mex(tlocaly,pretlocalx{i},pretlocaly{i},pretlocalz{i},double(fillwith));
            tlocalz=linterp3mex(tlocalz,pretlocalx{i},pretlocaly{i},pretlocalz{i},double(fillwith));
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
    [Y, X, Z] = ndgrid(1:Isize(1),1:Isize(2),1:Isize(3));
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
tlocalz=reshape(tlocalz,size(Y));
Fx=tlocalx-X; Fy=tlocaly-Y; Fz=tlocalz-Z;

% Only perform F->B conversion for moving region of volume
moving_thresh=5e-3; % match this in imffd3
moving_point_mask=abs(Fx)>moving_thresh | abs(Fy)>moving_thresh | abs(Fz)>moving_thresh;
mvinds=find(moving_point_mask);
if numel(mvinds)<1,
    Iout=I;
    return;
end
[ii,jj,vv]=ind2sub(size(Fx),find(moving_point_mask));
mpbox=[min(ii) min(jj) min(vv); max(ii) max(jj) max(vv)];
Fx_small=Fx(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2),mpbox(1,3):mpbox(2,3));
Fy_small=Fy(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2),mpbox(1,3):mpbox(2,3));
Fz_small=Fz(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2),mpbox(1,3):mpbox(2,3));

expandfactor=1.0;
% Hkern=[];
% Hkern=fspecial('gaussian',[3 3],.5);
Hkern=gauss3([3/4 3/4 1/3],[5 5 3]);
% Hkern=gauss3([.5 .5 .25],[3 3 3]);
% Hkern=ones(2)/4;
% Hkern=1;

% [Bx,By,Bz]=ksplat3(Fx,Fy,Fz,Hkern,expandfactor);
[Bx_small,By_small,Bz_small]=ksplat3(Fx_small,Fy_small,Fz_small,Hkern,expandfactor);
Bx=zeros(size(Fx)); By=zeros(size(Fy)); Bz=zeros(size(Fz));
Bx(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2),mpbox(1,3):mpbox(2,3))=Bx_small;
By(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2),mpbox(1,3):mpbox(2,3))=By_small;
Bz(mpbox(1,1):mpbox(2,1),mpbox(1,2):mpbox(2,2),mpbox(1,3):mpbox(2,3))=Bz_small;

if numel(I)>3,
    if exist('linterp3mexmt','file') && strcmpi(imethod,'linear'), % ~isempty(strfind(imethod,'linear')),
        Iout=linterp3mexmt(I,X+Bx,Y+By,Z+Bz,fillwith);
    elseif exist('linterp3mex','file') && strcmpi(imethod,'linear'), % ~isempty(strfind(imethod,'linear')),
        Iout=linterp3mex(I,X+Bx,Y+By,Z+Bz,fillwith);
    else
        Iout=interp3(I,X+Bx,Y+By,Z+Bz,imethod,cast(fillwith,class(I)));
    end
    
    % if any(size(tlocalx)~=size(I)),
    %     tlocalx=reshape(tlocalx,size(I));
    %     tlocaly=reshape(tlocaly,size(I));
    % end
    % Iout=griddata(tlocalx,tlocaly,I,X,Y,imethod);
    Iout(isnan(Iout))=cast(fillwith,class(Iout));
else
    Iout=[];
end
