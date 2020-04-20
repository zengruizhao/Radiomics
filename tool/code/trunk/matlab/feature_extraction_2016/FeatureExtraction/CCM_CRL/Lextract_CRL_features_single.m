% not CRL and CRLM is different
function [allCRL,CRL_description,CCGinfo] =  Lextract_CRL_features_single(bounds,I,properties,a,r,ParaC)
ParaC.CRLM_maxDistance=4;% the maximun run is 8 by default, greater than that we force it to 8
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
for j = 1:length([bounds.centroid_c])-1
    for k = j+1:length([bounds.centroid_c])
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


idx_c=1;
c=cell(1);
feats=cell(1);
for ii = 1:length(group_ind) % define a neighborhood for each gland network (number of neighborhoods = number of networks)
    % retrive current CCG edge info
    cur_subedges=edges(group == group_ind(ii),group == group_ind(ii));
    N_cell=size(cur_subedges,1);
    % the cell run length vector, each column is a
    % run, e.g., p(2) represents the total run number in 2-run.
    %         p = zeros(1, ParaC.CRLM_maxDistance);
    p = [];
    
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
    
    dist_0=dist;dist_0(dist==Inf)=0;
    %         for rr=1:size(dist,1)
    %             if sum(dist_0(rr,:))==0
    %                 p(cur_gray,1)=p(cur_gray,1)+1;
    %             end
    %         end
    
    %%% get other runs
    %%% find the max run first
    dist_0_triu=triu(dist_0);
    curmaxrun=max(dist_0_triu(:));
    %%% count all curmaxrun-run
    p(curmaxrun+1)=sum(sum(dist_0_triu==curmaxrun));
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
            p(cur_run_num+1)= cur_num_run-length(set_visited);
        end
        visitedRunNodes=unique([visitedRunNodes;cur_visitedRunNodes]);
    end
    
    % check here
    if sum(p<0)>0
        error('bug in CRL, counts < 0');
    end
    
    c(idx_c) = {p}; % save all cell run length matrix
    ss=run_length_vector_feature(p,N_cell);
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
    allCRL=[allCRL zeros(1,24)];
end


%% feature names organization
count = 1;
modifier = [{'mean'} {'median'} {'std'} {'range'} {'kurtosis'} {'skewness'} ];
%the format is like this: morphofeaturenames_haralickfeatuename_statistcname
run_length_feature_names={'Short Run Emphasis', 'Long Run Emphasis',...
    'Run Length Nonuniformity','Run Percentage'};

for mi = 1:numel(modifier)
    for j = 1:numel(run_length_feature_names)
        CRL_description{count} = ['CRL-' modifier{mi} '(' run_length_feature_names{j} ')'  ];
        count = count + 1;
    end
end

end