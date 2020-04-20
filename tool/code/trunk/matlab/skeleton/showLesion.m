
% info is a black and white mask image
% image is the image to display explicit shape descriptors

%flip and morph images
lesion = edge(info);
options=[];

if ~isfield(options, 'shape'), options.shape         = 'diamond'  ; end
if ~isfield(options, 'nhood'), options.nhood         = 0          ; end
if ~isfield(options, 'scale'), options.scale         = true       ; end
 
se=strel(options.shape,options.nhood);

% [LOI]=repmat(flipud(imdilate(lesion(:,:),se)),[1 1 3]);
LOI=double(lesion);
% IOI=double(repmat(flipud(image(:,:)),[1 1 3]));
IOI=double(image);

if ~isfield(options, 'ut'), options.ut         =max(IOI(:))       ; end
if ~isfield(options, 'lt'), options.lt         =min(IOI(:))       ; end

IOI(IOI>options.ut)=options.ut;
IOI(IOI<options.lt)=options.lt;

%scale intensity
if options.scale
  IOIscale= (IOI-min(IOI(:)))./(range(IOI(:)));
else
  IOIscale=IOI;
end% ig options.scale

%display lesion boundary in red
LOI=LOI>0;
IOIscale(LOI)=0;

figure;
h2=imshow(IOIscale);
lesionMask = info;

%get medial axis
temp_skel=get_skeleton(logical(lesionMask),logical(LOI(:,:,1)),[1 1 0],100,2,2);
hold on;

for i=1:length(temp_skel.atom)
    temp_skel.atom{i}.location=temp_skel.atom{i}.location;
    temp_skel.atom{i}.radius=temp_skel.atom{i}.radius;
    %if (temp_skel.atom{i}.radius)>5 -- uncomment this if you want the
    %skeleton to be more pruned
        %plot the medial atom in magenta
        %plot the surface vectors in blue
        for j=1:size(temp_skel.atom{i}.norms,2)
            surf_vector=temp_skel.atom{i}.location'+temp_skel.atom{i}.radius*temp_skel.atom{i}.norms(:,j);
            plot([temp_skel.atom{i}.location(2) surf_vector(2)],[temp_skel.atom{i}.location(1) surf_vector(1)],'c','LineWidth',2)
        end
           plot(temp_skel.atom{i}.location(2),temp_skel.atom{i}.location(1),'.b');

    %end
end