% by cheng lu, on 3rd Dec. 2015
% calculate all subgraphs hosoya index. 
% note we use undirected graph for the hosoya index.
% the ccg visualization parameter are stroed in CCGinfo.
function [hosoya_all,CCGinfo] = Lextract_Hosoya_features(bounds,a,r)
% compute the mean and standard deviation of CGTs across all bounds
% c = co-occurrence matrix
if nargin < 2
    info.alpha = 0.5;
else
    info.alpha = a;
end
if nargin < 3
    info.radius = 0.2;
else
    info.radius = r;
end

%% build ccg
% build graph
alpha = info.alpha;
r = info.radius;
[VX,VY,x,y,edges] = construct_ccgs(bounds,alpha, r);

CCGinfo.VX=VX;
CCGinfo.VY=VY;
% CCGinfo.x=x;
% CCGinfo.y=y;
% CCGinfo.edges=edges;
% [VX,VY,x,y,edges] =Lconstruct_ccgs_optimized(bounds,alpha, r);

% nodes=bounds;
% figure;hold on;
% for x = 1: numel(nodes)
%     plot(nodes(x).centroid_r, nodes(x).centroid_c,'bo', 'MarkerSize',2, 'MarkerFaceColor', 'g');
% end
% plot(VX', VY', 'r-', 'LineWidth', 1);
% hold off;


%% based on number of neighborhoods rather than number of bounds
for j = 1:length([bounds.centroid_c])-1
    for k = j+1:length([bounds.centroid_c])
        edges(k,j) = edges(j,k);
    end
end

% find gland networks
[numcomp,group] = graphconncomp(sparse(edges),'Directed','false');

% % test graph
% UG=[0 1 0 1;1 0 0 1;0 0 0 0;1 1 0 0];
% h=view(biograph(UG));
% 
% [S,C]=graphconncomp(sparse(UG),'Directed','false');
% 
% curGraphIdx=find((C == 1));
% curGraph=UG(curGraphIdx,curGraphIdx);
% [x,y]=find(curGraph);
% hosoya(i) = hosoya_c([x,y]);

% [S,C]=graphconncomp(sparse(curGraph),'Directed','false');
% 
% h=view(biograph(curGraph));
% colors = jet(S);
% for i = 1:numel(h.nodes)
%   h.Nodes(i).Color = colors(C(i),:);
% end

% % test hosoya index function
% UG=[0 1 1 1;1 0 1 1;1 1 0 1;1 1 1 0];
% h=view(biograph(UG));
% [x,y]=find(UG);
% hosoya = hosoya_c([x,y]);
% 
% UGu=triu(UG);
% [x,y]=find(UGu);
% hosoya = hosoya_c([x,y]);


% remove single networks
temp_network = hist(group,numcomp);
[a,group_ind] = find(temp_network > 1); 

[origgroup_ind,newgroup_ind] = sort(group_ind,'ascend');
% fprintf('find %d subgraphs for hosoya calculation at a=%.2f\n',length(newgroup_ind),a);

for i=1:length(origgroup_ind)
    curGraphIdx=find((group == origgroup_ind(i)));
    % for the subgraph has large number of nodes, we put the value of Inf
    if length(curGraphIdx)<30 
        curGraph=edges(curGraphIdx,curGraphIdx);
        curGraph=triu(curGraph);
        [x,y]=find(curGraph);
        hosoya_all(i) = hosoya_c([x,y]);
    else
        hosoya_all(i)=Inf;
    end
end
