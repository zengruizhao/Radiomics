function hf = subdispimg3(varargin)

if nargin>=3 && isa(varargin{3},'function_handle')
    feval(varargin{3},varargin{4:end});
    return;
else
    img = varargin{1};
end

hf = figure;
fpos=get(hf,'Position');
fpos=fpos.*[.5 .5 2 1.5];
set(hf,'Position',fpos);
img8=uint8(round(255*rescale(img)));
subplot(121)
h=image(img(:,:,1)); axis image; colormap gray
title('Unwindowed');
h=get(h,'Parent'); % axis
subplot(122);
imdisp(img(:,:,1), gcf);
title('Windowed, interactive');
xlabel(['max(abs()): ' num2str(max(max(abs(img(:,:,1)))))])
slider = uicontrol('Style', 'slider',...
       'Max',size(img,3),'Min',1,...
       'Units', 'normalized', ...
       'Position', [.25 .005 .4 .04],...
       'SliderStep', [1/(size(img,3)-1) .5], ...
       'Value', 1, ...
       'Callback', {@dispimg3,@move_slider});
% set(slider,'Units','pixels');
% newsize=get(slider,'Position'); newsize(4)=20;
% set(slider,'Position',newsize);
text_box = uicontrol('Style', 'text',...
       'Units', 'normalized', ...
       'Position', [.675 .006 .04 .04],...
       'String', '1');
udata.img = img;
udata.img8 = img8;
udata.text_box = text_box;
udata.hax=h;
set(gcf,'UserData',udata);

   
function move_slider
udata = get(gcf,'UserData');
img = udata.img;
img8 = udata.img8;
val = round(get(gcbo,'Value'));
set(gcbo,'Value',val);
set(udata.text_box,'String',num2str(val));
save_xlim = xlim;
save_ylim = ylim;
subplot(121);
image(img8(:,:,val),'Parent',udata.hax); axis image; colormap gray
title('Unwindowed');
subplot(122);
imdisp(img(:,:,val),gcf);
title('Windowed, interactive');
% xlabel(['max: ' num2str(max(max(img(:,:,val))))])
xlabel(['max(abs()): ' num2str(max(max(abs(img(:,:,val)))))])
xlim(save_xlim);
ylim(save_ylim);