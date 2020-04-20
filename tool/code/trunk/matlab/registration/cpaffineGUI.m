function varargout = cpaffineGUI(varargin)
% CPAFFINEGUI Place control points and calculate affine/linear transform.
%   CPAFFINEGUI(IM1,IM2), allows interactive selection of control points
%   on a pair of images, followed by calculation of the necessary affine
%   transformation to minimize the sum of squared distances between the
%   transformed points.
%
%   M = CPAFFINEGUI(...) returns the homogeneous affine transformation 
%   matrix necessary to map the control points on image B towards the
%   corresponding points on image A.
%
%   [M,IM1PTS,IM2PTS,IM2PTSMOVED] = CPAFFIENGUI(...) returns the manually
%   placed control points, IM1PTS and IM2PTS, as well as the transformed
%   control points of IM2.
%
%   [M,IM1PTS,IM2PTS,IM2PTSMOVED,XFORM] = CPAFFIENGUI(...) returns the
%   structure XFORM containing the transormation matrix, the decomposed
%   rotation and scaling matrices, the translations, and the rotation
%   angles.
%
%   [M,IM1PTS,IM2PTS,IM2PTSMOVED,XFORM,OO,IM12OVERLAY] = CPAFFIENGUI(...) 
%   returns the origin offset OO.  This is the location of the original top
%   left corner of IM2 in the overlay image, IM12OVERLAY.
%
%   [..,IM12OVERLAY,IM2DEF] = CPAFFIENGUI(...) returns the transformed IM2
%   in the eigth (8th) output argument.  This image IM2DEF is the size of
%   the input images (i.e. if they are differnet sizes, the maximum on each
%   dimension).
%
%   For example:
%    [M,im1pts,im2pts,im2ptsmoved,xform,oo,im12overlay,im2def]=cpaffineGUI;
%
% See also: LSQAFFINEH, LSQSIMILARITYH, AFFINEDECOMP, PROCRUSTES, RESCALE.
%
%JC

% Last Modified by GUIDE v2.5 04-Aug-2010 12:22:28

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @cpaffineGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @cpaffineGUI_OutputFcn, ...
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
% --- Executes just before cpaffineGUI is made visible.
function cpaffineGUI_OpeningFcn(hObject, eventdata, handles, varargin) %#ok<*INUSL>
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to cpaffineGUI (see VARARGIN)

% Make a global for the overlay since it could be huge
clear global im12overlay
global im12overlay %#ok<NUSED>

% Choose default command line output for cpaffineGUI
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

fprintf('Padding images to same size... ');
padsize=[max(size(handles.im1,1),size(handles.im2,1)) max(size(handles.im1,2),size(handles.im2,2))];
if isa(handles.im1,'float'),
    fprintf('float data... ');
    handles.im1=impad(handles.im1,padsize); % rescale accepts singles without conversion, but warns
    handles.im2=impad(handles.im2,padsize);
else
    fprintf('converting integer to single... ');
%     handles.im1=single(rescale(impad(handles.im1,padsize))); % rescale converts integers to double for maximum accuracy
%     handles.im2=single(rescale(impad(handles.im2,padsize)));
    handles.im1=single(impad(handles.im1,padsize)); % rescale accepts singles without conversion, but warns
    handles.im2=single(impad(handles.im2,padsize));
end
fprintf('done.\n');

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

% UIWAIT makes cpaffineGUI wait for user response (see UIRESUME)
uiwait(handles.cpaffineGUI);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% OUTPUTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Outputs from this function are returned to the command line.
function varargout = cpaffineGUI_OutputFcn(hObject, eventdata, handles) 
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
    imdisp(im12overlay)
    title('Image overlay');
else
    warndlg('Ouput image empty!','No Output Image');
end
varargout{7} = im12overlay;
varargout{8} = handles.im2def;

delete(handles.cpaffineGUI);

clear global im12overlay

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CLOSING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes when user attempts to close cpaffineGUI.
function cpaffineGUI_CloseRequestFcn(hObject, eventdata, handles)

if isequal(get(handles.cpaffineGUI, 'waitstatus'), 'waiting')
    uiresume(handles.cpaffineGUI);
else
    delete(handles.cpaffineGUI);
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%% KEYBOARD PRESSED WITH NOTHING SELECTED %%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on key press with focus on cpaffineGUI and no controls selected.
function cpaffineGUI_KeyPressFcn(hObject, eventdata, handles)

if handles.gettingpoints,
    % isequal(get(hObject,'CurrentKey'),'escape')
    d = double(get(handles.cpaffineGUI, 'currentcharacter'));
    if any(d==27), % 27=escape, 13=Return, 32=space
    	handles=stopgettingpoints(handles);
    end
elseif isequal(get(hObject,'CurrentKey'),'escape')
    uiresume(handles.cpaffineGUI);
end

guidata(hObject, handles);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%% "Choose Points/Enough" button %%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% KEYBOARD  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on key press with focus on getcpsbutton and no controls selected.
function getcpsbutton_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to getcpsbutton (see GCBO)
% handles    structure with handles and user data (see GUIDATA)

d = double(get(handles.cpaffineGUI, 'currentcharacter'));
if handles.gettingpoints, % stop getting points
    if any(d==[13 27]), % 27=escape, 13=Return, 32=space
    	handles=stopgettingpoints(handles);
    end
elseif any(d==13), % start getting points
    getcpsbutton_Callback(hObject, eventdata, handles);
    return
elseif d==27, % escape presed, quit
    uiresume(handles.cpaffineGUI);
end

guidata(hObject, handles);

%%%%%%%%%%%%%%%%%%%%%%%% CALLBACK (Mouse & Space bar) %%%%%%%%%%%%%%%%%%%%%
% --- Executes on button press in getcpsbutton.
function getcpsbutton_Callback(hObject, eventdata, handles)
% hObject    handle to getcpsbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.gettingpoints==1, % stop getting points
    handles=stopgettingpoints(handles);
else % start getting points
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
    set(handles.removeptsbutton,'Enable','off')
    set(handles.savebutton,'Enable','off')

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

d = double(get(handles.cpaffineGUI, 'currentcharacter'));
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
            handles.newptcount=0;
        end
    end
elseif any(d==[13 27]), % escape presed, quit
    uiresume(handles.cpaffineGUI);
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
        handles.newptcount=0;
    end
    guidata(hObject, handles);
else
    if ~isempty(handles.im2ptsmoved),
        handles.dolargetrans=true;
        guidata(hObject, handles);
        transimbutton_Callback(hObject, eventdata, handles)
        handles=guidata(hObject);
    end
    uiresume(handles.cpaffineGUI);
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
        [D,im2ptsmoved,pxform] = procrustes(handles.im1pts,handles.im2pts); %#ok<ASGLU>
        MS=diag([pxform.b pxform.b 1]);
        MT=eye(3); MT(:,3)=[pxform.c(1,:) 1]';
        MR=eye(3); MR(1:2,1:2)=pxform.T;
        M=MT*MS*MR'; % MS*MR'*[handles.im2pts ones(3,1)]' == MS*([handles.im2pts ones(3,1)]*MR)'
        handles.xformtype='Unconstrained Procrustes (reflection, isotropic scaling)';
    elseif handles.allowreflection,  % reflection, no scaling
        [D,im2ptsmoved,pxform] = procrustes(handles.im1pts,handles.im2pts); %#ok<ASGLU>
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
function cpaffineGUI_WindowButtonMotionFcn(hObject, eventdata, handles)
% hObject    handle to cpaffineGUI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.gettingpoints,
    scrn_pt = get(0, 'PointerLocation');
    set(hObject,'units','pixels')
    loc = get(handles.cpaffineGUI, 'Position');
    
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
set(handles.removeptsbutton,'Enable','off')
set(handles.savebutton,'Enable','off')

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
    
    % Full size transformation
    [im12overlay,~,~,~,originoffset] = imdeffull(handles.M,handles.im2,'*linear',[],[],[size(handles.im2,1)/2 size(handles.im2,2)/2]);
%     im2defmask=any(im12overlay>eps,3);
    
    % Also produce the same size output (when "Done" is pressed)
    handles.im2def=imdef(handles.M,handles.im2,'*nearest',[],[],[size(handles.im2,1)/2 size(handles.im2,2)/2]);
else
    fprintf('Small transformation.\n');
    [im12overlay,~,~,~,originoffset] = imdeffull(handles.M,handles.im2disp,'*linear',[],[],[size(handles.im2disp,1)/2 size(handles.im2disp,2)/2]);
    [im12overlayNN,~,~,~,originoffset] = imdeffull(handles.M,handles.im2dispNN,'*linear',[],[],[size(handles.im2disp,1)/2 size(handles.im2disp,2)/2]);
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
    
    % Insert image 1 into the correct position
    im12overlay(aooy+(1:y1), aoox+(1:x1), :)=im12overlay(aooy+(1:y1), aoox+(1:x1), :)+handles.im1;
else
    % Display/preview sized image 1
    y1=size(handles.im1disp,1); x1=size(handles.im1disp,2);
    
    % Insert image 1 into the correct position
    im12overlay(aooy+(1:y1), aoox+(1:x1), :)=im12overlay(aooy+(1:y1), aoox+(1:x1), :)+handles.im1disp;
end

% Scale overlaping regions
% nzdims=size(im12overlay,3);
% whereoverlap=repmat(any(im12overlay>1,3) | im12mask,[1 1 nzdims]);
% im12overlay(whereoverlap)=im12overlay(whereoverlap)/2;
% im12overlay(im12overlay>1)=1;
% im12overlay(repmat(any(im12overlay>1,3),[1 1 nzdims]))
im12overlay=im12overlay/2;

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


% --- Executes on button press in removeptsbutton.
function removeptsbutton_Callback(hObject, eventdata, handles)
% hObject    handle to removeptsbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.ptcount>0,
    
    removalongoing=true;
    while removalongoing,
        cpinfo={};
        for i=1:handles.ptcount,
            cpinfo{i}=['Pair ' num2str(i)]; %#ok<AGROW>
        end
    
        [selectedpt,hitremove]=listdlg('Name','Edit Points','PromptString','Select a pair of control points',...
            'SelectionMode','single','ListSize',[170 120],...
            'OKString','Remove','CancelString','Close','ListString',cpinfo);
        drawnow;
        
        if hitremove,
            mh1=handles.im1ptshandles(selectedpt);
            mh2=handles.im2ptshandles(selectedpt);
            msize1=get(mh1,'MarkerSize'); mcolor1=get(mh1,'MarkerEdgeColor');
            msize2=get(mh2,'MarkerSize'); mcolor2=get(mh2,'MarkerEdgeColor');
            set(mh1,'MarkerSize',12,'MarkerEdgeColor','red');
            set(mh2,'MarkerSize',12,'MarkerEdgeColor','red');
            removecolorq = questdlg('Remove the highlighted (red) point?', ...
                'Remove Point?', 'Yes', 'No', 'Yes'); drawnow;
            
            if ~isempty(removecolorq) && strcmp(removecolorq,'Yes'),
                handles.im1pts(selectedpt,:)=[];
                delete(handles.im1ptshandles(selectedpt));
                handles.im1ptshandles(end)=[];
                handles.im2pts(selectedpt,:)=[];
                delete(handles.im2ptshandles(selectedpt));
                handles.im2ptshandles(end)=[];
                handles.ptcount=handles.ptcount-1;
                handles.newptcount=handles.newptcount-1;
                guidata(hObject, handles);
                if handles.ptcount<1,
                    removalongoing=false;
                    clearptsbutton_Callback(hObject, eventdata, handles);
                else
                    calcxformbutton_Callback(hObject, eventdata, handles);
                end
                handles=guidata(hObject);
            else
                set(mh1,'MarkerSize',msize1,'MarkerEdgeColor',mcolor1);
                set(mh2,'MarkerSize',msize2,'MarkerEdgeColor',mcolor2);
            end
        else
            removalongoing=false;
        end
    end
else
    fprintf('This button should be disabled right now.\n');
end

guidata(hObject, handles);

%%#########################################################################
%%%%%%%%%%%%%%%%%%%% Nested Functions (Not Callbacks) %%%%%%%%%%%%%%%%%%%%%
%##########################################################################

%%%%%%%%%%%%%%%%%%%%%%%%%%%% Stop Getting Points %%%%%%%%%%%%%%%%%%%%%%%%%%
function handles=stopgettingpoints(handles)

set(handles.cpaffineGUI,'pointer','arrow');
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
    set(handles.removeptsbutton,'Enable','on')
    set(handles.savebutton,'Enable','on')
end

handles.activeaxis=0;

set(handles.donebutton,'String','Done')

guidata(handles.cpaffineGUI, handles);


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

cl=clock;
[filename, pathname] = uiputfile('*.mat', 'Save points as',sprintf('cpoints_%04.0f%02.0f%02.0f-%02.0f.%02.0f.mat',cl(1:5)));

if isequal(filename,0) || isequal(pathname,0), return; end
im1pts=handles.im1pts; im2pts=handles.im2pts; %#ok<NASGU>
fraction=handles.fraction;
im1pts_orig=im1pts/fraction; im2pts_orig=im2pts/fraction;
Iorigsize=size(handles.im1); Idispsize=size(handles.im1disp);
save(fullfile(pathname,filename),'im1pts','im2pts','im1pts_orig','im2pts_orig','Iorigsize','Idispsize','fraction');


% --- Executes on button press in loadbutton.
function loadbutton_Callback(hObject, eventdata, handles)
% hObject    handle to loadbutton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.ptcount>0,
    loadq = questdlg('This will remove existing points.  Are you sure?', ...
    	'Load Points?', 'Yes', 'No', 'Yes'); drawnow;
    if strcmp(loadq,'No'), return; end
end

[filename, pathname] = uigetfile('*.mat', 'Pick an MAT-file to load');

if isequal(filename,0) || isequal(pathname,0), return; end

clearptsbutton_Callback(hObject, eventdata, handles);
handles=guidata(hObject);

loadedpoints=load(fullfile(pathname, filename),'im1pts','im2pts','Iorigsize','Idispsize','fraction');
if ~isfield(loadedpoints,'im1pts') || ~isfield(loadedpoints,'im1pts'),
    warndlg('Selected file does not contain required variables, im1pts and im2pts.','Variables not present','modal');
    return;
end

if ~isequal(loadedpoints.Iorigsize(1:2),[size(handles.im1,1) size(handles.im1,2)]),
    warndlg('Original image sizes not equal.', 'Image sizes not equal', 'modal');
end
if ~isequal(loadedpoints.fraction,handles.fraction),
    warndlg('Control points selected on different display size, trying to adjust.', 'Adjusting control points for new display', 'modal');
    fractionratio=loadedpoints.fraction/handles.fraction; % LARGER dispsize -> ratio<1, SMALLER dispsize -> ratio>1
    loadedpoints.im1pts=loadedpoints.im1pts*fractionratio;
    loadedpoints.im2pts=loadedpoints.im2pts*fractionratio;
end

handles.im1pts=loadedpoints.im1pts;
handles.im2pts=loadedpoints.im2pts;
handles.ptcount=size(loadedpoints.im2pts,1);

hold(handles.im1ax,'on');
hold(handles.im2ax,'on');
for i=1:handles.ptcount,
    handles.im1ptshandles(i) = plot(handles.im1ax,handles.im1pts(i,1), handles.im1pts(i,2),'b*');
    handles.im2ptshandles(i) = plot(handles.im2ax,handles.im2pts(i,1), handles.im2pts(i,2),'b*');
end

set(handles.calcxformbutton,'Enable','on')
set(handles.transimbutton,'Enable','on')
set(handles.removeptsbutton,'Enable','on')
set(handles.savebutton,'Enable','on')

calcxformbutton_Callback(hObject, eventdata, handles);
handles=guidata(hObject);

guidata(hObject, handles);
