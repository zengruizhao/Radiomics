function [Idef,optgriddispl,Pxdispl,Pydispl,Pzdispl,Px,Py,Pz,MIiters,meshdata,tlocalx,tlocaly,tlocalz] = ffdreg3(varargin)
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

%%% INPUTS

% Parse inputs
ip=inputParser;
ip.addRequired('Itarget', @isnumeric);
ip.addRequired('Itemplate', @isnumeric);
npdefault=4;
ip.addOptional('npx', npdefault, @(x)isnumeric(x) && numel(x)==1);
ip.addOptional('npy', npdefault, @(x)isnumeric(x) && numel(x)==1); % default needs to be set later
ip.addOptional('npz', npdefault, @(x)isnumeric(x) && numel(x)==1); % default needs to be set later
ip.addOptional('N', 256, @(x)isnumeric(x) && numel(x)==1);
measures={'alphami','MI','NMI','ECC','JE','EOH'};
ip.addParamValue('measure', 'MI', @(x)any(strcmpi(x,measures)));
imethods={'linear','nearest','cubic','*linear','*nearest','*cubic'};
ip.addParamValue('imethod', 'linear', @(x)any(strcmpi(x,imethods)));
ip.addParamValue('quiet', false, @(x)islogical(x) || x==0 || x==1);
ip.addParamValue('fillwith',0,@(x)isnumeric(x) && numel(x)==1);
% FFD parameters
ip.addParamValue('xcpoptlim',[],@(x)isnumeric(x) && numel(x)==2); % default needs to be set later
ip.addParamValue('ycpoptlim',[],@(x)isnumeric(x) && numel(x)==2); % default needs to be set later
ip.addParamValue('zcpoptlim',[],@(x)isnumeric(x) && numel(x)==2); % default needs to be set later
ip.addParamValue('pretlocalx',[],@isnumeric);
ip.addParamValue('pretlocaly',[],@isnumeric);
ip.addParamValue('pretlocalz',[],@isnumeric);
ip.addParamValue('moveedgenodes', false, @(x)islogical(x) || x==0 || x==1);
% alpha-MI parameters
ip.addParamValue('alpha',0.98,@(x)isnumeric(x) && numel(x)==1);
ip.addParamValue('k',20,@(x)isnumeric(x) && numel(x)==1);
ip.addParamValue('ptinc',1,@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('nowarnknn', false, @(x)islogical(x) || x==0 || x==1);
ip.addParamValue('knnthreads',6,@(x)isnumeric(x) && numel(x)==1 && x>0);
% optimset parameters
ip.addParamValue('tolx',[],@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('tolfun',[],@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('maxiter',12000,@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('maxfunevals',[],@(x)isnumeric(x) && numel(x)==1 && x>0);

ip.parse(varargin{:});

% (Itarget, Itemplate)
Itarget=ip.Results.Itarget;
Itemplate=ip.Results.Itemplate;

if any([size(Itemplate,1) size(Itemplate,2) size(Itemplate,3)]~=[size(Itarget,1) size(Itarget,2) size(Itarget,3)]),
    error('Images not the same size. Attributes should be on dimension 4.');
end

Xmax=size(Itarget,2);
Ymax=size(Itarget,1);
Zmax=size(Itarget,3);
Isize=[Ymax Xmax Zmax];

% (fillwith, quiet, N, measure)
fillwith=cast(ip.Results.fillwith,class(Itarget));
quiet=logical(ip.Results.quiet);
N=ip.Results.N;
measure=ip.Results.measure;
imethod=ip.Results.imethod;

% (npx,npy) - Number of interior control points that define the grid size
npx=ip.Results.npx;
npz=ip.Results.npz;
if any(strcmpi('npy',ip.UsingDefaults)) && ~any(strcmpi('npx',ip.UsingDefaults)),
    npy=npx; % npy WAS NOT input AND npx WAS input
else % either npx WAS NOT input OR npy WAS input
    npy=ip.Results.npy;
end

% (xoptlim, yoptlim) - Moving control points that define the region of optimization/deformation
if any(strcmpi('ycpoptlim',ip.UsingDefaults)) || isempty(ip.Results.ycpoptlim),
    if any(strcmpi('xcpoptlim',ip.UsingDefaults)) || isempty(ip.Results.xcpoptlim),
        xcpoptlim=[1 npx];
        ycpoptlim=[1 npy];
        zcpoptlim=[1 npz];
    else
        xcpoptlim=ip.Results.xcpoptlim;
        ycpoptlim=xcpoptlim;
        zcpoptlim=xcpoptlim;
    end
else
    xcpoptlim=ip.Results.xcpoptlim;
    ycpoptlim=ip.Results.ycpoptlim;
    zcpoptlim=ip.Results.zcpoptlim;
end

% (pretlocalx, pretlocaly)
pretlocalx=ip.Results.pretlocalx;
pretlocaly=ip.Results.pretlocaly;
pretlocalz=ip.Results.pretlocalz;
% if isempty(pretlocalx), pre=false;
% else pre=true; end


%%% Mesh Grid
moveedgenodes=ip.Results.moveedgenodes;
% Used control points
npxused=xcpoptlim(2)-xcpoptlim(1)+1;
npyused=ycpoptlim(2)-ycpoptlim(1)+1;
npzused=zcpoptlim(2)-zcpoptlim(1)+1;
npxdiff=npx-npxused; % unused
npydiff=npy-npyused;
npzdiff=npz-npzused;

% Control point spacing
if moveedgenodes,
    spyxz=(([Ymax Xmax Zmax]-1)./[npy-1 npx-1 npz-1]);
else
    spyxz=(([Ymax Xmax Zmax]-1)./[npy+1 npx+1 npz+1]);
end
spx=spyxz(2); spy=spyxz(1); spz=spyxz(3);
display(spyxz)

% Generate *padded* mesh for image
gp=2; % padding on each side, units are blocks
% [Px,Py,cpxin,cpyin,PGridsize] = cpmesh([size(Itarget,1) size(Itarget,2)],spx,spy);
cpsx1=linspace(1-spx*gp,1,3);
cpsy1=linspace(1-spy*gp,1,3);
cpsz1=linspace(1-spz*gp,1,3);
cpsx2=linspace(1,size(Itarget,2),npx+2*~moveedgenodes);
cpsy2=linspace(1,size(Itarget,1),npy+2*~moveedgenodes);
cpsz2=linspace(1,size(Itarget,3),npz+2*~moveedgenodes);
cpsy3=linspace(size(Itarget,1),size(Itarget,1)+spy*gp,3);
cpsx3=linspace(size(Itarget,2),size(Itarget,2)+spx*gp,3);
cpsz3=linspace(size(Itarget,3),size(Itarget,3)+spz*gp,3);
cpsx=[cpsx1 cpsx2(2:end-1) cpsx3];
cpsy=[cpsy1 cpsy2(2:end-1) cpsy3];
cpsz=[cpsz1 cpsz2(2:end-1) cpsz3];
clear cpsx1 cpsx2 cpsx3 cpsy1 cpsy2 cpsy3 cpsz1 cpsz2 cpsz3
[Px,Py,Pz]=meshgrid(cpsx,cpsy,cpsz);
PGridsize=size(Px);
cpxin=Px(1,Px(1,:,1)>0 & Px(1,:,1)<=size(Itarget,2));
cpyin=Py(Py(:,1,1)>0 & Py(:,1,1)<=size(Itarget,1),1)';
cpzin=Pz(1,1,Pz(1,1,:)>0 & Pz(1,1,:)<=size(Itarget,3)); cpzin=cpzin(:)';  % right?
% number of control points in each dimension - includes padding and edges
ncpsy=PGridsize(1); ncpsx=PGridsize(2); ncpsz=PGridsize(3);
% 
xcpstart=Px(1); ycpstart=Py(1); zcpstart=Pz(1);

% structure for mesh grid information
meshdata.Px=Px;
meshdata.Py=Py;
meshdata.Pz=Pz;
meshdata.cpxin=cpxin;
meshdata.cpyin=cpyin;
meshdata.cpzin=cpzin;
meshdata.PGridsize=PGridsize;
meshdata.ncpsx=ncpsx;
meshdata.ncpsy=ncpsy;
meshdata.ncpsz=ncpsz;
meshdata.xcpstart=xcpstart;
meshdata.ycpstart=ycpstart;
meshdata.zcpstart=zcpstart;
meshdata.spx=spx;
meshdata.spy=spy;
meshdata.spz=spz;

display(cpxin), display(Px)
display(cpyin), display(Py)
display(cpzin), display(Pz)

%%% Registration

% Grab the interior control points (not the outside ones or the ones on the boundaries)
% initgriddispl=zeros((numel(cpxin)-2)*(numel(cpyin)-2),2);
if moveedgenodes,
    initgriddispl=zeros(numel(cpyin)-npydiff, numel(cpxin)-npxdiff, numel(cpzin)-npzdiff, 3); % initial displacment of moving points only
else
    initgriddispl=zeros(numel(cpyin)-2-npydiff, numel(cpxin)-2-npxdiff, numel(cpzin)-2-npzdiff, 3); % initial displacment of moving points only
end
gridbuffer=(size(Px)-[npy npx npz])./2;  % [y,x,z] buffer of points outside of image
imgylocs=gridbuffer(1):ncpsy-gridbuffer(1)+1; % indexes of points inside image or on boundary, but not necessarily moving
imgxlocs=gridbuffer(2):ncpsx-gridbuffer(2)+1;
imgzlocs=gridbuffer(3):ncpsx-gridbuffer(3)+1;

movingylocs=gridbuffer(1)+ycpoptlim(1):ycpoptlim(2)+gridbuffer(1);
movingxlocs=gridbuffer(2)+xcpoptlim(1):xcpoptlim(2)+gridbuffer(2);
movingzlocs=gridbuffer(3)+zcpoptlim(1):zcpoptlim(2)+gridbuffer(3);

% copy values into meshdata
meshdata.gridbuffer=gridbuffer;
meshdata.initgriddispl=initgriddispl;
meshdata.imgxlocs=imgxlocs;
meshdata.imgylocs=imgylocs;
meshdata.imgzlocs=imgzlocs;
meshdata.movingxlocs=movingxlocs;
meshdata.movingylocs=movingylocs;
meshdata.movingzlocs=movingzlocs;

% Coordinates of Px,Py that are moving
optptsX=Px(movingylocs,movingxlocs,movingzlocs); %Px(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
optptsY=Py(movingylocs,movingxlocs,movingzlocs); %Py(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
optptsZ=Pz(movingylocs,movingxlocs,movingzlocs); %Py(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
meshdata.optptsX=optptsX;
meshdata.optptsY=optptsY;
meshdata.optptsZ=optptsZ;
display(optptsX), display(optptsY), display(optptsZ)

if ~quiet,
    figure;
end

% Optimize deformation
% options = optimset('Display','iter','MaxIter',700);
maxiter=ip.Results.maxiter;
tolfun=ip.Results.tolfun;
tolx=ip.Results.tolx;
maxfunevals=ip.Results.maxfunevals;
if isempty(tolx),
    pixeltransmin=4;
    tolx=pixeltransmin/(3200*spx^.7); %tolx=pixeltransmin/(1500*spx^.77);
end
options = optimset('Display','iter','MaxIter',maxiter,'TolX',tolx,'TolFun',tolfun,'MaxFunEvals',maxfunevals);
% meshdata.optgriddisplparams = fminsearch(@ffdobjfunc,initgriddispl,options, ... 
%     Itarget, Itemplate, Px, Py, N, spx, spy,pretlocalx,pretlocaly,fillwith,...
%     quiet,movingxlocs,movingylocs,measure);
fprintf('Precalculating Bezier products...\n');
[Bproducts,tlis] = imbeziers3(Isize,Px,Py,Pz);
ffdobjfunc;
alphamiparams.alpha=ip.Results.alpha;
alphamiparams.k=ip.Results.k;
alphamiparams.ptinc=ip.Results.ptinc;
alphamiparams.nowarn=ip.Results.nowarnknn;
alphamiparams.knnthreads=ip.Results.knnthreads;
fprintf('Beginning registration.\n');
meshdata.optgriddisplparams = fminsearch(@ffdobjfunc,initgriddispl,options, ... 
    Itarget, Itemplate, Px, Py, Pz, N, spx, spy, spz, pretlocalx,pretlocaly,pretlocalz,fillwith,...
    quiet,movingxlocs,movingylocs,movingzlocs,measure,Bproducts,tlis,alphamiparams,imethod);
%     quiet,movingxlocs,movingylocs,movingzlocs,measure,Bproducts,tlis,ip.Results.alpha,ip.Results.k);
    
% Grab error record
err_record=ffdobjfunc;
MIiters=-err_record(2:end);

% Clear persistent variables in objective function
clear ffdobjfunc

% Apply determined transformation to image - EXACTLY as in ffdobjfuc!
optgriddispl=meshdata.optgriddisplparams;
% optgriddispl(:,1)=optgriddispl(:,1)*spx*700;
% optgriddispl(:,2)=optgriddispl(:,2)*spy*700;
% optgriddispl(:,:,1)=optgriddispl(:,:,1)*(spx^1.1)*600;
% optgriddispl(:,:,2)=optgriddispl(:,:,2)*(spy^1.1)*600;
optgriddispl(:,:,:,1)=optgriddispl(:,:,:,1)*(spx^.7)*3200;
optgriddispl(:,:,:,2)=optgriddispl(:,:,:,2)*(spy^.7)*3200;
optgriddispl(:,:,:,3)=optgriddispl(:,:,:,3)*(spz^.7)*3200;

xgriddispl=zeros(size(Px)); ygriddispl=xgriddispl; zgriddispl=xgriddispl; % init
xgriddisplparams=zeros(size(Px)); ygriddisplparams=xgriddispl; zgriddisplparams=xgriddispl;
xgriddispl(movingylocs,movingxlocs,movingzlocs)=optgriddispl(:,:,:,1);
ygriddispl(movingylocs,movingxlocs,movingzlocs)=optgriddispl(:,:,:,2);
%% Following line was commented by Mirabela Rusu on 01/20/2012
%ygriddispl(movingylocs,movingxlocs,movingzlocs)=optgriddispl(:,:,:,3);
%% Following line was added by Mirabela Rusu on 01/20/2012 to replace previous line
%% fix bug related to the transformation not being applied properly  
zgriddispl(movingylocs,movingxlocs,movingzlocs)=optgriddispl(:,:,:,3);
xgriddisplparams(movingylocs,movingxlocs,movingzlocs)=meshdata.optgriddisplparams(:,:,:,1);
ygriddisplparams(movingylocs,movingxlocs,movingzlocs)=meshdata.optgriddisplparams(:,:,:,2);
zgriddisplparams(movingylocs,movingxlocs,movingzlocs)=meshdata.optgriddisplparams(:,:,:,3);

Pxdispl=Px+xgriddispl;
Pydispl=Py+ygriddispl;
Pzdispl=Pz+zgriddispl;

meshdata.xgriddispl=xgriddispl;
meshdata.ygriddispl=ygriddispl;
meshdata.zgriddispl=zgriddispl;
meshdata.xgriddisplparams=xgriddisplparams;
meshdata.ygriddisplparams=ygriddisplparams;
meshdata.zgriddisplparams=zgriddisplparams;
meshdata.Pxdispl=Pxdispl;
meshdata.Pydispl=Pydispl;
meshdata.Pzdispl=Pzdispl;

% [Idef,junk,junk,junk,junk,tlocalx,tlocaly] = FFDimmesh(Itarget,Px,Py,Pxdispl,Pydispl); %#ok<*ASGLU>
[Idef,tlocalx,tlocaly,tlocalz] = imffd3(Itarget,Bproducts,tlis,Pxdispl,Pydispl,Pzdispl,pretlocalx,pretlocaly,pretlocalz,fillwith,[],[],[],[],[],[],[],[],[],imethod);
if ~strcmpi(measure,'alphami'), Idef=round(Idef); end

% if pre,
% [Idefjunk,tlocalx,tlocaly]=forwarddisp(Itarget,tlocalx,tlocaly,pretlocalx,pretlocaly);
% end

end

function err = ffdobjfunc(griddispl,I,J,Px,Py,Pz,N,spx,spy,spz,pretlocalx,pretlocaly,pretlocalz,fillwith,dispbool,movingxlocs,movingylocs,movingzlocs,measure,Bproducts,tlis,alphamiparams,imethod)
% tic
persistent err_record regiter Pxdispl_last Pydispl_last Pzdispl_last tlocalxlast tlocalylast tlocalzlast Jtreedists

if nargin==0,
    err=err_record(1:regiter);
    clear err_record regiter Pxdispl_last Pydispl_last tlocalxlast tlocalylast
    return;
end

if isempty(regiter), % first call
    regiter=0;
    err_record=zeros(1,1e4); % allocate for 10000 iterations
end
regiter=regiter+1;

[~,~,DIslices,DIattribs]=size(I);
[nrows,ncols,DJslices,DJattribs]=size(J);
% maxgriddisp=max(griddispl(:));
% maxgriddisppix=max(griddispl(:))*(spx^.77)*1500;
% fprintf('Max grid displacement: %g.\n',maxgriddisppix);
% griddispl(:,1)=griddispl(:,1)*spx*700;
% griddispl(:,2)=griddispl(:,2)*spy*700;
% griddispl(:,:,1)=griddispl(:,:,1)*(spx^1.1)*600;
% griddispl(:,:,2)=griddispl(:,:,2)*(spy^1.1)*600;
griddispl(:,:,:,1)=griddispl(:,:,:,1)*(spx^.7)*3200;
griddispl(:,:,:,2)=griddispl(:,:,:,2)*(spy^.7)*3200;
griddispl(:,:,:,3)=griddispl(:,:,:,3)*(spz^.7)*3200;

xgriddispl=zeros(size(Px));
ygriddispl=xgriddispl;
zgriddispl=xgriddispl;

xgriddispl(movingylocs,movingxlocs,movingzlocs)=griddispl(:,:,:,1);
ygriddispl(movingylocs,movingxlocs,movingzlocs)=griddispl(:,:,:,2);
zgriddispl(movingylocs,movingxlocs,movingzlocs)=griddispl(:,:,:,3);

% Transform image
% Iout = FFDimmesh(I,Px,Py,Px+xgriddispl,Py+ygriddispl,pretlocalx,pretlocaly,fillwith);
Pxdispl=Px+xgriddispl;
Pydispl=Py+ygriddispl;
Pzdispl=Pz+zgriddispl;
dontdodiff=false;
if dontdodiff,
    Iout = imffd3(I,Bproducts,tlis,Pxdispl,Pydispl,Pzdispl,pretlocalx,pretlocaly,pretlocalz,fillwith,Px,Py,Pz,[],[],[],[],[],[],imethod);
elseif ~isempty(tlocalylast), % after first iteration of localized/differential
    % pretlocal not combined with previous output, must feed it in everytime
    [Iout,~,~,~,tlocalxlast,tlocalylast,tlocalzlast] = imffd3(I,Bproducts,tlis,Pxdispl,Pydispl,Pzdispl,pretlocalx,pretlocaly,pretlocalz,fillwith,Px,Py,Pz,Pxdispl_last,Pydispl_last,Pzdispl_last,tlocalxlast,tlocalylast,tlocalzlast,imethod);
else % first iteration of localized/differential
    % do not combine pretlocal with tlocal output
    [Iout,~,~,~,tlocalxlast,tlocalylast,tlocalzlast] = imffd3(I,Bproducts,tlis,Pxdispl,Pydispl,Pzdispl,pretlocalx,pretlocaly,pretlocalz,fillwith,Px,Py,Pz,[],[],[],[],[],[],imethod);
end

% [Iout,tlocalxlast,tlocalylast] = imffd(I,Bproducts,tlis,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith,Px,Py);
if ~strcmpi(measure,'alphami'), Iout=round(Iout); end %toc

% Similarity measure
multiattribute=DIattribs>1 || DJattribs>1;
if multiattribute,
    % Calculate MI from attributes
    Ivect=reshape(Iout,prod([nrows ncols DIslices]),DIattribs);
    Jvect=reshape(J,prod([nrows ncols DJslices]),DJattribs);
    switch lower(measure)
        case 'mi'
            err=CMIgen(Ivect,Jvect,N);
        case 'alphami'
            if regiter==1,
                [~,~,Jtreedists]=alphami(Ivect(1:alphamiparams.ptinc:end,:),Jvect(1:alphamiparams.ptinc:end,:),...
                    alphamiparams.alpha,alphamiparams.k,'nowarn',alphamiparams.nowarn,'knnthreads',alphamiparams.knnthreads);
            end
            err=alphami(Ivect(1:alphamiparams.ptinc:end,:),Jvect(1:alphamiparams.ptinc:end,:),...
                alphamiparams.alpha,alphamiparams.k,'nowarn',alphamiparams.nowarn,...
                'Ytreedists',Jtreedists,'knnthreads',alphamiparams.knnthreads);
%             err=alphami(Ivect(1:alphamiparams.ptinc:end,:),Jvect(1:alphamiparams.ptinc:end,:),...
%                 alphamiparams.alpha,alphamiparams.k,'nowarn',alphamiparams.nowarn,'knnthreads',alphamiparams.knnthreads);
        case 'nmi'
            [~,~,~,~,~,~,~,err]=CMIgen(Ivect,Jvect,N);
        case 'ecc'
            [~,~,~,~,~,~,~,~,err]=CMIgen(Ivect,Jvect,N);
        case {'je','eoh'}
            error('Measure not implemented for multi-attribute images.');
        otherwise
            error('Measure name not recognized for multi-attribute images.');
    end
else
    switch lower(measure)
        case 'mi'
            err=mimex(J,Iout,N); %MI(J,Iout,N);
        case 'nmi'
            % [junk,junk,junk,junk,err]=MI(J,Iout,N);
            [~,err]=mimex(J,Iout,N);
        case 'ecc'
            % [junk,junk,junk,junk,junk,err]=MI(J,Iout,N);
            [~,~,err]=mimex(J,Iout,N);
        case 'je'
            % [junk,err]=MI(J,Iout,N);
            [~,~,~,err]=mimex(J,Iout,N);
            err=-err;
        case 'eoh'
            % [junk,junk,junk,junk,junk,junk,junk,junk,junk,err]=MI(J,Iout,N);
            [~,~,~,~,~,~,err]=MI(J,Iout,N);
        otherwise
            error('Measure name not recognized for 1D intensity images.');
    end
end

% Display
if ~dispbool
    cla; hold off;
    midcpind=round(size(Pz,3)/2);
    midslice_with_points=round(Pz(1,1,midcpind));
%     imdisp(rescale(Iout(:,:,ceil(DIslices/2),1))); hold on
    imdisp(rescale(Iout(:,:,midslice_with_points,1))); hold on
    % Pxdispl=Px+xgriddispl; Pydispl=Py+ygriddispl;
    Pxdispl_midz=Pxdispl(:,:,midcpind);
    Pydispl_midz=Pydispl(:,:,midcpind);
    Pzdispl_midz=Pzdispl(:,:,midcpind);
    plot(Pxdispl_midz,Pydispl_midz,'r'); plot(Pxdispl_midz',Pydispl_midz','r')
    if regiter>1,
        moving_thresh=5e-3; % match this in imffd3 and forwarddisp3 (if you want to see what is actually being "moved")
        movinglocs=abs(Pxdispl_last(:,:,midcpind)-Pxdispl_midz)>moving_thresh ...
            | abs(Pydispl_last(:,:,midcpind)-Pydispl_midz)>moving_thresh ...
            | abs(Pzdispl_last(:,:,midcpind)-Pzdispl_midz)>moving_thresh;
        plot(Pxdispl_midz(~movinglocs),Pydispl_midz(~movinglocs),'r.')
        plot(Pxdispl_midz(movinglocs),Pydispl_midz(movinglocs),'g*');
    else
        plot(Pxdispl_midz(:),Pydispl_midz(:),'r.')
    end
    xlim([0 size(Iout,2)]); ylim([0 size(Iout,1)])
    title(sprintf('objective: %g',err));
    drawnow
end

Pxdispl_last=Pxdispl;
Pydispl_last=Pydispl;
Pzdispl_last=Pzdispl;

err=-err;
% err_record(end+1)=err;
err_record(regiter)=err;

end
