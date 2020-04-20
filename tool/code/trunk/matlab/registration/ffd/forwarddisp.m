function [Iout,tlocalx,tlocaly] = forwarddisp(I,tlocalx, tlocaly, pretlocalx, pretlocaly, fillwith)
% FORWARDDISP Apply a forward displacement field to an image.
%   IOUT = FORWARDDISP(I,TLOCALX,TLOCALY) applies the forward displacement
%   field (i.e. one that specifies the destination for points on the source
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
%
% NOTE: This function can be implemented with calls to interp2 for 
% PRETLOCAL* inverse transformations to TLOCAL*, and a call to griddata or
% TriScatteredInterp, for the TLOCAL* forward deformation of I.  See
% forwarddisp_testing.m.

tol=1e-6;%sqrt(eps);%1e-10;

if nargin<6,
    fillwith=cast(0,class(I));
end


%%% Pre-deformation fields (pretlocalx, pretlocaly), updating primary field

% Got non-empty PRETLOCALX and PRETLOCALY?
if nargin>3 && ~isempty(pretlocaly),
    % tlocalxorig=tlocalx; tlocalyorig=tlocaly;
    
    % NOTE: pretlocalx and pretlocaly can be cell arrays of arbitrary 
    % length, representing multiple successive deformation fields.
    
    % Make predef inputs into cells, if not already
    if ~iscell(pretlocalx)
        pretlocalx={pretlocalx};
    end
    if ~iscell(pretlocaly),
        pretlocaly={pretlocaly};
    end
    if numel(pretlocalx) ~= numel(pretlocaly),
        error('Different number cells in PRETLOCALX and PRETLOCALY.');
    end
    
    Iplanesize=[size(I,1), size(I,2)];
    
    ndefs=numel(pretlocalx);
%     for i=1:ndefs,
%         % pretlocalx{i}(pretlocalx{i}<1)=1; pretlocalx{i}(pretlocalx{i}>size(I,2))=size(I,2);
%         % pretlocaly{i}(pretlocaly{i}<1)=1; pretlocaly{i}(pretlocaly{i}>size(I,1))=size(I,1);
%         
%         if i<ndefs, % combine the pretlocalx and pretlocaly inputs
%             % tlocalxnew=interp2(pretlocalx{i+1},pretlocalx{i},pretlocaly{i},imethod);
%             % tlocalynew=interp2(pretlocaly{i+1},pretlocalx{i},pretlocaly{i},imethod);
%             tlocalxnew=linterp2mex(pretlocalx{i+1},pretlocalx{i},pretlocaly{i});
%             tlocalynew=linterp2mex(pretlocaly{i+1},pretlocalx{i},pretlocaly{i});
%             pretlocaly{i+1}=tlocalynew;
%             pretlocalx{i+1}=tlocalxnew;
%         else % then modify tlocalx and tlocaly
%             % tlocalx=interp2(tlocalx,pretlocalx{i},pretlocaly{i},imethod);
%             % tlocaly=interp2(tlocaly,pretlocalx{i},pretlocaly{i},imethod);
%             tlocalx=linterp2mex(tlocalx,pretlocalx{i},pretlocaly{i});
%             tlocaly=linterp2mex(tlocaly,pretlocalx{i},pretlocaly{i});
%         end
%         
%     end
    
    % for i=length(pretlocalx):-1:1,
    for i=1:ndefs,
        % Bounds check
        pretlocalx{i}(pretlocalx{i}<1)=1; pretlocalx{i}(pretlocalx{i}>size(I,2))=size(I,2);
        pretlocaly{i}(pretlocaly{i}<1)=1; pretlocaly{i}(pretlocaly{i}>size(I,1))=size(I,1);
        
        % Linear gray level interpolation/blending
        % x coordinates
        lowx=floor(pretlocalx{i});
        highx=ceil(pretlocalx{i});
        txhighweight=pretlocalx{i}-lowx;
        txlowweight=1-txhighweight;
        % deal with numerical precision problems
        basically_zero_high=txhighweight<=tol;
        basically_zero_low=txlowweight<=tol;
        txhighweight(basically_zero_high)=0; txhighweight(basically_zero_low)=1;
        txlowweight(basically_zero_low)=0; txlowweight(basically_zero_high)=1;
        highx(basically_zero_high)=lowx(basically_zero_high);
        lowx(basically_zero_low)=highx(basically_zero_low);

        % y coodinates
        lowy=floor(pretlocaly{i});
        highy=ceil(pretlocaly{i});
        tyhighweight=pretlocaly{i}-lowy;
        tylowweight=1-tyhighweight;
        % deal with numerical precision problems
        basically_zero_high=tyhighweight<=tol;
        basically_zero_low=tylowweight<=tol;
        tyhighweight(basically_zero_high)=0; tyhighweight(basically_zero_low)=1;
        tylowweight(basically_zero_low)=0; tylowweight(basically_zero_high)=1;
        highy(basically_zero_high)=lowy(basically_zero_high);
        lowy(basically_zero_low)=highy(basically_zero_low);
        
        % calculate weights of coordinates 4 neighboring pixels
        tylowtxlowweight=txlowweight.*tylowweight;
        tylowtxhighweight=txhighweight.*tylowweight;
        tyhightxlowweight=txlowweight.*tyhighweight;
        tyhightxhighweight=txhighweight.*tyhighweight;
        
        % prelowlowinds=sub2ind(Iplanesize, lowy, lowx);
        % prelowhighinds=sub2ind(Iplanesize, lowy, highx);
        % prehighlowinds=sub2ind(Iplanesize, highy, lowx);
        % prehighhighinds=sub2ind(Iplanesize, highy, highx);
        prelowlowinds=lowy+(lowx-1)*Iplanesize(1);
        prelowhighinds=lowy+(highx-1)*Iplanesize(1);
        prehighlowinds=highy+(lowx-1)*Iplanesize(1);
        prehighhighinds=highy+(highx-1)*Iplanesize(1);
        
        % Incorporate pretlocalx,y into tlocalx,y (or next pretlocalx,y)
        tlocalxnew=zeros(size(tlocalx));
        tlocalynew=zeros(size(tlocaly));
        if i==ndefs, % last pretlocalx - adjust tlocalx
            tlocalxnew(:) = tlocalx(prelowlowinds) .* tylowtxlowweight;
            tlocalxnew(:) = tlocalxnew(:) + tlocalx(prelowhighinds) .* tylowtxhighweight;
            tlocalxnew(:) = tlocalxnew(:) + tlocalx(prehighlowinds) .* tyhightxlowweight;
            tlocalxnew(:) = tlocalxnew(:) + tlocalx(prehighhighinds) .* tyhightxhighweight;
        else % more pretlocalx coming - adjust next tlocalx
            tlocalxnew(:) = pretlocalx{i+1}(prelowlowinds) .* tylowtxlowweight;
            tlocalxnew(:) = tlocalxnew(:) + pretlocalx{i+1}(prelowhighinds) .* tylowtxhighweight;
            tlocalxnew(:) = tlocalxnew(:) + pretlocalx{i+1}(prehighlowinds) .* tyhightxlowweight;
            tlocalxnew(:) = tlocalxnew(:) + pretlocalx{i+1}(prehighhighinds) .* tyhightxhighweight;
        end

        if i==ndefs, % last pretlocaly - adjust tlocaly
            tlocalynew(:) = tlocaly(prelowlowinds) .* tylowtxlowweight;
            tlocalynew(:) = tlocalynew(:) + tlocaly(prelowhighinds) .* tylowtxhighweight;
            tlocalynew(:) = tlocalynew(:) + tlocaly(prehighlowinds) .* tyhightxlowweight;
            tlocalynew(:) = tlocalynew(:) + tlocaly(prehighhighinds) .* tyhightxhighweight;
        else % more pretlocaly coming - adjust next tlocaly
            tlocalynew(:) = pretlocaly{i+1}(prelowlowinds) .* tylowtxlowweight;
            tlocalynew(:) = tlocalynew(:) + pretlocaly{i+1}(prelowhighinds) .* tylowtxhighweight;
            tlocalynew(:) = tlocalynew(:) + pretlocaly{i+1}(prehighlowinds) .* tyhightxlowweight;
            tlocalynew(:) = tlocalynew(:) + pretlocaly{i+1}(prehighhighinds) .* tyhightxhighweight;
        end

        % Fix out of bounds points
        outtlocalx=isnan(tlocalxnew); outtlocaly=isnan(tlocalynew);
        tlocalxnew(outtlocalx)=1;
        tlocalynew(outtlocaly)=1;
        if i<ndefs,%>1,
            pretlocaly{i+1}=tlocalynew;
            pretlocalx{i+1}=tlocalxnew;
        end

    end
    tlocalx=tlocalxnew(:);
    tlocaly=tlocalynew(:);
end

% THE ALTERNATIVE for pre-deformation fields:
% imethod='linear';
% tlocalxalt=interp2(tlocalxorig,pretlocalx{i},pretlocaly{i},imethod);
% tlocalyalt=interp2(tlocalyorig,pretlocalx{i},pretlocaly{i},imethod);
% END ALTERNATIVE


%%% Primary deformation fields (tlocalx, tlocaly)

% Linear Interpolation/Blending
tol=1e-6;%sqrt(eps);%1e-10;

tlocalx=tlocalx(:);
tlocaly=tlocaly(:);

% Linear gray level interpolation/blending
% x coordinates
lowx=floor(tlocalx);
highx=ceil(tlocalx);
txhighweight=tlocalx-lowx;
txlowweight=1-txhighweight;
% deal with numerical precision problems
basically_zero_high=txhighweight<=tol;
basically_zero_low=txlowweight<=tol;
txhighweight(basically_zero_high)=0; txhighweight(basically_zero_low)=1;
txlowweight(basically_zero_low)=0; txlowweight(basically_zero_high)=1;
highx(basically_zero_high)=lowx(basically_zero_high);
lowx(basically_zero_low)=highx(basically_zero_low);

% y coordinates
lowy=floor(tlocaly);
highy=ceil(tlocaly);
tyhighweight=tlocaly-lowy;
tylowweight=1-tyhighweight;
% deal with numerical precision problems
basically_zero_high=tyhighweight<=tol;
basically_zero_low=tylowweight<=tol;
tyhighweight(basically_zero_high)=0; tyhighweight(basically_zero_low)=1;
tylowweight(basically_zero_low)=0; tylowweight(basically_zero_high)=1;
highy(basically_zero_high)=lowy(basically_zero_high);
lowy(basically_zero_low)=highy(basically_zero_low);

% calculate weights of coordinates 4 neighboring pixels
tylowtxlowweight=txlowweight.*tylowweight;
tylowtxhighweight=txhighweight.*tylowweight;
tyhightxlowweight=txlowweight.*tyhighweight;
tyhightxhighweight=txhighweight.*tyhighweight;


%%% Deform image

% Calculate output image size and necessary padding
% defmag=ceil(max(max(sqrt((Px-Pxdispl).^2+(Py-Pydispl).^2))));
Isize=size(I);
maxprotrusion=max([max(highx(highx>=Isize(2)))-Isize(2) abs(min(lowx(lowx<=1)))+1 ...
    max(highy(highy>=Isize(1)))-Isize(1) abs(min(lowy(lowy<=1)))+1 1]);  % might throw warnings
pad=4*maxprotrusion; % even number
Ioutpaddedsize=[Isize(1)+pad, Isize(2)+pad];
ycrop=pad/2+1:Isize(1)+pad/2;
xcrop=pad/2+1:Isize(2)+pad/2;
Iout=zeros(Isize);

% lowlowinds=sub2ind(Ioutpaddedsize, lowy + pad/2, lowx + pad/2);
% lowhighinds=sub2ind(Ioutpaddedsize, lowy + pad/2, highx + pad/2);
% highlowinds=sub2ind(Ioutpaddedsize, highy + pad/2, lowx + pad/2);
% highhighinds=sub2ind(Ioutpaddedsize, highy + pad/2, highx + pad/2);
lowlowinds=(lowy+pad/2)+(lowx+(pad/2-1))*Ioutpaddedsize(1);
lowhighinds=(lowy+pad/2)+(highx+(pad/2-1))*Ioutpaddedsize(1);
highlowinds=(highy+pad/2)+(lowx+(pad/2-1))*Ioutpaddedsize(1);
highhighinds=(highy+pad/2)+(highx+(pad/2-1))*Ioutpaddedsize(1);

% For each pixel on output image, calculate total weight of incoming pixels
Itotalweight=zeros(Ioutpaddedsize);
Itotalweight(lowlowinds) = tylowtxlowweight;
Itotalweight(lowhighinds) = Itotalweight(lowhighinds) + tylowtxhighweight;
Itotalweight(highlowinds) = Itotalweight(highlowinds) + tyhightxlowweight;
Itotalweight(highhighinds) = Itotalweight(highhighinds) + tyhightxhighweight;

% crop and wipe out zero weights (from outside image)
Itotalweight=Itotalweight(ycrop,xcrop);%+(~Itotalweight(ycrop,xcrop));

[nrows,ncols,totalplanes]=size(I); % note that ndims(I) may be >3, fine
Id=double(reshape(I,nrows*ncols,totalplanes));

% Transform image(s)
for i=1:totalplanes,  % each plane/color-channel
    Iouti=zeros(Ioutpaddedsize);
    
    Iouti(lowlowinds) = Id(:,i) .* tylowtxlowweight;
    Iouti(lowhighinds) = Iouti(lowhighinds) + Id(:,i) .* tylowtxhighweight;
    Iouti(highlowinds) = Iouti(highlowinds) + Id(:,i) .* tyhightxlowweight;
    Iouti(highhighinds) = Iouti(highhighinds) + Id(:,i) .* tyhightxhighweight;
    
%     warning('off','MATLAB:divideByZero')
    Iout(:,:,i)=Iouti(ycrop,xcrop)./Itotalweight;   % normalize intensity
%     warning('on','MATLAB:divideByZero')
%     Iout(:,:,i)=Iouti(ycrop,xcrop);
end

% if >3D
if ndims(I)>3,
    Iout=reshape(Iout,Isize);
end
Iout(isnan(Iout))=cast(fillwith,class(Iout));  % does background too because we set Itotalweight to zero to begin!

% THE ALTERNATIVE BELOW:
% Get values at col_samples and row_samples
% Ioutalt=griddata(tlocalxalt,tlocalyalt,I,X,Y);
% Ioutalt(isnan(Ioutalt))=cast(fillwith,class(Ioutalt));
% END ALTERNATIVE
