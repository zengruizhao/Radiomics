function [Idef,optgriddispl,Pxdispl,Pydispl,Px,Py,MIiters,meshdata,tlocalx,tlocaly] = ffdreg(Itarget,Itemplate,npx,npy,N,nodisp,xcpoptlim,ycpoptlim,pretlocalx,pretlocaly,fillwith)
% FFDREG Free Form Deformation based image registration.
%   ITARGETDEF = FFDREG(ITARGET,ITEMPLATE) performs registration of ITARGET
%   onto ITEMPLATE.  By default, 4-by-4 movable grid vertices are used, and
%   the number of graylevel bins N=256.
%
%   ITARGETDEF = FFDREG(ITARGET,ITEMPLATE,NCPS) uses NCPS-by-NCPS movable
%   mesh grid vertices.
%
%   ITARGETDEF = FFDREG(ITARGET,ITEMPLATE,NCPSX,NCPSX) uses NCPSX-by-NCPSY
%   movable mesh grid vertices.
%
%   ITARGETDEF = FFDREG(ITARGET,ITEMPLATE,NCPSX,NCPSX,N) uses N graylevels
%   in the MI similarity measure histograming algorithm.
%
%   [ITARGETDEF,OPTGRIDDISPL] = FFDREG(...) returns the optimal x and y
%   displacement of the grid formed by the NCPSX-by-NCPSY-by-2 grid vertices.
%
%   [ITARGETDEF,OPTGRIDDISPL,PXDISPL,PYDISPL] = FFDREG(...) returns the
%   full meshgrid displacements, each of size (NCPSX+3)-by-(NCPSY+3). These
%   variables PXDISPL and PYDISPL can be used as inputs to FFDIMMESH.
%
%   [...,PX,PY] = FFDREG(...) returns the unmoved mesh grid vertices.
%   Again, good as inputs to FFDIMMESH.
%
%   [...,MIITERS] = FFDREG(...) returns the values of MI at each iteration
%   in MIITERS.
%
%   [...,MESHDATA] = FFDREG(...) returns the struct MESHDATA, which
%   contains lots of additional information about the mesh.
%
%   [...,TLOCALX,TLOCALY] = FFDREG(...) returns the deformation field
%   components in TLOCALX and TLOCALY.  These can be applied using
%   FORWARDDISP.
%
%   See also: FFDIMMESH, FORWARDDISP.
%
%JC

warning('ffdreg:deprecated','This function is deprecated!  Use ffdreg2 instead.\n');

if any(size(Itarget)~=size(Itemplate)),
    error('Images not the same size.');
end

% target image dimensions
Xmax=size(Itarget,2);
Ymax=size(Itarget,1);

% spacing
if nargin<11,
    fillwith=cast(0,class(Itarget));
    if nargin<10,
        pretlocalx=[];
        pretlocaly=[];
        if nargin<8,
            if nargin<7,
                if nargin<6,
                    nodisp=false;
                    if nargin<5,
                        N=256;
                        fprintf('Using N=256.\n');
                        if nargin<4,
                            if nargin<3,
                                % spx=round(Xmax/6);
                                % spy=round(Ymax/6);
                                npx=4;
                                npy=4;
                            else % nargin=2, isomorphic spacing
                                % spy=spx;
                                npy=npx;
                            end
                        end
                    end
                end
                %npxused=npx;
                %npyused=npy;
                xcpoptlim=[1 npx];
                ycpoptlim=[1 npy];
            else
                % npyused=npxused;
                ycpoptlim=xcpoptlim;
            end
        end
    end
end

if isempty(xcpoptlim),
    xcpoptlim=[1 npx];
    ycpoptlim=[1 npy];
end

if isempty(pretlocalx),
    pre=false;
else
    pre=true;
end

if ~nodisp,
    figure;
end

npxused=xcpoptlim(2)-xcpoptlim(1)+1;
npyused=ycpoptlim(2)-ycpoptlim(1)+1;
npxdiff=npx-npxused;
npydiff=npy-npyused;
% npxbound=npxdiff/2;
% npybound=npydiff/2;

spyx=(([Ymax Xmax]-1)./[npy+1 npx+1]);
% spyx=floor((size(Itarget))./[npy+1 npx+1]);
spx=spyx(2); spy=spyx(1);
display(spyx)

% Generate *padded* mesh for image
% [Px,Py,cpxin,cpyin,PGridsize] = cpmesh([size(Itarget,1) size(Itarget,2)],spx,spy);
cpsx1=linspace(1-spx*2,1,3); cpsx2=linspace(1,size(Itarget,2),npx+2); cpsx3=linspace(size(Itarget,2),size(Itarget,2)+spx*2,3);
cpsx=[cpsx1 cpsx2(2:end-1) cpsx3]; clear cpsx1 cpsx2 cpsx3
cpsy1=linspace(1-spy*2,1,3); cpsy2=linspace(1,size(Itarget,1),npy+2); cpsy3=linspace(size(Itarget,1),size(Itarget,1)+spy*2,3);
cpsy=[cpsy1 cpsy2(2:end-1) cpsy3]; clear cpsy1 cpsy2 cpsy3
% [Px,Py]=meshgrid(linspace(1-spx*2,size(Itarget,2)+spx*2,6+npx),linspace(1-spy*2,size(Itarget,1)+spy*2,6+npy)); % -1 added to end because of machineprecision problems
[Px,Py]=meshgrid(cpsx,cpsy);
PGridsize=size(Px);
cpxin=Px(1,Px(1,:)>0 & Px(1,:)<=size(Itarget,2));
cpyin=Py(Py(:,1)>0 & Py(:,1)<=size(Itarget,1),1)';
% number of control points in each dimension
ncpsy=PGridsize(1); ncpsx=PGridsize(2);
% 
xcpstart=Px(1); ycpstart=Py(1);

meshdata.Px=Px;
meshdata.Py=Py;
meshdata.cpxin=cpxin;
meshdata.cpyin=cpyin;
meshdata.PGridsize=PGridsize;
meshdata.ncpsy=ncpsy;
meshdata.ncpsx=ncpsx;
meshdata.ycpstart=ycpstart;
meshdata.xcpstart=xcpstart;
meshdata.spx=spx;
meshdata.spy=spy;

display(cpxin), display(Px)
display(cpyin), display(Py)

% Deformations
global err_record dispbool movingylocs movingxlocs %fig hi
dispbool=nodisp;

% Grab the interior control points (not the outside ones or the ones on the boundaries)
% initgriddispl=zeros((numel(cpxin)-2)*(numel(cpyin)-2),2);
initgriddispl=zeros(numel(cpyin)-2-npydiff,numel(cpxin)-2-npxdiff,2); % initial displacment of moving points only
% gridbuffer=(size(Px)-[size(initgriddispl,1) size(initgriddispl,2)])./2; % [y,x] buffer of points outside of image
gridbuffer=(size(Px)-[npy npx])./2; 
imgylocs=gridbuffer(1):ncpsy-gridbuffer(1)+1; % indexes of points inside image or on boundary, but not necessarily moving
imgxlocs=gridbuffer(2):ncpsx-gridbuffer(2)+1;

% movingylocs=gridbuffer(1)+1+floor(npybound):ncpsy-gridbuffer(1)-ceil(npybound); % gridbuffer(1)+1:ncpsy-gridbuffer(1); % indexes of moving points
% movingxlocs=gridbuffer(2)+1+floor(npxbound):ncpsx-gridbuffer(2)-ceil(npxbound); % gridbuffer(2)+1:ncpsx-gridbuffer(2); % indexes of moving points
movingylocs=gridbuffer(1)+ycpoptlim(1):ycpoptlim(2)+gridbuffer(1);
movingxlocs=gridbuffer(2)+xcpoptlim(1):xcpoptlim(2)+gridbuffer(2);

% copy values into meshdata
meshdata.gridbuffer=gridbuffer;
meshdata.initgriddispl=initgriddispl;
meshdata.imgylocs=imgylocs;
meshdata.imgxlocs=imgxlocs;
meshdata.movingylocs=movingylocs;
meshdata.movingxlocs=movingxlocs;

% Coordinates of Px,Py that are moving
optptsX=Px(movingylocs,movingxlocs); %Px(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
optptsY=Py(movingylocs,movingxlocs); %Py(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
meshdata.optptsX=optptsX;
meshdata.optptsY=optptsY;
display(optptsX), display(optptsY)

Itarget=round(Itarget);

err_record=0;

% fig=figure;
% set(fig,'DoubleBuffer','on');
% mov=avifile('ffdREG.avi','Compression','None','Quality',100);
% hi=imdisp(Itarget); drawnow
% set(hi,'EraseMode','xor');
% F = getframe(gca);
% graymap=gray(N);
% F=im2frame(round(rescale(Itarget)*255)+1,graymap);
% F=im2frame(Itarget+1,graymap);
% mov = addframe(mov,F);
        
% N=256;
options = optimset('Display','iter','MaxIter',700);
meshdata.optgriddisplparams = fminsearch(@ffdobjfunc,initgriddispl,options, ... 
    Itarget, Itemplate, Px, Py, N, spx, spy,pretlocalx,pretlocaly,fillwith);

% mov = close(mov);

optgriddispl=meshdata.optgriddisplparams;
% optgriddispl(:,1)=optgriddispl(:,1)*spx*700;
% optgriddispl(:,2)=optgriddispl(:,2)*spy*700;
optgriddispl(:,:,1)=optgriddispl(:,:,1)*(spx^1.1)*600;
optgriddispl(:,:,2)=optgriddispl(:,:,2)*(spy^1.1)*600;

% npoints=size(optgriddispl,1); boxsize=sqrt(npoints);
xgriddispl=zeros(size(Px)); ygriddispl=xgriddispl;
xgriddisplparams=zeros(size(Px)); ygriddisplparams=xgriddispl;
xgriddispl(movingylocs,movingxlocs)=optgriddispl(:,:,1); %xgriddispl(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2))=optgriddispl(:,:,1);
ygriddispl(movingylocs,movingxlocs)=optgriddispl(:,:,2); %ygriddispl(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2))=optgriddispl(:,:,2);
xgriddisplparams(movingylocs,movingxlocs)=meshdata.optgriddisplparams(:,:,1);%xgriddisplparams(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2))=meshdata.optgriddisplparams(:,:,1);
ygriddisplparams(movingylocs,movingxlocs)=meshdata.optgriddisplparams(:,:,2); %ygriddisplparams(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2))=meshdata.optgriddisplparams(:,:,2);

Pxdispl=Px+xgriddispl;
Pydispl=Py+ygriddispl;

meshdata.xgriddispl=xgriddispl;
meshdata.ygriddispl=ygriddispl;
meshdata.xgriddisplparams=xgriddisplparams;
meshdata.ygriddisplparams=ygriddisplparams;
meshdata.Pxdispl=Pxdispl;
meshdata.Pydispl=Pydispl;

[Idef,junk,junk,junk,junk,tlocalx,tlocaly] = FFDimmesh(Itarget,Px,Py,Pxdispl,Pydispl); %#ok<*ASGLU>
Idef=round(Idef);

if pre,
[Idefjunk,tlocalx,tlocaly]=forwarddisp(Itarget,tlocalx,tlocaly,pretlocalx,pretlocaly);
end

MIiters=-err_record(2:end);
    clear global err_record

function err = ffdobjfunc(griddispl,Itarget,Itemplate,Px,Py,N,spx,spy,pretlocalx,pretlocaly,fillwith)

global err_record dispbool movingylocs movingxlocs %fig hi

% griddispl(:,1)=griddispl(:,1)*spx*700;
% griddispl(:,2)=griddispl(:,2)*spy*700;
griddispl(:,:,1)=griddispl(:,:,1)*(spx^1.1)*600;
griddispl(:,:,2)=griddispl(:,:,2)*(spy^1.1)*600;

% display(round(griddispl))
% nparams=numel(griddispl);

% npoints=size(griddispl,1);
% boxsize=sqrt(npoints);
% npoints=numel(griddispl(:,:,1));

xgriddispl=zeros(size(Px));
ygriddispl=xgriddispl;
% gridbuffer=(size(Px)-[size(griddispl,1) size(griddispl,2)])./2;
% xgriddispl(4:end-3,4:end-3)=reshape(griddispl(:,1),boxsize,boxsize);
% ygriddispl(4:end-3,4:end-3)=reshape(griddispl(:,2),boxsize,boxsize);
% xgriddispl(4:end-3,4:end-3)=griddispl(:,:,1);
% ygriddispl(4:end-3,4:end-3)=griddispl(:,:,2);
% xgriddispl(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2))=griddispl(:,:,1);
% ygriddispl(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2))=griddispl(:,:,2);
xgriddispl(movingylocs,movingxlocs)=griddispl(:,:,1);
ygriddispl(movingylocs,movingxlocs)=griddispl(:,:,2);


Iout = FFDimmesh(Itarget,Px,Py,Px+xgriddispl,Py+ygriddispl,pretlocalx,pretlocaly,fillwith);
Iout=round(Iout);
if ~dispbool
    cla; hold off;
    hi=imdisp(rescale(Iout)); hold on %#ok<NASGU>
    Pxdispl=Px+xgriddispl; Pydispl=Py+ygriddispl;
    plot(Pxdispl(:),Pydispl(:),'r.')
    plot(Pxdispl,Pydispl,'r'); plot(Pxdispl',Pydispl','r')
    xlim([0 size(Iout,2)]); ylim([0 size(Iout,1)])
    drawnow
end
% set(hi,'EraseMode','xor');
% F = getframe(gca);
% F=im2frame(round(rescale(Iout)*255)+1,graymap);
% F=im2frame(Iout+1,graymap);
% mov = addframe(mov,F);
% imdisp(uint8(round(Iout))); drawnow

[MIval,h12,h1,h2,nmi,ecc]=MI(Iout,Itemplate,N); %#ok<NASGU>

err=-nmi;
% err=-MIval;
err_record(end+1)=err;
