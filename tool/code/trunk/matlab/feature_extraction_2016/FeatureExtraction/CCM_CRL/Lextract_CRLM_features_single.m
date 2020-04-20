% this one is the correct one, do remember the reinitialization in
% multi-loop senario!!!
function [allCRL,CRL_description,CCGinfo] =  Lextract_CRLM_features_single(bounds,I,properties,a,r,ParaC)
ParaC.CRLM_maxDistance=4;% the maximun run is 8 by default, greater than that we force it to 8
ParaC.num_level_morphlogy=4;% quantify the morphology into 6 levels

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
allCRL=[]; % save all CRL features
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
    info.num_level(i)=ParaC.num_level_morphlogy; % quantification level
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
        % retrive current CCG edge info
        curedges=edges(group == group_ind(ii),group == group_ind(ii));
        neighborhood_attribute = feature_discret(group == group_ind(ii),i); % aggregate angles in cluster cell network
        
        %         IDS=num2str(neighborhood_attribute);
        %         for id=1:length(IDS)
        %             IDs{id}=IDS(id);
        %         end
        %         h = view(biograph(curedges,IDs));
        
        % the cell run length matrix, each row is a discrete value, each column is a
        % run, e.g., p(2,1) represents the total run number of value 2 in 1 run.
        p = zeros(size(bin(i,:),2), ParaC.CRLM_maxDistance);
        cur_g=unique(neighborhood_attribute);
        %         if length(cur_g)>2
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
            error('bug in CRLM, counts < 0');
        end
        
        c(idx_c) = {p}; % save all cell run length matrix
        ss=run_length_matrix_feature(p);
        feats{idx_c} = ss;
        idx_c=idx_c+1;
    end
    %% statistics: mean, median, standard deviation, range, kurtosis, skewness , across bounds for each haralick feature
    % check if the current nuclear morpholgy has no haralick features
    if ~isempty(feats) && length(feats)>3
        all_feat=[];% aggregating all haralick features for current nuclear morpholgy for all CCG in an image
        for fi=1:length(feats)
            cur_feat=feats{fi};
            all_feat=[all_feat;cur_feat];
        end
        
        curCRL=[];
        curCRL=[mean(all_feat) median(all_feat) std(all_feat)  range(all_feat) kurtosis(all_feat) skewness(all_feat)];
        %         idx_CRL=1;
        %     CRL(idx_CRL,:)=mean(all_feat);idx_CRL=idx_CRL+1;
        %     CRL(idx_CRL,:)=median(all_feat);idx_CRL=idx_CRL+1;
        %     CRL(idx_CRL,:)=std(all_feat);idx_CRL=idx_CRL+1;
        %     CRL(idx_CRL,:)=range(all_feat);idx_CRL=idx_CRL+1;
        %     CRL(idx_CRL,:)=kurtosis(all_feat);idx_CRL=idx_CRL+1;
        %     CRL(idx_CRL,:)=skewness(all_feat);idx_CRL=idx_CRL+1;
        allCRL=[allCRL curCRL];
    else
        allCRL=[allCRL zeros(1,66)];
    end
end

%% feature names organization
count = 1;
modifier = [{'mean'} {'median'} {'std'} {'range'} {'kurtosis'} {'skewness'} ];
%the format is like this: morphofeaturenames_haralickfeatuename_statistcname
run_length_feature_names={'Short Run Emphasis(SRE)', 'Long Run Emphasis (LRE)','Morph-Level Nonuniformity (MLN)',...
    'Run Length Nonuniformity (RLN)','Run Percentage (RP)','Low Morph-Level Run Emphasis (LMRE)',...
    'High Morph-Level Run Emphasis (HMRE)','Short Run Low Morph-Level Emphasis (SRLME)',...
    'Short Run High Morph-Level Emphasis (SRHME)','Long Run Low Morph-Level Emphasis (LRLME)',...
    'Long Run High Morph-Level Emphasis (LRHME)'};
for m=1:length(feature_names);
    for mi = 1:numel(modifier)
        for j = 1:numel(run_length_feature_names)
            CRL_description{count} = ['CRLM-' feature_names{m} ':' modifier{mi} '(' run_length_feature_names{j} ')'  ];
            count = count + 1;
        end
    end
end
end