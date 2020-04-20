% this one is the correct one, do remember the reinitialization in
% multi-loop senario!!!
function [allCCM,CCM_description,CCGinfo] = Lextract_CCM_features_single_correct(bounds,I,properties,a,r)
%% buid ccg

% build graph
alpha = a;
% r = r;
[VX,VY,x,y,edges] = construct_ccgs(bounds,alpha, r);

CCGinfo.VX=VX;
CCGinfo.VY=VY;

% check
% nodes=bounds;
% show(I,1);hold on;
% for x = 1: numel(nodes)
%     plot(nodes(x).centroid_c, nodes(x).centroid_r,'yo', 'MarkerSize',2, 'MarkerFaceColor', 'g');
% end
% plot(VY', VX', 'g-', 'LineWidth', 1);
% hold off;

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
allCCM=[]; % save all CCM features
feats=[];
% feature_names';
% lulu dec 28
feature_names={'Area','MajorAxisLength','MinorAxisLength','Eccentricity','Orientation','EquivDiameter','Solidity','Perimeter','Circularity','EllipticalDeviation',...
    'MeanIntensity','IntensityDeviation','IntensityRange','MeanInsideBoundaryIntensity','InsideBoundaryIntensityDeviation','InsideBoundaryIntensityRange',...
    'MeanOutsideBoundaryIntensity','OutsideBoundaryIntensityDeviation','OutsideBoundaryIntensityRange','BoundarySaliency'};
feature_max=[2000, 80, 30,   1,    90, 60,    1, 180,    1, 0.6, 180,  60, 180, 180, 60, 180, 180,60, 180, 90];
feature_min=[ 200, 20,  8, 0.4,   -90, 20, 0.86,  50, 0.85,   0,  40,  10,  50,  50, 10,  40, 50, 10,  40, 20];

for i=1:length(feature_names)
    cur_f=eval(['[properties.' feature_names{i} ']']);
    %     info.feature_max(i)=max(cur_f);
    %     info.feature_min(i)=min(cur_f);
    
    info.feature_max(i)=feature_max(i);
    info.feature_min(i)=feature_min(i);
    info.num_level(i)=10; % quantification level
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
    c=cell(1);
    feats=cell(1);
    for ii = 1:length(group_ind) % define a neighborhood for each gland network (number of neighborhoods = number of networks)
        p = zeros(size(bin(i,:),2), size(bin(i,:),2));
        
        neighborhood_angles = feature_discret(group == group_ind(ii),i); % aggregate angles in cluster cell network
        %w = 5 % size of discretization
        % p = hist(angles,180/w) %
        
        %discrete_angles(i)
        
        % aggregate co-occurrences, the haralick features are stored in
        % feats
        
        cur_g=unique(neighborhood_angles);
        if length(cur_g)>2
            C= nchoosek(cur_g,2);
            for jj=1:size(C,1)
                p(C(jj,1),C(jj,2))=sum(neighborhood_angles==C(jj,2))+sum(neighborhood_angles==C(jj,1));
                %              p(C(jj,2),C(jj,1))=sum(neighborhood_angles==C(jj,1));
            end
            %
            %         for jj=1:length(cur_g)
            %             for kk=jj:length(cur_g)
            %                     p(jj,kk) = p(jj,kk) + 1; % add instances to co-occurrence
            %             end
            %         end
            %
            %
            %         for jj = 1:length(bin) % find out how often each pair of bins co-occur
            %             for kk = jj:length(bin)
            %                 if sum(ismember(neighborhood_angles, bin(jj))) && sum(ismember(neighborhood_angles, bin(kk)))
            %                     p(jj,kk) = p(jj,kk) + 1; % add instances to co-occurrence
            %                 end
            %                 n = n+1;
            %             end
            %         end
            
            c(idx_c) = {p./sum(p(:))}; % normalize co-occurence matrix
            feats{idx_c} = haralick_no_img(c{idx_c});
            idx_c=idx_c+1;
        end
    end
    
    
    % remove networks that don't give co-occurrence features
    %         for k = 1:length(feats{1}.names)
    %             n = 0;
    %             for iii = 1:length(feats)
    %                 if isfield(feats{iii},'val')
    %                 else
    %                     group_ind(group_ind == iii) = [];
    %                     continue
    %                 end
    %             end
    %         end
    %         [origgroup_ind,newgroup_ind] = sort(group_ind,'ascend');
    %
    %         % define network and apply new network indices following network prunes
    %         network = [];
    %         num_connected_comp = max(newgroup_ind);
    %
    %         for j = 1:num_connected_comp
    %             network(group == origgroup_ind(j)) = newgroup_ind(j);
    %         end
    %
    %         % replicate feats based on the size of network
    %         %netfeats = feats;
    %         for j = 1:max(group)
    %             networksize(j) = sum(group == j);
    %         end
    %
    %% statistics: mean, median, standard deviation, range, kurtosis, skewness , across bounds for each haralick feature
    % check if the current nuclear morpholgy has no haralick features
    if ~isempty(feats) && length(feats)>3
        all_feat=[];% aggregating all haralick features for current nuclear morpholgy for all CCG in an image
        for fi=1:length(feats)
            cur_feat=feats{fi}.val;
            all_feat=[all_feat;cur_feat];
        end
        
        curCCM=[];
        curCCM=[mean(all_feat) median(all_feat) std(all_feat)  range(all_feat) kurtosis(all_feat) skewness(all_feat)];
        %         idx_ccm=1;
        %     CCM(idx_ccm,:)=mean(all_feat);idx_ccm=idx_ccm+1;
        %     CCM(idx_ccm,:)=median(all_feat);idx_ccm=idx_ccm+1;
        %     CCM(idx_ccm,:)=std(all_feat);idx_ccm=idx_ccm+1;
        %     CCM(idx_ccm,:)=range(all_feat);idx_ccm=idx_ccm+1;
        %     CCM(idx_ccm,:)=kurtosis(all_feat);idx_ccm=idx_ccm+1;
        %     CCM(idx_ccm,:)=skewness(all_feat);idx_ccm=idx_ccm+1;
        allCCM=[allCCM curCCM];
    else
        allCCM=[allCCM zeros(1,78)];
    end
end

%% feature names organization
count = 1;
modifier = [{'mean'} {'median'} {'std'} {'range'} {'kurtosis'} {'skewness'} ];
%the format is like this: morphofeaturenames_haralickfeatuename_statistcname
haralick_feature_names={'contrast-energy','contrast-inverse-moment','contrast-ave','contrast-var',...
    'contrast-ent','intensity-ave','intensity-var','intensity-ent','entropy','energy',...
    'correlation','info-measure1','info-measure2'};
for m=1:length(feature_names);
    for mi = 1:numel(modifier)
        for j = 1:numel(haralick_feature_names)
            CCM_description{count} = ['CCM-' feature_names{m} ':' modifier{mi} '(' haralick_feature_names{j} ')'  ];
            count = count + 1;
        end
    end
end
end