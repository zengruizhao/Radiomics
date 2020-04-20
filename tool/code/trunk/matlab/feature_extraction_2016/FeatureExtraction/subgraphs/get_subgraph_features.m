function [feats,feature_list] = get_subgraph_features(bounds, edges)
% MATLAB Implementation of Sahir's ccg features
% updated - faster implementation of connected components
% George Lee (2013)

%1) Number of Nodes
N = length(edges);
feats(1) = N;
feature_list(1) = {'Number of Nodes'};


%2) Number of edges
E = sum(edges(:));
feats(2) = E;
feature_list(2) = {'Number of Edges'};


%3) Average Degree
feats(3) = E/N;
feature_list(3) = {'Average Degree'};


%4) Average Eccentricity
X = [bounds(:).centroid_r; bounds(:).centroid_c];
D = pdist(X','euclidean');
D = squareform(D);
D = D + diag(ones(1,length(D)));

sym_edges = edges;
for i = 1:length(edges)-1
    for j = i+1:length(edges)
        sym_edges(j,i) = edges(i,j);
    end
end

% weighted edges
weighted = sym_edges.*D;

% pre-calculate shortest paths (8/24/2013)
for i = numel(bounds)
    distance = graphshortestpath(sparse(weighted),i);
    temp = distance(~isinf(distance));
    pathlengths{i} = temp(temp ~= 0);
end

count = 1;
for i = 1:length(edges)
    %neighborhood = neighborhood_noinf(distance > 0);
    if ~isempty(pathlengths{i})
        eccentricity(count) = max(pathlengths{i});
        count = count + 1;
    end
end

feats(4) = sum(eccentricity)/N;
feature_list(4) = {'Average Eccentricity'};

%5) Diameter
diameter = max(eccentricity);
feats(5) = diameter;
feature_list(5) = {'Diameter'};

%6) Radius
radius = min(eccentricity);
feats(6) = radius;
feature_list(6) = {'Radius'};


% not sure if these 90% are supposed to represent the closest, further, or
% random 90%
%7) Average Eccentricity 90%
count = 1;
for i = 1:length(bounds)
    %sorted_pathlengths = sort(pathlengths{i},'descend');
    sorted_pathlengths = pathlengths{i};
    sorted_pathlengths90{i} = sorted_pathlengths(1:round(0.9*length(sorted_pathlengths))); %truncate
    %neighborhood = neighborhood_noinf(distance > 0);
    if ~isempty(pathlengths{i})
        eccentricity90(count) = max(sorted_pathlengths90{i});
        count = count + 1;
    end
end

feats(7) = sum(eccentricity90)/N;
feature_list(7) = {'Average Eccentricity 90\%'};


%8) Diameter 90%
diameter90 = max(eccentricity90);
feats(8) = diameter90;
feature_list(8) = {'Diameter 90\%'};


%9) Radius 90%
radius90 = min(eccentricity90);
feats(9) = radius90;
feature_list(9) = {'Radius 90\%'};

%10) Average Path Length
feats(10) = sum( [pathlengths{:}] ) / length([pathlengths{:}]);
feature_list(10) = {'Average Path Length'};


%% clustering coefficients

% find connected components
% graph = edges;
% for j = 1:length(bounds)-1
%     for k = j+1:length(bounds)
%         graph(k,j) = graph(j,k);
%     end
% end

[numcomp,network] = graphconncomp(sparse(sym_edges));
[upper] = triu(edges);

for n = 1:length(network)
    [neighborhood_size, nodes] = find(network == n);
    %local_index = find(network == network(n));
    % search through edges

  count = countnodesandedges(nodes,upper); % mex file
% 
%         count = 0;
%     for i = [nodes]
%         for j = [nodes]
%             %fprintf('%i\n',sub2ind(size(upper),i,j));
%             if upper(i,j) == 1,
%                 count = count +1;
%             end
%         end
%     end
    
    En(nodes) = count;% number of edges between nodes in a neighborhood
    kn(nodes) = numel(nodes); %number of nodes in the neighborhood of node n
end

%11) Clustering Coefficient C
% ratio beteween A: the number of edges between neighbors of node n and B:
% the number of possible edges between the neighbors of node n

%Cn(n) = 2*En(n) / ( kn(n) * (kn(n)-1) );
Cn = 2*En ./ ( kn .* (kn-1) );
Cn( isnan(Cn) ) = 0; % account for divide by zero
feats(11) = sum(Cn)/N;
feature_list(11) = {'Clustering Coefficient C'};


%12) Clustering Coefficient D
%Dn(n) = 2*(kn(n) + En(n)) / ( kn(n)* (kn(n)+1) );
Dn = 2*(kn + En) ./ ( kn .* (kn+1) );
Dn( isnan(Dn) ) = 0;
feats(12) = sum(Dn)/N;
feature_list(12) = {'Clustering Coefficient D'};


%13) Clustering Coefficient E
% count isolated nodes
iso_nodes = sum(kn == 1);
feats(13) = sum( Cn(kn > 1) ) / (N - iso_nodes);
feature_list(13) = {'Clustering Coefficient E'};

%14) Number of connected components
feats(14) = length(kn(kn>1));
feature_list(14) = {'Number of connected components'};


%15) Giant connected component ratio
feats(15) = max(kn) / N;
feature_list(15) = {'giant connected component ratio'};

%16) Average Connected Component Size
feats(16) = mean(kn(kn>1));
feature_list(16) = {'average connected component size'};


%17 and 18) Number / Percentage of Isolated Nodes
feats(17) = iso_nodes;
feature_list(17) = {'number isolated nodes'};

feats(18) = iso_nodes/N;
feature_list(18) = {'percentage isolated nodes'};


%19 and 20) Number / Percentage of End points
feats(19) = sum(kn==2);
feature_list(19) = {'number end nodes'};
feats(20) = sum(kn==2)/N;
feature_list(20) = {'percentage end nodes'};


% 21 and 22) Number / Percentage of Central points
feats(21) = sum(eccentricity == radius);
feature_list(21) = {'number central nodes'};
feats(22) = sum(eccentricity == radius) / N;
feature_list(22) = {'percentage central nodes'};

% 23 - 26) Edge length statistics
edge_lengths = weighted(:);
edge_lengths(edge_lengths==0) = []; % remove zero edge lengths

feats(23) = sum(edge_lengths)/length(edge_lengths); % mean edge-length
feature_list(23) = {'mean edge length'};
feats(24) = std(edge_lengths); % standard deviation
feature_list(24) = {'standard deviation edge length'};
feats(25) = skewness(edge_lengths); % skewness
feature_list(25) = {'skewness edge length'};
feats(26) = kurtosis(edge_lengths); % kurtosis
feature_list(26) = {'kurtosis edge length'};
