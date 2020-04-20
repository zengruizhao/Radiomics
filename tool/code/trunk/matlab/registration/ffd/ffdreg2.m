function [Idef,optgriddispl,Pxdispl,Pydispl,Px,Py,MIiters,meshdata,tlocalx,tlocaly] = ffdreg2(varargin)
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
ip.addOptional('N', 256, @(x)isnumeric(x) && numel(x)==1);
measures={'alphami','MI','NMI','ECC','JE','EOH', 'SSD'};
ip.addParamValue('measure', 'MI', @(x)any(strcmpi(x,measures)));
ip.addParamValue('quiet', false, @(x)islogical(x) || x==0 || x==1);
ip.addParamValue('fillwith',0,@(x)isnumeric(x) && numel(x)==1);
% FFD parameters
ip.addParamValue('xcpoptlim',[],@(x)isnumeric(x) && numel(x)==2); % default needs to be set later
ip.addParamValue('ycpoptlim',[],@(x)isnumeric(x) && numel(x)==2); % default needs to be set later
ip.addParamValue('pretlocalx',[],@isnumeric);
ip.addParamValue('pretlocaly',[],@isnumeric);
% alpha-MI parameters
ip.addParamValue('alpha',0.98,@(x)isnumeric(x) && numel(x)==1);
ip.addParamValue('k',20,@(x)isnumeric(x) && numel(x)==1);
ip.addParamValue('ptinc',1,@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('nowarnknn', false, @(x)islogical(x) || x==0 || x==1);
ip.addParamValue('knnthreads',6,@(x)isnumeric(x) && numel(x)==1 && x>0);
% optimset parameters
ip.addParamValue('tolx',[],@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('tolfun',[],@(x)isnumeric(x) && numel(x)==1 && x>0);
ip.addParamValue('maxiter',600,@(x)isnumeric(x) && numel(x)==1 && x>0);
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
Isize=[Ymax Xmax];

% (fillwith, quiet, N, measure)
fillwith=cast(ip.Results.fillwith,class(Itarget));
quiet=logical(ip.Results.quiet);
N=ip.Results.N;
measure=ip.Results.measure;

% (npx,npy) - Number of interior control points that define the grid size
npx=ip.Results.npx;
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
    else
        xcpoptlim=ip.Results.xcpoptlim;
        ycpoptlim=xcpoptlim;
    end
else
    xcpoptlim=ip.Results.xcpoptlim;
    ycpoptlim=ip.Results.ycpoptlim;
end

% (pretlocalx, pretlocaly)
pretlocalx=ip.Results.pretlocalx;
pretlocaly=ip.Results.pretlocaly;
% if isempty(pretlocalx), pre=false;
% else pre=true; end


%%% Mesh Grid

% Used control points
npxused=xcpoptlim(2)-xcpoptlim(1)+1;
npyused=ycpoptlim(2)-ycpoptlim(1)+1;
npxdiff=npx-npxused; % unused
npydiff=npy-npyused;

% Control point spacing
spyx=(([Ymax Xmax]-1)./[npy+1 npx+1]);
spx=spyx(2); spy=spyx(1);
display(spyx)

% Generate *padded* mesh for image
gp=2; % padding on each side, units are blocks
% [Px,Py,cpxin,cpyin,PGridsize] = cpmesh([size(Itarget,1) size(Itarget,2)],spx,spy);
cpsx1=linspace(1-spx*gp,1,3); cpsx2=linspace(1,size(Itarget,2),npx+2); cpsx3=linspace(size(Itarget,2),size(Itarget,2)+spx*gp,3);
cpsx=[cpsx1 cpsx2(2:end-1) cpsx3]; clear cpsx1 cpsx2 cpsx3
cpsy1=linspace(1-spy*gp,1,3); cpsy2=linspace(1,size(Itarget,1),npy+2); cpsy3=linspace(size(Itarget,1),size(Itarget,1)+spy*gp,3);
cpsy=[cpsy1 cpsy2(2:end-1) cpsy3]; clear cpsy1 cpsy2 cpsy3
[Px,Py]=meshgrid(cpsx,cpsy);
PGridsize=size(Px);
cpxin=Px(1,Px(1,:)>0 & Px(1,:)<=size(Itarget,2));
cpyin=Py(Py(:,1)>0 & Py(:,1)<=size(Itarget,1),1)';
% number of control points in each dimension - includes padding and edges
ncpsy=PGridsize(1); ncpsx=PGridsize(2);
% 
xcpstart=Px(1); ycpstart=Py(1);

% structure for mesh grid information
meshdata.Px=Px;
meshdata.Py=Py;
meshdata.cpxin=cpxin;
meshdata.cpyin=cpyin;
meshdata.PGridsize=PGridsize;
meshdata.ncpsx=ncpsx;
meshdata.ncpsy=ncpsy;
meshdata.xcpstart=xcpstart;
meshdata.ycpstart=ycpstart;
meshdata.spx=spx;
meshdata.spy=spy;

display(cpxin), display(Px)
display(cpyin), display(Py)

%%% Registration

% Grab the interior control points (not the outside ones or the ones on the boundaries)
% initgriddispl=zeros((numel(cpxin)-2)*(numel(cpyin)-2),2);
initgriddispl=zeros(numel(cpyin)-2-npydiff,numel(cpxin)-2-npxdiff,2); % initial displacment of moving points only
gridbuffer=(size(Px)-[npy npx])./2;  % [y,x] buffer of points outside of image
imgylocs=gridbuffer(1):ncpsy-gridbuffer(1)+1; % indexes of points inside image or on boundary, but not necessarily moving
imgxlocs=gridbuffer(2):ncpsx-gridbuffer(2)+1;

movingylocs=gridbuffer(1)+ycpoptlim(1):ycpoptlim(2)+gridbuffer(1);
movingxlocs=gridbuffer(2)+xcpoptlim(1):xcpoptlim(2)+gridbuffer(2);

% copy values into meshdata
meshdata.gridbuffer=gridbuffer;
meshdata.initgriddispl=initgriddispl;
meshdata.imgxlocs=imgxlocs;
meshdata.imgylocs=imgylocs;
meshdata.movingxlocs=movingxlocs;
meshdata.movingylocs=movingylocs;

% Coordinates of Px,Py that are moving
optptsX=Px(movingylocs,movingxlocs); %Px(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
optptsY=Py(movingylocs,movingxlocs); %Py(gridbuffer(1)+1:end-gridbuffer(1),gridbuffer(2)+1:end-gridbuffer(2));
meshdata.optptsX=optptsX;
meshdata.optptsY=optptsY;
display(optptsX), display(optptsY)

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
[Bproducts,tlis] = imbeziers(Isize,Px,Py);
ffdobjfunc;
alphamiparams.alpha=ip.Results.alpha;
alphamiparams.k=ip.Results.k;
alphamiparams.ptinc=ip.Results.ptinc;
alphamiparams.nowarn=ip.Results.nowarnknn;
alphamiparams.knnthreads=ip.Results.knnthreads;
meshdata.optgriddisplparams = fminsearch(@ffdobjfunc,initgriddispl,options, ... 
    Itarget, Itemplate, Px, Py, N, spx, spy,pretlocalx,pretlocaly,fillwith,...
    quiet,movingxlocs,movingylocs,measure,Bproducts,tlis,alphamiparams);

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
optgriddispl(:,:,1)=optgriddispl(:,:,1)*(spx^.7)*3200;
optgriddispl(:,:,2)=optgriddispl(:,:,2)*(spy^.7)*3200;

xgriddispl=zeros(size(Px)); ygriddispl=xgriddispl;
xgriddisplparams=zeros(size(Px)); ygriddisplparams=xgriddispl;
xgriddispl(movingylocs,movingxlocs)=optgriddispl(:,:,1);
ygriddispl(movingylocs,movingxlocs)=optgriddispl(:,:,2);
xgriddisplparams(movingylocs,movingxlocs)=meshdata.optgriddisplparams(:,:,1);
ygriddisplparams(movingylocs,movingxlocs)=meshdata.optgriddisplparams(:,:,2);

Pxdispl=Px+xgriddispl;
Pydispl=Py+ygriddispl;

meshdata.xgriddispl=xgriddispl;
meshdata.ygriddispl=ygriddispl;
meshdata.xgriddisplparams=xgriddisplparams;
meshdata.ygriddisplparams=ygriddisplparams;
meshdata.Pxdispl=Pxdispl;
meshdata.Pydispl=Pydispl;

% [Idef,junk,junk,junk,junk,tlocalx,tlocaly] = FFDimmesh(Itarget,Px,Py,Pxdispl,Pydispl); %#ok<*ASGLU>
[Idef,tlocalx,tlocaly] = imffd(Itarget,Bproducts,tlis,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith);
if ~strcmpi(measure,'alphami'), Idef=round(Idef); end

% if pre,
% [Idefjunk,tlocalx,tlocaly]=forwarddisp(Itarget,tlocalx,tlocaly,pretlocalx,pretlocaly);
% end

end

function err = ffdobjfunc(griddispl,I,J,Px,Py,N,spx,spy,pretlocalx,pretlocaly,fillwith,dispbool,movingxlocs,movingylocs,measure,Bproducts,tlis,alphamiparams)
% tic
persistent err_record regiter Pxdispl_last Pydispl_last tlocalxlast tlocalylast Jtreedists

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
griddispl(:,:,1)=griddispl(:,:,1)*(spx^.7)*3200;
griddispl(:,:,2)=griddispl(:,:,2)*(spy^.7)*3200;

xgriddispl=zeros(size(Px));
ygriddispl=xgriddispl;

xgriddispl(movingylocs,movingxlocs)=griddispl(:,:,1);
ygriddispl(movingylocs,movingxlocs)=griddispl(:,:,2);

% Transform image
% Iout = FFDimmesh(I,Px,Py,Px+xgriddispl,Py+ygriddispl,pretlocalx,pretlocaly,fillwith);
Pxdispl=Px+xgriddispl;
Pydispl=Py+ygriddispl;
dontdodiff=false;
if dontdodiff,
    Iout = imffd(I,Bproducts,tlis,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith,Px,Py);
elseif ~isempty(tlocalylast), % after first iteration of localized/differential
    % pretlocal not combined with previous output, must feed it in everytime
    [Iout,~,~,tlocalxlast,tlocalylast] = imffd(I,Bproducts,tlis,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith,Px,Py,Pxdispl_last,Pydispl_last,tlocalxlast,tlocalylast);
else % first iteration of localized/differential
    % do not combine pretlocal with tlocal output
    [Iout,~,~,tlocalxlast,tlocalylast] = imffd(I,Bproducts,tlis,Pxdispl,Pydispl,pretlocalx,pretlocaly,fillwith,Px,Py);
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
                    alphamiparams.alpha,alphamiparams.k,'nowarn',alphamiparams.nowarn,...
                    'knnthreads',alphamiparams.knnthreads,'noise',false);
            end
            err=alphami(Ivect(1:alphamiparams.ptinc:end,:),Jvect(1:alphamiparams.ptinc:end,:),...
                alphamiparams.alpha,alphamiparams.k,'nowarn',true,...
                'Ytreedists',Jtreedists,'knnthreads',alphamiparams.knnthreads,...
                'noise',false);
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
        case 'ssd'
            err = -sumall((Iout-J).^2);
        otherwise
            error('Measure name not recognized for 1D intensity images.');
    end
end

% Display
if ~dispbool
    cla; hold off;
    subplot(1,3,1);
    imdisp(J(:,:,1,1))
    subplot(1,3,2);
    imdisp(J(:,:,1,1)-Iout(:,:,1,1))
    
    subplot(1,3,3);
    imdisp(rescale(Iout(:,:,ceil(DIslices/2),1))); hold on
    % Pxdispl=Px+xgriddispl; Pydispl=Py+ygriddispl;
    plot(Pxdispl,Pydispl,'r'); plot(Pxdispl',Pydispl','r')
    if regiter>1, 
        movinglocs=abs(Pxdispl_last-Pxdispl)>1e-4 | abs(Pydispl_last-Pydispl)>1e-4;
        plot(Pxdispl(~movinglocs),Pydispl(~movinglocs),'r.')
        plot(Pxdispl(movinglocs),Pydispl(movinglocs),'g*');
    else
        plot(Pxdispl(:),Pydispl(:),'r.')
    end
    xlim([0 size(Iout,2)]); ylim([0 size(Iout,1)])
    title(sprintf('objective: %g',err));
    drawnow
end

Pxdispl_last=Pxdispl;
Pydispl_last=Pydispl;

err=-err;
% err_record(end+1)=err;
err_record(regiter)=err;

end
