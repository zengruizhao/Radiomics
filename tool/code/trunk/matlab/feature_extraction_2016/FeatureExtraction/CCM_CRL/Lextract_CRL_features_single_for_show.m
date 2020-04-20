
function [curCCM_val,curCCM_description] = Lextract_CRL_features_single_for_show(bounds,I,properties,paraC)
ParaC.CRLM_maxDistance=8;% the maximun run is 8, greater than that we force it to 8
%% buid ccg

% build graph
alpha = paraC.alpha;
r = paraC.r;
[VX,VY,x,y,edges] = construct_ccgs(bounds,alpha, r);

CCGinfo.VX=VX;
CCGinfo.VY=VY;
% add to do new thing with nuclei lulu
nuclei=bounds.nuclei;

% check
nodes=bounds;
show(I,10);hold on;
for x = 1: numel(nodes)
    plot(nodes(x).centroid_c, nodes(x).centroid_r,'yo', 'MarkerSize',2, 'MarkerFaceColor', 'g');
end
plot(VY', VX', 'g-', 'LineWidth', 1);
for k = 1:length(nuclei)
    plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 1);
end
hold off;
%% based on number of neighborhoods rather than number of bounds
for j = 1:length(bounds.centroid_c)-1
    for k = j+1:length(bounds.centroid_c)
        edges(k,j) = edges(j,k);
    end
end

% find gland networks
[numcomp,group] = graphconncomp(sparse(edges));

% remove single gland networks
temp_network = hist(group,numcomp);
[a,group_ind] = find(temp_network > 1);

%% parameters settings
% feature_names = fieldnames(properties);
% feature_names(2,10,13,14,21,25,27)=[];
allCRLM=[]; % save all CRL features
feats=[];
% feature_names';
% lulu dec 28
feature_names={'Area','MajorAxisLength','MinorAxisLength','Eccentricity','Orientation','EquivDiameter','Solidity','Perimeter','Circularity','EllipticalDeviation',...
    'MeanIntensity','IntensityDeviation','IntensityRange','MeanInsideBoundaryIntensity','InsideBoundaryIntensityDeviation','InsideBoundaryIntensityRange',...
    'MeanOutsideBoundaryIntensity','OutsideBoundaryIntensityDeviation','OutsideBoundaryIntensityRange','BoundarySaliency'};
feature_max=[2000, 80, 30,   1,    90, 60,    1, 180,    1, 0.6, 180,  60, 180, 180, 60, 180, 180,60, 180, 90];
feature_min=[ 200, 20,  8, 0.4,   -90, 20, 0.86,  50, 0.85,   0,  40,  10,  50,  50, 10,  40, 50, 10,  40, 20];

interestF=paraC.idx_Mor;
% map_feature=zeros(size(I,1),size(I,2));% the feature map we want

% color map for displaying featuer map
curbin=6;
cmap = colormap(jet(curbin));

for i=interestF:interestF
    cur_f=eval(['[properties.' feature_names{i} ']']);
    %     info.feature_max(i)=max(cur_f);
    %     info.feature_min(i)=min(cur_f);
    
    
    info.feature_max(i)=feature_max(i);
    info.feature_min(i)=feature_min(i);
    info.num_level(i)=6; % quantification level
    info.bin_size(i)=(info.feature_max(i)-info.feature_min(i))/info.num_level(i);% width of bin
    w = info.bin_size(i);
    % let the out of range value to be the boundary value
    cur_f(cur_f>info.feature_max(i))=info.feature_max(i);
    cur_f(cur_f<info.feature_min(i))=info.feature_min(i);
    % discretization
    feature_discret(:,i) = floor((cur_f-info.feature_min(i))/w)+1; %+1 to make them start from 1 instead of 0
    
    % initialize co-occurence
    bin(i,:) = [info.feature_min(i):w:info.feature_max(i)]; % discretizations!!!
    bin(i,:) = [0:info.num_level(i)]+1; % discretizations!!!
    
    idx_c=1;
    set_sizeCCG=[];
    for ii = 1:length(group_ind) % define a neighborhood for each gland network (number of neighborhoods = number of networks)
        % retrive current CCG edge info
        
        curedges=edges(group == group_ind(ii),group == group_ind(ii));
        
        neighborhood_attribute = feature_discret(group == group_ind(ii),i); % aggregate angles in cluster cell network
        
        %         IDS=num2str(neighborhood_attribute);
        %         for id=1:length(IDS)
        %             IDs{id}=IDS(id);
        %         end
        %         h = view(biograph(curedges,IDs));
        
        % the cell run length matrix, each row is a discrete value, each row is a
        % run, e.g., p(2,1) represents the total run number of value 2 in 1 run.
        p = zeros(size(bin(i,:),2), ParaC.CRLM_maxDistance);
        cur_g=unique(neighborhood_attribute);
        
        for  jj=1:length(cur_g)
            cur_gray=cur_g(jj);
            %             cur_gray_run_count=1;
            % how many cur_gray in current CCG
            temp=find(neighborhood_attribute==cur_gray);
            cur_subedges=curedges(temp,temp);
            % test a self made cur_subedges here
            %             cur_subedges=[0 1 0 0 0 0;
            %                 0 0 1 1 0 0;
            %                 0 0 0 0 0 0;
            %                 0 0 0 0 1 1;
            %                 0 0 0 0 0 0;
            %                 0 0 0 0 0 0];
            %
            %              cur_subedges= cur_subedges+ cur_subedges';
            
            dist=graphallshortestpaths(sparse(cur_subedges),'directed',false);
            
            %%% count all 1-run
            dist_0=dist;dist_0(dist==Inf)=0;
            for rr=1:size(dist,1)
                if sum(dist_0(rr,:))==0
                    p(cur_gray,1)=p(cur_gray,1)+1;
                end
            end
            
            %%% get other runs
            %%% find the max run first
            dist_0_triu=triu(dist_0);
            curmaxrun=max(dist_0_triu(:));
            if curmaxrun>size(p,2)-1
                dist_0_triu(dist_0_triu>size(p,2)-1)=size(p,2)-1;
            else
                %%% count all curmaxrun-run
                p(cur_gray,curmaxrun+1)=sum(sum(dist_0_triu==curmaxrun));
                % record the visited nodes, for shorter run compuation, we
                % don't want repeated runs later
                [row,col]=find(dist_0_triu==curmaxrun);
                visitedRunNodes=unique([row; col]);
                %%% for the remaining number runs
                for cur_run_num=curmaxrun-1:-1:1
                    [row,col]=find(dist_0_triu==cur_run_num);
                    cur_num_run=length(row);
                    cur_visitedRunNodes=unique([row; col]);
                    set_visited=intersect(cur_visitedRunNodes,visitedRunNodes);
                    
                    if cur_num_run-length(set_visited)>0
                        p(cur_gray,cur_run_num+1)= cur_num_run-length(set_visited);
                    end
                    visitedRunNodes=unique([visitedRunNodes;cur_visitedRunNodes]);
                end
            end
        end
        
        % check here
        if sum(p<0)>0
            error('bug in RLM, counts < 0');
        end
        
        c(idx_c) = {p./sum(p(:))}; % normalize co-occurence matrix
        
        
        %%
        
        
        
        %%%% show heatmap for a CCG
        %                         M=c{idx_c};
        %                         figure(2);
        %                         colormap('jet');   % set colormap
        %                         imagesc(M,[0 max(M(:))]);        % draw image and scale colormap to values range
        %                         colorbar;          % show color scale
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
        %% set every pixel inside the CCG as the current haralick feature value
        %% find and check the CCG regions
        curCCG_nuiclei=nuclei(group == group_ind(ii));
        curCCG_nuiclei_X=[];
        curCCG_nuiclei_Y=[];
        
        for itemp=1:length(curCCG_nuiclei)
            curCCG_nuiclei_X(itemp)=curCCG_nuiclei{itemp}(1);
            curCCG_nuiclei_Y(itemp)=curCCG_nuiclei{itemp}(length(curCCG_nuiclei{itemp})+1);
        end
        
        bnd_CCG_regions_idx{idx_c} = boundary(curCCG_nuiclei_X',curCCG_nuiclei_Y',0.6);
        K= bnd_CCG_regions_idx{idx_c};
        %                         show(I,3);hold on;
        % %                         for x = 1: numel(nodes)
        % %                             plot(nodes(x).centroid_c, nodes(x).centroid_r,'yo', 'MarkerSize',2, 'MarkerFaceColor', 'g');
        % %                         end
        % %                         plot(VY', VX', 'g-', 'LineWidth', 1);
        % %
        %                         for k = 1:length(nuclei)
        %                             plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 1);
        %                         end
        %                         plot(curCCG_nuiclei_Y(K),curCCG_nuiclei_X(K),'b-','LineWidth', 3);
        %                         hold off;
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %% get current haralick feature value for current CCG region, should change the idx_fea for different feature
        % haralick_feature_names={'contrast-energy','contrast-inverse-moment','contrast-ave','contrast-var',...
        %     'contrast-ent','intensity-ave','intensity-var','intensity-ent','entropy','energy',...
        %     'correlation','info-measure1','info-measure2'};
        
        idx_fea=paraC.idx_Har;%should change the idx_fea for different feature
        curharalick= haralick_no_img(c{idx_c});%curharalick= haralick_no_img(zeros(7,7));
        feats{idx_c} =curharalick;
        haralick_vals(idx_c)=curharalick.val(idx_fea);
        %% assign feature value to current CCG
        curCCG_nuiclei_Ys{idx_c}=curCCG_nuiclei_Y(K);
        curCCG_nuiclei_Xs{idx_c}=curCCG_nuiclei_X(K);
        curCCGregion=poly2mask(curCCG_nuiclei_Y(K),curCCG_nuiclei_X(K),size(I,1),size(I,2));%show(curCCGregion);
        set_sizeCCG(idx_c)=sum(curCCGregion(:));
        %%% enlarge the region for better visual
        curCCGregion=imdilate(curCCGregion,strel('disk',50));%show(curCCGregion);
        ss=regionprops(curCCGregion,'ConvexHull');
        temp=[ss.ConvexHull];
        %             fprintf('%d\n',ii);
        %             if ii==797;
        %                 disp('');
        %             end
        if isempty(temp)
            curCCG_nuclei_Ys_large{idx_c}=curCCG_nuiclei_Ys{idx_c};
            curCCG_nuclei_Xs_large{idx_c}=curCCG_nuiclei_Xs{idx_c};
        else
            curCCG_nuclei_Ys_large{idx_c}=temp(:,2);
            curCCG_nuclei_Xs_large{idx_c}=temp(:,1);
        end
        %             map_feature(curCCGregion)=haralick_vals(idx_c);%
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        idx_c=idx_c+1;
    end
end
%% draw color ccg regions onto original image
show(I,4);hold on;
if ~paraC.flag_useotherfeaturelimits
    paraC.feature_limits=[min(haralick_vals) max(haralick_vals)];
end
feature_range=paraC.feature_limits(2)-paraC.feature_limits(1);
[v,idx_sizeCCG]=sort(set_sizeCCG,'descend');
for ish=1:idx_c-1
    cur_g=floor(((haralick_vals(idx_sizeCCG(ish))-paraC.feature_limits(1))/feature_range)*(curbin-1));
    %         fill(curCCG_nuiclei_Ys{ish},curCCG_nuiclei_Xs{ish}, cmap(cur_g+1,:));
    fill(curCCG_nuclei_Xs_large{idx_sizeCCG(ish)},curCCG_nuclei_Ys_large{idx_sizeCCG(ish)}, cmap(cur_g+1,:));
end
hold off; colorbar;
%% feature map as heatmap
%     show(map_feature,5);    cmap = colormap(jet(curbin)); colorbar;

%% statistics: mean, median, standard deviation, range, kurtosis, skewness , across bounds for each haralick feature
all_feat=haralick_vals;   figure(11);hist(all_feat);
curCCM=[mean(all_feat) median(all_feat) std(all_feat)  range(all_feat) kurtosis(all_feat) skewness(all_feat)];
curCCM_val=curCCM(paraC.idx_Sta);
%% name
modifier = [{'mean'} {'median'} {'std'} {'range'} {'kurtosis'} {'skewness'} ];
%the format is like this: morphofeaturenames_haralickfeatuename_statistcname
haralick_feature_names={'contrast-energy','contrast-inverse-moment','contrast-ave','contrast-var',...
    'contrast-ent','intensity-ave','intensity-var','intensity-ent','entropy','energy',...
    'correlation','info-measure1','info-measure2'};

curCCM_description=['CCM-' feature_names{paraC.idx_Mor} '-' modifier{paraC.idx_Sta} '(' haralick_feature_names{paraC.idx_Har} ')'];
fprintf('%s, value is %.3f\n',curCCM_description,curCCM_val);
end

end