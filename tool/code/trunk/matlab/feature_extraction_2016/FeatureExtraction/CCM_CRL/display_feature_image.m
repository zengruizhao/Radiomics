function display = display_feature_image(img,featurename,varargin)

%img = imrotate(img,90);

featurename = lower(featurename); %lower

bounds = varargin{1};
% if numel(varargin) >=2
% bounds = varargin{2};
% end
% if numel(varargin) >=3
%imgscale = varargin{2};
imgscale = 1;
% end

% if ~exist('bounds','var') || ~isempty(varargin{1})
%     if isfield(params,'x')
%     for i = 1:numel(params.x)
%         bounds(i).centroid_c = params.x(i);
%         bounds(i).centroid_r = params.y(i);
%         
% %         bounds(i).centroid_c = params.y(i);
% %         bounds(i).centroid_r = params.x(i);
%     end
%     end
% end


%  if strcmp(featurename,'clustergraph')
%       for i = 1:numel(params.cluster_bounds)
%         bounds(i).centroid_r = params.cluster_bounds.r;
%         bounds(i).centroid_c = params.cluster_bounds.c;
%         bounds(i).r = params.cluster_bounds.r;
%         bounds(i).c = params.cluster_bounds.c;
%       end
% end

%display_feature_image(img,featurename,display_params)
if ~exist('scale','var') %default
    scale = 1;
end

display = featurename;

 %{'directionalitycolormap'} {'disorder'}
if ~ismember(featurename,{'morph'})
    for i = 1:numel(bounds)
        cen_c=bounds(i).centroid_r;
        cen_r=bounds(i).centroid_c;
        c=bounds(i).r;
        r=bounds(i).c;
        
        
        bounds(i).centroid_r = cen_r;
        bounds(i).centroid_c = cen_c;
        bounds(i).r = r;
        bounds(i).c = c;
    end
end

switch featurename
    case 'morph'
            rescalehistology(img,imgscale)
            hold on
            for i = 1:numel(bounds)
                plot([bounds(i).c(:); bounds(i).c(1)],[bounds(i).r(:); bounds(i).r(1)],'g','LineWidth',2)
            end

    
    case {'directionalitycolormap', 'disorder'}
            %% directionality colormap
            rescalehistology(img,imgscale)
            hold on
            directionality_colormap(bounds,scale);
            
    case {'clustergraph', 'subgraph'}
        %% subgraph
            %featurename = 'subgraph'
            rescalehistology(img,imgscale)
            hold on
            cluster_graph_map(bounds,2,2,scale);
    case 'core'
            %featurename = 'core'
            [CGTfeats, c_matrix, info, feats, network, edges] = get_CGT_features_networks_weight(bounds);
            %for attribute = 1:13
            rescalehistology(img,imgscale)
            hold on
            axis ij
            attribute = 1;
            %scale = 0.5;
            F = cluster_graph_network_map(bounds, network,feats,attribute,scale,'capture');
            
            rescalehistology(img,imgscale)
            
    case 'voronoi'
        % featurename = 'voronoi'
            rescalehistology(img,imgscale)
            hold on
            x = [bounds.centroid_c];
            y = [bounds.centroid_r];
            
            %plot(y,x,'rs','linewidth',2); hold on %plot centroids
            plot(y,x,'rs','linewidth',2); hold on %plot centroids
            [VY,VX] = voronoi(x,y);
            %plot(VY,VX,'g-','linewidth',2)
            plot(VX,VY,'g-','linewidth',2)
        
    case 'delaunay'
        %featurename = 'delaunay'
            rescalehistology(img,imgscale)
            hold on
            x = [bounds.centroid_c].*scale;
            y = [bounds.centroid_r].*scale;

            del = delaunay(x,y);

            plot(y,x,'rs','linewidth',2); hold on %plot centroids
            
            triplot(del,y,x,'g','LineWidth',1.5);
            
    case {'texture','haralick'}
        
           %[feats, description, param] = extract_texture_feats(img, params.mask);
           
           mask = varargin{3};
           if ~exist('MPP','var')
               MPP = 0.5; % default assumes 20x magnification
           end
           
           factor = 0.5/MPP;
           
           info.dist = round(1 * factor);
           info.win = round(1 * factor);
           if info.dist < 1, info.dist = 1; end
           if info.win < 1, info.win = 1; end
           
           info.grays = 256;
           
           n = 0;
           
           if ndims(img) < 3
               gimg = img;
           elseif ndims(img) == 3
               gimg = rgb2gray(img); % assume img is rgb
           else
               fprintf('Unidentified image format')
           end
           
           if ~exist('mask','var')
               mask(:,:,1) = (gimg ~= max(max(gimg)));
           else
               img_mask = mask;
               %mask = imdilate(mask, strel('disk', ceil(2 * factor) )); %doesn't affect feature
               mask = imdilate(mask, strel('disk', info.win )); %doesn't affect feature
               mask = logical(mask);
           end
           
           grays = info.grays;
           win = info.win;
           dist = info.dist;
           
           himg = uint16(rescale_range(gimg,0,grays-1));
               f = haralick_img(himg,mask,grays,win,dist,1);
           Hf = f.img3;
           
           for i = 1:size(Hf,3)
               feat = Hf(:,:,i);
               if ~exist('img_mask','var')
                   img_mask = mask(:,:,1);
               end
               roi = feat(img_mask ==1);
               
               display.texture_img{i} = feat.*img_mask;
               
               Texturefeats(n+1) = mean(roi(:));
               Texturefeats(n+2) = std(roi(:));
               %Texturefeats(n+3) = mode(roi(:));
               n = n + 2;
           end

           
    otherwise
        fprintf('\nfeature not recognized for plotting')    
    display = [];
end
