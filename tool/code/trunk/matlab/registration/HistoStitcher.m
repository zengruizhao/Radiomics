function varargout = HistoStitcher(varargin)
% HISTOSTITCHER Place control points and calculate affine/linear transform.
%   HISTOSTITCHER(IM1,IM2), allows interactive selection of control points
%   on a pair of images, followed by calculation of the necessary affine
%   transformation to minimize the sum of squared distances between the
%   transformed points.
%
%   M = HISTOSTITCHER(...) returns the homogeneous affine transformation
%   matrix necessary to map the control points on image B towards the
%   corresponding points on image A.
%
%   [M,IM1PTS,IM2PTS,IM2PTSMOVED] = HISTOSTITCHER(...) returns the manually
%   placed control points, IM1PTS and IM2PTS, as well as the transformed
%   control points of IM2.
%
%   [M,IM1PTS,IM2PTS,IM2PTSMOVED,XFORM] = HISTOSTITCHER(...) returns the
%   structure XFORM containing the transormation matrix, the decomposed
%   rotation and scaling matrices, the translations, and the rotation
%   angles.
%
%   [M,IM1PTS,IM2PTS,IM2PTSMOVED,XFORM,OO,IM12OVERLAY] = HISTOSTITCHER(...)
%   returns the origin offset OO.  This is the location of the original top
%   left corner of IM2 in the overlay image, IM12OVERLAY.
%
%   [..,IM12OVERLAY,IM2DEF] = HISTOSTITCHER(...) returns the transformed
%   IM2 in the eigth (8th) output argument.  This image IM2DEF is the size
%   of the input images (i.e. if they are differnet sizes, the maximum on
%   each dimension).
%
%   For example:
%    [M,im1pts,im2pts,im2ptsmoved,xform,oo,im12overlay,im2def]=HistoStitcher;
%
% See also: LSQAFFINEH, LSQSIMILARITYH, AFFINEDECOMP, PROCRUSTES, RESCALE.
%
%JC

% Last Modified by GUIDE v2.5 14-Dec-2010 14:05:37

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @HistoStitcher_OpeningFcn, ...
                   'gui_OutputFcn',  @HistoStitcher_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%% INITIALIZATION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes just before HistoStitcher is made visible.
function HistoStitcher_OpeningFcn(hObject, eventdata, handles, varargin) %#ok<*INUSL>
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to HistoStitcher (see VARARGIN)

% Make a global for the overlay since it could be huge
clear global im12overlay
global im12overlay %#ok<NUSED>

% Choose default command line output for HistoStitcher
handles.M = [];

% Selection init
handles.gettingpoints=0;
handles.activeaxis=0;
handles.drawn=0;

handles.dolargetrans=false;

% setup for the affine case
handles.isotropicscale=0;
handles.allowreflection=1;
handles.allowscaling=1;
handles.xformtype='Unconstrained Affine (reflection, anisotropic scaling)';

handles.im2def=[];
handles.originoffset=[];
handles.im2deffigh=[];
handles.im1fullfig=[];
handles.im2fullfig=[];

handles.ptcount=0;handles.newptcount=0;
handles.im1pts=[];handles.im1ptshandles=[];
handles.im2pts=[];handles.im2ptshandles=[];
handles.im2ptsmoved=[];%handles.movedptshandle=0;

[handles.MR1,handles.MS,handles.MR2,handles.MR1alt,handles.MR2alt, ... 
        handles.theta1, handles.theta2, handles.theta1alt, handles.theta2alt, ...
        handles.MRSRx, handles.MRSRxalt,handles.translation]=deal([]);

% Get the two images from files if no inputs specified
if numel(varargin)<2,
    [I1filename, I1pathname] = uigetfile( ...
       {'*.tif*;*.png;*.jpg;*.svs', 'All Image Files (*.tif*, *.png, *.jpg, *.svs)';
        '*.tif*','TIFF images (*.tif, *.tiff)'; ...
        '*.png','PNG images (*.png)'; ...
        '*.jpg','JPG images (*.jpg)'; ...
        '*.svs','SVS images (*.svs)'; ...
        '*.*',  'All Files (*.*)'}, ...
        'Pick Image Number 1 (stationary)');
    [I2filename, I2pathname] = uigetfile( ...
       {'*.tif*;*.png;*.jpg;*.svs', 'All Image Files (*.tif*, *.png, *.jpg, *.svs)';
        '*.tif*','TIFF images (*.tif, *.tiff)'; ...
        '*.png','PNG images (*.png)'; ...
        '*.jpg','JPG images (*.jpg)'; ...
        '*.svs','SVS images (*.svs)'; ...
        '*.*',  'All Files (*.*)'}, ...
        'Pick Image Number 2 (moving)');
    if isequal(I1filename,0) || isequal(I1filename,0) || isequal(I2filename,0) || isequal(I2filename,0)
       guidata(hObject, handles);
       return
    end
    
    fprintf('Inspecting image file 1... ');
    I1info=imfinfo([I1pathname I1filename]);
    for i=1:numel(I1info),pageinfo1{i}=['Page ' num2str(i) ': ' num2str(I1info(i).Height) ' x ' num2str(I1info(i).Width)]; end %#ok<AGROW>
    fprintf('done.\n');
    if numel(I1info)>1,
        [s,v]=listdlg('Name','Choose Level','PromptString','Select a page for image 1:','SelectionMode','single','ListSize',[170 120],'ListString',pageinfo1); drawnow;
        if ~v, guidata(hObject, handles); return; end
        fprintf('Reading page %g of image 1... ',s);
        handles.im1=imread([I1pathname I1filename],s);
        fprintf('done.\n');
    elseif numel(I1info)==1,
        fprintf('Reading image 1... ');
        handles.im1=imread([I1pathname I1filename]);
        fprintf('done.\n');
    elseif numel(I1info)==0,
        guidata(hObject, handles); return;
    end
    
    fprintf('Inspecting image file 2... ');
    I2info=imfinfo([I2pathname I2filename]);
    fprintf('done.\n');
    for i=1:numel(I2info),pageinfo2{i}=['Page ' num2str(i) ': ' num2str(I2info(i).Height) ' x ' num2str(I2info(i).Width)]; end %#ok<AGROW>
    if numel(I2info)>1,
        [s,v]=listdlg('Name','Choose Level','PromptString','Select a page for image 2:','SelectionMode','single','ListSize',[170 120],'ListString',pageinfo2); drawnow;
        if ~v, guidata(hObject, handles); return; end
        fprintf('Reading page %g of image 2... ',s);
        handles.im2=imread([I2pathname I2filename],s);
        fprintf('done.\n');
    elseif numel(I2info)==1,
        fprintf('Reading image 2... ');
        handles.im2=imread([I2pathname I2filename]);
        fprintf('done.\n');
    elseif numel(I2info)==0,
        guidata(hObject, handles); return;
    end

    % guidata(hObject, handles);
    % return
else
    handles.im1=varargin{1};
    handles.im2=varargin{2};
end

handles.ndims=size(handles.im1,3);
if handles.ndims~=size(handles.im2,3), error('Both images must be color or grayscale'); end

handles.im1class=class(handles.im1);
handles.im2class=class(handles.im2);
if ~strcmp(handles.im1class,handles.im2class),
    error('Image datatypes do not match.'); end

[vals,ii]=max(sqrt(sum(handles.im1.^2,3)));
[~,j]=max(vals); i=ii(j);
handles.whitecolor1=handles.im1(i,j,:);

[vals,ii]=max(sqrt(sum(handles.im2.^2,3)));
[~,j]=max(vals); i=ii(j);
handles.whitecolor2=handles.im2(i,j,:);

handles.whiteval=max([max(handles.im1(:)) max(handles.im2(:))]);

ButtonName = questdlg('Do you want to segment the background?', 'Segment?', 'Yes', 'No', 'Yes'); drawnow;
switch ButtonName,
    case 'Yes',
        k=3;
        hi=figure; ha=axes('Parent',hi); imdisp(ha,handles.im1);
        title('Click on the intensity of interest/background.')
        [x,y]=ginput(1); c1=round(x); r1=round(y); close(hi); drawnow;
        
        hi=figure; ha=axes('Parent',hi); imdisp(ha,handles.im2);
        title('Click on the intensity of interest/background.')
        [x,y]=ginput(1); c2=round(x); r2=round(y); close(hi); drawnow;
        
        [handles.bgmask1,~,~,handles.bgkcenter1,~,~,~,edge1] = bgseg(rgb2gray(handles.im1),k,r1,c1);
        handles.bgmask1=~handles.bgmask1;
        
        [handles.bgmask2,~,~,handles.bgkcenter2,~,~,~,edge2] = bgseg(rgb2gray(handles.im2),k,r2,c2);
        handles.bgmask2=~handles.bgmask2;
        
        hi=figure; ha=subplot(121);
        imdisp(ha,handles.bgmask1);
        ha=subplot(122);
        imdisp(ha,handles.bgmask2);
        pause; close(hi); drawnow;
        
        bgvals=handles.im1(repmat(~handles.bgmask1,[1 1 3]));
        bgvals=reshape(bgvals,numel(bgvals)/3,3);
        handles.bgmedian1=median(bgvals);
        bgvals=handles.im2(repmat(~handles.bgmask2,[1 1 3]));
        bgvals=reshape(bgvals,numel(bgvals)/3,3);
        handles.bgmedian2=median(bgvals);

    case 'No',
        handles.bgmask1=[];
        handles.bgmask2=[];
        handles.bgkcenter1=handles.whiteval;
        handles.bgkcenter2=handles.whiteval;
        handles.bgmedian1=zeros([1 handles.ndims]);
        handles.bgmedian2=zeros([1 handles.ndims]);
        
end % switch

fprintf('Padding images to same size... ');
padsize=[max(size(handles.im1,1),size(handles.im2,1)) max(size(handles.im1,2),size(handles.im2,2))];
if isa(handles.im1,'float'),
    fprintf('float data... ');
    handles.im1=impad(handles.im1,padsize,handles.bgmedian1); % rescale accepts singles without conversion, but warns
    handles.im2=impad(handles.im2,padsize,handles.bgmedian2);
else
    fprintf('converting integer to single... ');
%     handles.im1=single(rescale(impad(handles.im1,padsize))); % rescale converts integers to double for maximum accuracy
%     handles.im2=single(rescale(impad(handles.im2,padsize)));
    handles.im1=single(impad(handles.im1,padsize,handles.bgmedian1)); % rescale accepts singles without conversion, but warns
    handles.im2=single(impad(handles.im2,padsize,handles.bgmedian2));
end
if ~isempty(handles.bgmask1),
    handles.bgmask1=impad(handles.bgmask1,padsize,false);
    handles.bgmask2=impad(handles.bgmask2,padsize,false);
    edge1=impad(edge1,padsize,false);
    [handles.edgei1,handles.edgej1]=find(edge1); clear edge1
    edge2=impad(edge2,padsize,false);
    [handles.edgei2,handles.edgej2]=find(edge2); clear edge2
else
    handles.bgmask1=true([size(handles.im1,1) size(handles.im1,2)]);
    handles.bgmask2=true([size(handles.im1,1) size(handles.im1,2)]);
    handles.edgei1=[]; handles.edgej1=[];
    handles.edgei2=[]; handles.edgej2=[];
end
fprintf('done.\n');

handles.im1mask=any(handles.im1>eps,3);
handles.im2mask=any(handles.im2>eps,3);

% Update handles structure
guidata(hObject, handles);
btndownax1=get(handles.im1ax,'ButtonDownFcn');
btndownax2=get(handles.im2ax,'ButtonDownFcn');

set(handles.im1ax,'Units','pixels');
set(handles.im2ax,'Units','pixels');
handles.im1axpos=get(handles.im1ax,'Position');
handles.im2axpos=get(handles.im2ax,'Position');
yfraction=handles.im1axpos(4)/padsize(1);xfraction=handles.im1axpos(3)/padsize(2);
fraction=(yfraction+xfraction)/2;
handles.dispsize=round(padsize*fraction*2);
handles.fraction=handles.dispsize./padsize;
handles.fraction=mean(handles.fraction);
fprintf('Resizing image 1 for GUI display... ');
handles.im1disp=imresize(handles.im1,handles.dispsize,'lanczos3');
handles.im1dispNN=imresize(handles.im1,handles.dispsize,'nearest');
handles.bgmask1disp=logical(imresize(handles.bgmask1,handles.dispsize,'nearest'));
handles.im1range=[min(handles.im1(:)) max(handles.im1(:))];
% toobright=any(handles.im1disp>handles.im1range(2),3);
% for i=1:numel(handles.whitecolor1),
%     implane=handles.im1disp(:,:,i);
%     implane(toobright)=handles.whitecolor1(i); % handles.im1range(2);
%     handles.im1disp(:,:,i)=implane;
% end
handles.im1disp(handles.im1disp<handles.im1range(1))=handles.im1range(1);
handles.im1disp(handles.im1disp>handles.im1range(2))=handles.im1range(2);
% handles.im1disp(repmat(~any(handles.im1dispNN>eps,3),[1 1 3]))=0;
fprintf('image 2... ');
handles.im2disp=imresize(handles.im2,handles.dispsize,'lanczos3');
handles.im2dispNN=imresize(handles.im2,handles.dispsize,'nearest');
handles.bgmask2disp=logical(imresize(handles.bgmask2,handles.dispsize,'nearest'));
handles.im2range=[min(handles.im2(:)) max(handles.im2(:))];
% toobright=any(handles.im2disp>handles.im2range(2),3);
% for i=1:numel(handles.whitecolor2),
%     implane=handles.im2disp(:,:,i);
%     implane(toobright)=handles.whitecolor2(i); % handles.im2range(2);
%     handles.im1disp(:,:,i)=implane;
% end
handles.im2disp(handles.im2disp<handles.im2range(1))=handles.im2range(1);
handles.im2disp(handles.im2disp>handles.im2range(2))=handles.im2range(2);
% handles.im2disp(repmat(~any(handles.im2dispNN>eps,3),[1 1 3]))=0;
fprintf('done.\n');

axes(handles.im1ax) %#ok<MAXES>
handles.him1=imagesc(cast(handles.im1disp,handles.im1class));
% axis off equal tight; colormap gray
axis off equal tight;
if size(handles.im1disp,3)==1, colormap gray; end
set(handles.him1,'ButtonDownFcn',btndownax1)

axes(handles.im2ax) %#ok<MAXES>
handles.him2=imagesc(cast(handles.im2disp,handles.im2class));
% axis off equal tight; colormap gray
axis off equal tight;
if size(handles.im2disp,3)==1, colormap gray; end
set(handles.him2,'ButtonDownFcn',btndownax2)

handles.drawn=1;
% get(handles.im1ax,'ButtonDownFcn')

set(handles.xformtypetext,'String',handles.xformtype);

drawnow;
handles.ss=get(0,'ScreenSize');

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes HistoStitcher wait for user response (see UIRESUME)
uiwait(handles.HistoStitcher);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% OUTPUTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Outputs from this function are returned to the command line.
function varargout = HistoStitcher_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);

global im12overlay

affdecomp=struct('MR1',handles.MR1, 'MS',handles.MS, 'MR2',handles.MR2, ... 
    'MR1alt',handles.MR1alt,'MR2alt',handles.MR2alt, 'theta1',handles.theta1, ...
    'theta2',handles.theta2, 'theta1alt',handles.theta1alt, 'theta2alt',handles.theta2alt, ...
        'MRSRx',handles.MRSRx, 'MRSRxalt',handles.MRSRxalt, 'translation',handles.translation);

% Get default command line output from handles structure
varargout{1} = handles.M;
varargout{2} = handles.im1pts;
varargout{3} = handles.im2pts;
varargout{4} = handles.im2ptsmoved;
varargout{5} = affdecomp;
varargout{6} = handles.originoffset;
if ~isempty(im12overlay),
    ButtonName = questdlg('Do you want to crop the stitched image?', 'Crop?', 'Yes', 'No', 'Yes');
    switch ButtonName,
        case 'Yes',
            cropfig=figure;
            imdisp(im12overlay);
            [im12overlay,croprect]=imcrop(cropfig);
            close(cropfig);
            drawnow
        case 'No',
            croprect=[];
    end % switch
    % resfig=figure;
    im12overlay=cast(im12overlay,handles.im1class);
    imdisp(im12overlay)
    title('Image overlay');
else
    warndlg('Ouput image empty!','No Output Image');
    croprect=[];
end
varargout{7} = im12overlay;
varargout{8} = handles.im2def;
varargout{9}=croprect;

delete(handles.HistoStitcher);

clear global im12overlay

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CLOSING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes when user attempts to close HistoStitcher.
function HistoStitcher_CloseRequestFcn(hObject, eventdata, handles)

if isequal(get(handles.HistoStitcher, 'waitstatus'), 'waiting')
    uiresume(handles.HistoStitcher);
else
    delete(handles.HistoStitcher);
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%% KEYBOARD PRESSED WITH NOTHING SELECTED %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on key press with focus on HistoStitcher and no controls selected.
function HistoStitcher_KeyPressFcn(hObject, eventdata, handles)

if handles.gettingpoints,
    % isequal(get(hObject,'CurrentKey'),'escape')
    d = double(get(handles.HistoStitcher, 'currentcharacter'));
    if any(d==27), % 27=escape, 13=Return, 32=space
    	handles=stopgettingpoints(handles);
    end
elseif isequal(get(hObject,'CurrentKey'),'escape')
    uiresume(handles.HistoStitcher);
end

guidata(hObject, handles);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% "Choose Points/Enough" button %%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% KEYBOARD  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on key press with focus on getcpsbutton and no controls selected.
function getcpsbutton_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to getcpsbutton (see GCBO)
% handles    structure with handles and user data (see GUIDATA)

d = double(get(handles.HistoStitcher, 'currentcharacter'));
if handles.gettingpoints,    
    if any(d==[13 27]), % 27=escape, 13=Return, 32=space
    	handles=stopgettingpoints(handles);
    end
elseif any(d==13),
    getcpsbutton_Callback(hObject, eventdata, handles);
    return
elseif d==27, % escape presed, quit
    uiresume(handles.HistoStitcher);
end

guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%% CALLBACK (Mouse & Space bar) %%%%%%%%%%%%%%%%%%%%%
% --- Executes on button press in getcpsbutton.
function getcpsbutton_Callback(hObject, eventdata, handles)
% hObject    handle to getcpsbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.gettingpoints==1,
    handles=stopgettingpoints(handles);
else
%     if handles.movedptshandle,
%         delete(handles.movedptshandle);
%         handles.movedptshandle=0;
%     end
    
    % set(gcf,'pointer','cross');
    handles.gettingpoints=1;
    handles.activeaxis=1;

    % set(handles.getcpsbutton,'Enable','off')
    set(handles.getcpsbutton,'String','Enough')
    set(handles.calcxformbutton,'Enable','off')
    set(handles.transimbutton,'Enable','off')

    set(handles.donebutton,'String','Cancel')
end

guidata(hObject, handles);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% "Done/Cancel" button %%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% KEYBOARD  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on key press with focus on donebutton and no controls selected.
function donebutton_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to donebutton (see GCBO)
% handles    structure with handles and user data (see GUIDATA)

d = double(get(handles.HistoStitcher, 'currentcharacter'));
if handles.gettingpoints,
    if any(d==[13 27]), % 27=escape, 13=Return, 32=space
        handles=stopgettingpoints(handles);
        if handles.newptcount>0,
            handles.im1pts(end-handles.newptcount+1:end,:)=[];
            handles.im2pts(end-handles.newptcount+1:end,:)=[];
            delete(handles.im1ptshandles(end-handles.newptcount+1:end))
            delete(handles.im2ptshandles(end-handles.newptcount+1:end))
            handles.im1ptshandles(end-handles.newptcount+1:end)=[];
            handles.im2ptshandles(end-handles.newptcount+1:end)=[];
        end
    end
elseif any(d==[13 27]), % escape presed, quit
    uiresume(handles.HistoStitcher);
end

guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%% CALLBACK (Mouse & Space bar) %%%%%%%%%%%%%%%%%%%%%
% --- Executes on button press in donebutton.
function donebutton_Callback(hObject, eventdata, handles)

if handles.gettingpoints,
    handles=stopgettingpoints(handles);
    if handles.newptcount>0,
        handles.im1pts(end-handles.newptcount+1:end,:)=[];
        handles.im2pts(end-handles.newptcount+1:end,:)=[];
        delete(handles.im1ptshandles(end-handles.newptcount+1:end))
        delete(handles.im2ptshandles(end-handles.newptcount+1:end))
        handles.im1ptshandles(end-handles.newptcount+1:end)=[];
        handles.im2ptshandles(end-handles.newptcount+1:end)=[];
    end
    guidata(hObject, handles);
else
    if ~isempty(handles.im2ptsmoved),
        handles.dolargetrans=true;
        guidata(hObject, handles);
        transimbutton_Callback(hObject, eventdata, handles)
        handles=guidata(hObject);
    end
    uiresume(handles.HistoStitcher);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%% "Calculate Transform" button %%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%% CALLBACK (Mouse & Space bar) %%%%%%%%%%%%%%%%%%%%%
% --- Executes on button press in calcxformbutton.
function calcxformbutton_Callback(hObject, eventdata, handles)
% hObject    handle to calcxformbutton (see GCBO)
% handles    structure with handles and user data (see GUIDATA)

if ~handles.gettingpoints && handles.ptcount>1,
    
%     if handles.movedptshandle,
%         delete(handles.movedptshandle);
%         handles.movedptshandle=0;
%     end
%     handles.allowreflection
%     handles.allowscaling
%     handles.isotropicscale
    
    % im2pts moving, im1pts (stationary)
    if handles.allowreflection && ~handles.isotropicscale && handles.allowscaling  % reflection, scaling, anisotropic
        [M,im2ptsmoved] = lsqaffineh(handles.im2pts',handles.im1pts');
        im2ptsmoved=im2ptsmoved';
        handles.xformtype='Unconstrained Affine (reflection, anisotropic scaling)';
    elseif handles.allowreflection && handles.allowscaling, % reflection, scaling, isotropic
        [D,im2ptsmoved,pxform] = procrustes(handles.im1pts,handles.im2pts);
        MS=diag([pxform.b pxform.b 1]);
        MT=eye(3); MT(:,3)=[pxform.c(1,:) 1]';
        MR=eye(3); MR(1:2,1:2)=pxform.T;
        M=MT*MS*MR'; % MS*MR'*[handles.im2pts ones(3,1)]' == MS*([handles.im2pts ones(3,1)]*MR)'
        handles.xformtype='Unconstrained Procrustes (reflection, isotropic scaling)';
    elseif handles.allowreflection,  % reflection, no scaling
        [D,im2ptsmoved,pxform] = procrustes(handles.im1pts,handles.im2pts);
        pxform.T = pxform.T;
        pxform.b = 1;
        pxform.c = mean(handles.im1pts) - mean(handles.im2pts) * pxform.T;
        % M=pxform.T; M(1:2,3)=pxform.c(1,:)'; M(3,:)=[0 0 1];
        MS=diag([pxform.b pxform.b 1]);
        MT=eye(3); MT(:,3)=[pxform.c(1,:) 1]';
        MR=eye(3); MR(1:2,1:2)=pxform.T;
        M=MT*MS*MR';
        im2ptsmoved = handles.im2pts * pxform.T + repmat(pxform.c,size(handles.im2pts,1),1);
        im2ptsmovedh= [handles.im2pts ones(size(handles.im2pts,1),1)]*M'; %#ok<NASGU> % a check, compare im2ptsmoved vs. im2ptsmovedh
        handles.xformtype='Constrained Procrustes (reflection, NO scaling)';
    else  % no reflection, scaling, isotropic
        [M,im2ptsmoved] = lsqsimilarityh(handles.im2pts',handles.im1pts'); 
        im2ptsmoved=im2ptsmoved';
        handles.xformtype='Similarity Transform (NO reflection, isotropic scaling)';
    end
    handles.M=M;
    handles.im2ptsmoved=im2ptsmoved;
    
    handles.residualerror=mean(sqrt(sum((im2ptsmoved-handles.im1pts).^2,2)));
    
%     axes(handles.im2ax);
%     handles.movedptshandle=plot(handles.im2ptsmoved(:,1),handles.im2ptsmoved(:,2),'r*');
    
    [handles.MR1,handles.MS,handles.MR2,handles.MR1alt,handles.MR2alt, ... 
        handles.theta1, handles.theta2, handles.theta1alt, handles.theta2alt, ...
        handles.MRSRx, handles.MRSRxalt] = affinedecomp(M(1:2,1:2));
    handles.translation=M(1:2,3);
    
    set(handles.edit7,'String',sprintf('%.1f',handles.translation(1)))
    set(handles.edit8,'String',sprintf('%.1f',handles.translation(2)))
    
    scalevectorstring=sprintf('(%.2f,%.2f)',handles.MS(1),handles.MS(2,2));
    set(handles.edit1,'String',sprintf('%.1f',handles.theta1*180/pi))
    set(handles.edit2,'String',scalevectorstring)
    set(handles.edit3,'String',sprintf('%.1f',handles.theta2*180/pi))
    
    set(handles.edit4,'String',sprintf('%.1f',handles.theta1alt*180/pi))
    set(handles.edit5,'String',scalevectorstring)
    set(handles.edit6,'String',sprintf('%.1f',handles.theta2alt*180/pi))
    
    set(handles.edit22,'String',sprintf('%.4f',handles.MS(1)))
    set(handles.edit23,'String',sprintf('%.4f',handles.MS(2,2)))
    
    affelements=[[handles.MRSRx handles.translation]; 0 0 1]';affelements=affelements(:);
    
    for i=11:19, % 11:19 is affine xform
        eval(['set(handles.edit' num2str(i) ',''String'',' sprintf('%.3f',affelements(i-10)) ')']);
    end
    
    set(handles.xformtypetext,'String',handles.xformtype);
    set(handles.residualtext,'String',sprintf('Residual Error: %g px',handles.residualerror));
    set(handles.residualtext,'Visible','on');
    
    guidata(hObject, handles);
end

% --- Executes on button press in isoscalecheckbox.
function isoscalecheckbox_Callback(hObject, eventdata, handles)
handles.isotropicscale=get(hObject,'Value');

% Optional only when both scaling and reflection are allowed
% Required when reflection not allowed
% Not allowed when scaling disabled (obviously)

calcxformbutton_Callback(hObject, eventdata, handles);
handles=guidata(hObject);

guidata(hObject, handles);

% --- Executes on button press in reflectioncheckbox.
function reflectioncheckbox_Callback(hObject, eventdata, handles)
handles.allowreflection=get(hObject,'Value');

% Optional only when scaling allowed
% Required when scaling not allowed

if ~handles.allowreflection && handles.allowscaling, % scaling must be isomorphic with no reflection
    set(handles.isoscalecheckbox,'Value',1);
    set(handles.isoscalecheckbox,'Enable','off');
    handles.isotropicscale=1;
elseif handles.allowreflection && handles.allowscaling, % with reflection and scaling, scaling can be anisomorphic
    set(handles.isoscalecheckbox,'Enable','on');
end

calcxformbutton_Callback(hObject, eventdata, handles);
handles=guidata(hObject);

guidata(hObject, handles);

% --- Executes on button press in allowscalingcheckbox.
function allowscalingcheckbox_Callback(hObject, eventdata, handles)
handles.allowscaling=get(hObject,'Value');

% Always optional
% When off, reflection required and isotropic disabled
% When on, reflection optional and isotropic optional but required without reflection 

if ~handles.allowscaling,
    set(handles.isoscalecheckbox,'Value',0);
    set(handles.isoscalecheckbox,'Enable','off');
    handles.isotropicscale=0;
    set(handles.reflectioncheckbox,'Value',1);
    set(handles.reflectioncheckbox,'Enable','off');
    handles.allowreflection=1;
else
    set(handles.reflectioncheckbox,'Enable','on');
    if handles.allowreflection,
        set(handles.isoscalecheckbox,'Enable','on');
    end
end

calcxformbutton_Callback(hObject, eventdata, handles);
handles=guidata(hObject);

guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% Mouse Motion & Cursor Style %%%%%%%%%%%%%%%%%%%%%%%%

% --- Executes on mouse motion over figure - except title and menu.
function HistoStitcher_WindowButtonMotionFcn(hObject, eventdata, handles)
% hObject    handle to HistoStitcher (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.gettingpoints,
    scrn_pt = get(0, 'PointerLocation');
    set(hObject,'units','pixels')
    loc = get(handles.HistoStitcher, 'Position');
    
    if handles.activeaxis==1,
        set(handles.im1ax,'units','pixels')
        axloc=get(handles.im1ax,'Position');
    elseif handles.activeaxis==2,
        set(handles.im2ax,'units','pixels')
        axloc=get(handles.im2ax,'Position');
    else % no active axis?
        fprintf('Weirdness.\n');
        return
    end
    scrn_axloc=[loc(1:2) loc(1:2)+axloc(1:2)]+axloc;
    
    if scrn_pt(1)>scrn_axloc(1) && scrn_pt(1)<scrn_axloc(3) && ...
            scrn_pt(2)>scrn_axloc(2) && scrn_pt(2)<scrn_axloc(4),
        set(gcf,'pointer','cross');
        handles.validposition=1;
        handles.scrn_axloc=scrn_axloc;
    else
        set(gcf,'pointer','arrow');
        handles.validposition=0;
    end
guidata(hObject, handles);    
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% Control Point Selection Mode %%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Left Image %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on mouse press over axes background.
function im1ax_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to im1ax (see GCBO)
% handles    structure with handles and user data (see GUIDATA)

if handles.gettingpoints && handles.activeaxis==1 && handles.validposition,
    
    handles.ptcount=handles.ptcount+1;
    handles.newptcount=handles.newptcount+1;
    
    pt=get(handles.im1ax,'CurrentPoint');
    handles.im1pts(handles.ptcount,:)=pt(1,1:2);
    
    hold on;
    handles.im1ptshandles(handles.ptcount) = ...
        plot(handles.im1pts(handles.ptcount,1)+1, handles.im1pts(handles.ptcount,2)+1,'b*');
    
    handles.activeaxis=2;
    handles.validposition=0;
    
else
    if ishandle(handles.im1fullfig),
        figure(handles.im1fullfig);
    else
        handles.im1fullfig=figure;
    end
    imagesc(cast(handles.im1,handles.im1class));
    axis off equal tight;
    if size(handles.im1,3)==1, colormap gray; end
    title('Full size image 1. Use toolbar to zoom.');
end

guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Right Image %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on mouse press over axes background.
function im2ax_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to im2ax (see GCBO)
% handles    structure with handles and user data (see GUIDATA)

if handles.gettingpoints && handles.activeaxis==2 && handles.validposition,
    
    pt=get(handles.im2ax,'CurrentPoint');
    handles.im2pts(handles.ptcount,:)=pt(1,1:2);
    
    hold on;
    handles.im2ptshandles(handles.ptcount) = ...
        plot(handles.im2pts(handles.ptcount,1)+1, handles.im2pts(handles.ptcount,2)+1,'g*');
    
    handles.activeaxis=1;
    handles.validposition=0;

else
    if ishandle(handles.im2fullfig),
        figure(handles.im2fullfig);
    else
        handles.im2fullfig=figure;
    end
    imagesc(cast(handles.im2,handles.im2class));
    axis off equal tight;
    if size(handles.im2,3)==1, colormap gray; end
    title('Full size image 1. Use toolbar to zoom.');
end

guidata(hObject, handles);


% --- Executes on button press in clearptsbutton.
function clearptsbutton_Callback(hObject, eventdata, handles)
% Reset button

handles.gettingpoints=0;
handles.activeaxis=0;
handles.im1pts=[];
handles.im2pts=[];
handles.ptcount=0;
handles.newptcount=0;
handles.im1pts=[];
handles.im2pts=[];
handles.im2ptsmoved=[];
handles.im2def=[];
if ~isempty(handles.im1ptshandles) && ~isempty(handles.im2ptshandles), 
delete(handles.im1ptshandles)
delete(handles.im2ptshandles)
end
% if handles.movedptshandle,
%     delete(handles.movedptshandle); end
handles.im1ptshandles=[];
handles.im2ptshandles=[];
% handles.movedptshandle=0;
if ishandle(handles.im2deffigh),
close(handles.im2deffigh);
handles.im2deffigh=[];
end

[handles.MR1,handles.MS,handles.MR2,handles.MR1alt,handles.MR2alt, ... 
        handles.theta1, handles.theta2, handles.theta1alt, handles.theta2alt, ...
        handles.MRSRx, handles.MRSRxalt,handles.translation]=deal([]);

set(handles.getcpsbutton,'String','Add Points')
set(handles.calcxformbutton,'Enable','off')
set(handles.transimbutton,'Enable','off')
set(handles.residualtext,'Visible','off')

% Clear transformation text boxes
for i=[1:8 22:23 11:19], % 1:8 22:23 is affine decompotition, 11:19 is affine xform
    eval(['set(handles.edit' num2str(i) ',''String'','''')']);
end

guidata(hObject, handles);

% --- Executes on button press in transimbutton.
function transimbutton_Callback(hObject, eventdata, handles)

% Ensure that the transform is up to date
calcxformbutton_Callback(hObject, eventdata, handles);
handles=guidata(hObject);

% Disable the GUI while the transformation is being applied
transimagebuttontext=get(handles.transimbutton,'String');
fprintf('Wait...'); set(handles.transimbutton,'Enable','off'); set(handles.transimbutton,'String','Wait...');
drawnow

% Perform the full-size transformation for the overlay
global im12overlay

if handles.dolargetrans,
    fprintf('Large transformation.\n');
    % Scale up the transformation for the full size image
    handles.M(1:2,3)=handles.M(1:2,3)/handles.fraction(1);
    
    edge2moved=round(xformcps([handles.edgej2 handles.edgei2]',handles.M));
    leftedge=min(edge2moved(1,:)); rightedge=max(edge2moved(1,:));
    topedge=min(edge2moved(2,:)); bottomedge=max(edge2moved(2,:));
    
    im2width=size(handles.im2,2); im2height=size(handles.im2,1);
    paddedwidth=max([rightedge size(handles.im2,2)])-min([leftedge 1])+1;
    paddedheight=max([bottomedge size(handles.im2,1)])-min([topedge 1])+1;
    
    originoffset=[min([topedge-1 0]) min([leftedge-1 0])]-2; % y,x
    
    im2padded=repmat(single(permute(handles.bgmedian1,[1 3 2])),[paddedheight paddedwidth]);
    im2padded((1:im2height)-originoffset(1),(1:im2width)-originoffset(2),:)=single(handles.im2);
    
    bgmask2padded=false([paddedheight paddedwidth]);
    bgmask2padded((1:im2height)-originoffset(1),(1:im2width)-originoffset(2),:)=handles.bgmask2;
    
    [im12overlay] = imdef(handles.M,single(im2padded),'*nearest',handles.bgmedian1,[],[size(im2padded,1)/2 size(im2padded,2)/2]+originoffset);
    [bgmask2def] = imdef(handles.M,single(bgmask2padded),'*nearest',0,[],[size(im2padded,1)/2 size(im2padded,2)/2]+originoffset);
    bgmask2def=logical(bgmask2def);
    
    % Full size transformation
    % [im12overlay,~,~,~,originoffset] = imdeffull(handles.M,handles.im2,'*nearest',handles.bgmedian1,[],[size(handles.im2,1)/2 size(handles.im2,2)/2]);
    % [bgmask2def,~,~,~,originoffset] = imdeffull(handles.M,single(handles.bgmask2),'*nearest',false,[],[size(handles.im2,1)/2 size(handles.im2,2)/2]);
    % bgmask2def=logical(bgmask2def);
%     im2defmask=any(im12overlay>eps,3);
    
    % Also produce the same size output (when "Done" is pressed)
    handles.im2def=imdef(handles.M,handles.im2,'*nearest',[],[],[size(handles.im2,1)/2 size(handles.im2,2)/2]);
else
    fprintf('Small transformation.\n');
    [im12overlay,~,~,~,originoffset] = imdeffull(handles.M,handles.im2disp,'*nearest',handles.bgmedian1,[],[size(handles.im2disp,1)/2 size(handles.im2disp,2)/2]);
    [im12overlayNN,~,~,~,originoffset] = imdeffull(handles.M,handles.im2dispNN,'*nearest',handles.bgmedian1,[],[size(handles.im2disp,1)/2 size(handles.im2disp,2)/2]);
    [bgmask2def,~,~,~,originoffset] = imdeffull(handles.M,single(handles.bgmask2disp),'*nearest',false,[],[size(handles.im2disp,1)/2 size(handles.im2disp,2)/2]);
    bgmask2def=logical(bgmask2def);
    % handles.im2def=imdef(handles.M,handles.im2);
%     im2defmask=any(im12overlayNN>eps,3);
end

% Save the position of the origin in the new larger image
handles.originoffset=originoffset;

% Make overlay/stitched image (image 2 + image 1)
aooy=abs(originoffset(1)); aoox=abs(originoffset(2));
if handles.dolargetrans,
    % Full sized image 1
    y1=size(handles.im1,1); x1=size(handles.im1,2);
%     im1mask=any(handles.im1>eps,3);
    
    % Insert image 1 into the correct position
    bgmask1full=false(size(bgmask2def));
    bgmask1full(aooy+(1:y1), aoox+(1:x1))=handles.bgmask1;
    
    overlapmask=bgmask2def & bgmask1full;
    overlapmask_im1=overlapmask(aooy+(1:y1), aoox+(1:x1));
    
    nonoverlap_im1=handles.bgmask1 & ~overlapmask_im1;
    nonoverlap_im1full=bgmask1full & ~bgmask2def;
    % nonoverlap_im2=~bgmask1full & bgmask2def;
    
    overlapmask=repmat(overlapmask,[1 1 size(im12overlay,3)]);
    overlapmask_im1=repmat(overlapmask_im1,[1 1 size(im12overlay,3)]);
    nonoverlap_im1full=repmat(nonoverlap_im1full,[1 1 size(im12overlay,3)]);
    nonoverlap_im1=repmat(nonoverlap_im1,[1 1 size(im12overlay,3)]);
    
    % blend overlap
    im12overlay(overlapmask)=(im12overlay(overlapmask)+handles.im1(overlapmask_im1))/2;
    % don't blend for non-overlap (im1)
    im12overlay(nonoverlap_im1full)=handles.im1(nonoverlap_im1);
    
%     for i=1:size(im12overlay,3),
%         im12overlayi=im12overlay(:,:,i);
%         % blend overlap
%         im12overlayi(overlapmask)=(im12overlayi(overlapmask)+handles.im1(overlapmask_im1))/2;
%         % don't blend for non-overlap
%         im12overlayi(nonoverlap_im1full)=handles.im1(nonoverlap_im1);
%         im12overlay(:,:,i)=im12overlayi;
% %         im12overlay(aooy+(1:y1), aoox+(1:x1), :)=im12overlay(aooy+(1:y1), aoox+(1:x1), :)+handles.im1;
%     end
else
    % Display/preview sized image 1
    y1=size(handles.im1disp,1); x1=size(handles.im1disp,2);
%     im1mask=any(handles.im1dispNN>eps,3);
    
    % Insert image 1 into the correct position
    bgmask1full=false(size(bgmask2def));
    bgmask1full(aooy+(1:y1), aoox+(1:x1))=handles.bgmask1disp;
    
    overlapmask=bgmask2def & bgmask1full;
    overlapmask_im1=overlapmask(aooy+(1:y1), aoox+(1:x1));
    
    nonoverlap_im1=handles.bgmask1disp & ~overlapmask_im1;
    nonoverlap_im1full=bgmask1full & ~bgmask2def;
    % nonoverlap_im2=~bgmask1full & bgmask2def;
    
    overlapmask=repmat(overlapmask,[1 1 size(im12overlay,3)]);
    overlapmask_im1=repmat(overlapmask_im1,[1 1 size(im12overlay,3)]);
    nonoverlap_im1full=repmat(nonoverlap_im1full,[1 1 size(im12overlay,3)]);
    nonoverlap_im1=repmat(nonoverlap_im1,[1 1 size(im12overlay,3)]);
    
    % blend overlap
    im12overlay(overlapmask)=(im12overlay(overlapmask)+handles.im1disp(overlapmask_im1))/2;
    % don't blend for non-overlap (im1)
    im12overlay(nonoverlap_im1full)=handles.im1disp(nonoverlap_im1);
    
%     for i=1:size(im12overlay,3),
%         im12overlayi=im12overlay(:,:,i);
%         % blend overlap
%         im12overlayi(overlapmask)=(im12overlayi(overlapmask)+handles.im1disp(overlapmask_im1))/2;
%         % don't blend for non-overlap
%         im12overlayi(nonoverlap_im1full)=handles.im1disp(nonoverlap_im1);
%         im12overlay(:,:,i)=im12overlayi;
% %         im12overlay(aooy+(1:y1), aoox+(1:x1), :)=im12overlay(aooy+(1:y1), aoox+(1:x1), :)+handles.im1;
%     end
end

% im1fullmask=false(size(im2defmask));
% im1fullmask(aooy+(1:y1), aoox+(1:x1))=im1mask;

% Scale overlaping regions
% nzdims=size(im12overlay,3);
% im12mask=im2defmask & im1fullmask;
% % whereoverlap=repmat(any(im12overlay>1,3) | im12mask,[1 1 nzdims]);
% whereoverlap=repmat(im12mask,[1 1 nzdims]);
% im12overlay(whereoverlap)=im12overlay(whereoverlap)/2;
% im12overlay(im12overlay>1)=1;
% % im12overlay(repmat(any(im12overlay>1,3),[1 1 nzdims]))
% % im12overlay=im12overlay/2;

% Open a figure for the overlay
if ishandle(handles.im2deffigh),
    figure(handles.im2deffigh);
    hold off;
else
    handles.im2deffigh=figure;
end

% Display the overlay
im12overlay=cast(im12overlay,handles.im1class);
maxval=max([handles.im1range(2) handles.im2range(2)]);
im12overlay(im12overlay>maxval)=maxval;
imagesc(im12overlay);
axis off equal tight;
if size(im12overlay,3)==1, colormap gray; end
hold on;

% Transform the control points as if they were on the large/full image 2
if handles.dolargetrans,
    handles.im2pts=handles.im2pts/handles.fraction(1);
    handles.im1pts=handles.im1pts/handles.fraction(1);
    im2ptsmovedh=[handles.im2pts ones(size(handles.im2pts,1),1)]*handles.M';
    handles.im2ptsmoved=im2ptsmovedh(:,1:2);
end

% Add control points onto the overlay
plot(handles.im2ptsmoved(:,1)-originoffset(2),handles.im2ptsmoved(:,2)-originoffset(1),'r*');
plot(handles.im1pts(:,1)-originoffset(2),handles.im1pts(:,2)-originoffset(1),'b*');
title('Overlay of Image 2 Deformed by Affine Transformation');
set(handles.im2deffigh,'Color',get(0,'defaultUicontrolBackgroundColor'),...
   'ToolBar','none')

% Restore GUI functionality
set(handles.transimbutton,'Enable','on'); set(handles.transimbutton,'String',transimagebuttontext);
guidata(hObject, handles);


%%#########################################################################
%%%%%%%%%%%%%%%%%%%% Nested Functions (Not Callbacks) %%%%%%%%%%%%%%%%%%%%%
%##########################################################################

%%%%%%%%%%%%%%%%%%%%%%%%%%%% Stop Getting Points %%%%%%%%%%%%%%%%%%%%%%%%%%
function handles=stopgettingpoints(handles)

set(handles.HistoStitcher,'pointer','arrow');
handles.gettingpoints=0;

% set(handles.getcpsbutton,'Enable','on')
set(handles.getcpsbutton,'String','Add Points')

if handles.activeaxis==2, % only selcted first point on image 1 (not a pair)
    handles.im1pts(end,:)=[];
    delete(handles.im1ptshandles(end))
    handles.im1ptshandles(end)=[];
    handles.ptcount=handles.ptcount-1;
    handles.newptcount=handles.newptcount-1;
end

if size(handles.im1pts,1)>1,
    set(handles.calcxformbutton,'Enable','on')
    set(handles.transimbutton,'Enable','on')
end

handles.activeaxis=0;

set(handles.donebutton,'String','Done')

guidata(handles.HistoStitcher, handles);


% --- NOTHING IMPORTANT IS BELOW

function edit1_Callback(hObject, eventdata, handles)
% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double

% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function edit7_Callback(hObject, eventdata, handles)


% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit8_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit8_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit4_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit4_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit5_Callback(hObject, eventdata, handles) %#ok<*DEFNU,*INUSD>

% --- Executes during object creation, after setting all properties.
function edit5_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit6_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit6_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit2_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit3_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit11_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit11_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit12_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit12_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit13_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit13_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit14_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit14_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit15_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit15_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit16_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit16_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function edit17_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit17_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit18_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit18_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit19_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit19_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit10_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit10_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit22_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit22_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function edit23_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function edit23_CreateFcn(hObject, eventdata, handles)

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in xreflect.
function xreflect_Callback(hObject, eventdata, handles)
% hObject    handle to xreflect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of xreflect


% --- Executes on button press in yreflect.
function yreflect_Callback(hObject, eventdata, handles)
% hObject    handle to yreflect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of yreflect


% --- Executes on button press in savebutton.
function savebutton_Callback(hObject, eventdata, handles)
% hObject    handle to savebutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in loadbutton.
function loadbutton_Callback(hObject, eventdata, handles)
% hObject    handle to loadbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
