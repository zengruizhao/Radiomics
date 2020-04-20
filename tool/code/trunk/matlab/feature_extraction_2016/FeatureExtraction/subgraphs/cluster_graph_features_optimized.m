function [feats,feature_list,params] = cluster_graph_features(bounds, varargin)
% MATLAB Implementation of Sahir's ccg features
% Usage:
% cluster_graph_features_networkboost(bounds)
% cluster_graph_features_networkboost(bounds, edges)
% cluster_graph_features_networkboost(bounds, alpha, r)
%
% updated (George, 2015) - handling of all isolated node cases
% updated (Ajay, 2014) - additional input options allow CCG to be constructed on-the-fly
% updated (Ajay, 2014) - reduced complexity from O(n^2) to 0(n)
% updated - faster implementation of connected components
% George Lee (2013)

% Allows for 3 different input argument structures
params = [];
if nargin == 1
    [~, ~, ~, ~, edges, params] = construct_ccgs(bounds);
elseif nargin == 2
    edges = varargin{1};
elseif nargin == 3
    [~, ~, ~, ~, edges, params] = construct_ccgs(bounds, varargin{1}, varargin{2});
else
    error('Input args must be: (bounds), (bounds, edges), or (bounds, alpha, r)');
end

if length(bounds) ==1
    fprintf('\nNot enough bounds to compute Cluster Graphs!')
    N = 1;
    
    pathlengths{1} = 0;
    pathlengths_nonempty{1} = 0;
    eccentricity = 0;
    
%     feats(1) = 1;     feature_list(1) = {'Number of Nodes'};
%     feats(2) = 0;    feature_list(2) = {'Number of Edges'};
%     feats(3) = 0;    feature_list(3) = {'Average Degree'};
%     
else
    %1) Number of Nodes
    %N = length(edges);
    N = length(bounds);
    feats(1) = N;
    feature_list(1) = {'Number of Nodes'};
    
    
    %%% Eccentricity calculation
    % generate distance matrix
    X = [bounds(:).centroid_r; bounds(:).centroid_c];
    D = squareform(pdist(X','euclidean')) + eye(length(X));
    
    % create sparse distance-weighted edge matrix
    edges = triu(edges); % force edges to be upper triangular
    sym_edges = sparse(edges | edges'); % full symmetric matrix
    weighted = sym_edges .* D;
    
    
    
    % pre-calculate shortest paths (8/24/2013)
    for i = 1:N
        distance = graphshortestpath(weighted,i);
        pathlengths{i} = nonzeros(distance(isfinite(distance))).';
    end
    pathlengths_nonempty = pathlengths;
    pathlengths_nonempty(cellfun(@isempty, pathlengths_nonempty, 'UniformOutput', true)) = [];
    
    % all non-zero pathlengths
    eccentricity = cellfun(@max, pathlengths_nonempty, 'UniformOutput', true);
end

    %1) Number of Nodes
    %N = length(edges);
    N = length(bounds);
    feats(1) = N;
    feature_list(1) = {'Number of Nodes'};
    
    
    %2) Number of edges
    E = nnz(edges);
    feats(2) = E;
    feature_list(2) = {'Number of Edges'};
    
    
    %3) Average Degree
    feats(3) = E/N;
    feature_list(3) = {'Average Degree'};


    %4) Average eccentricity
    feats(4) = sum(eccentricity)/N;
    feature_list(4) = {'Average Eccentricity'};
    

   
if length(pathlengths{1}) < 1, if isempty([pathlengths{:}]), for i = 1:N, pathlengths{i} = 0; fprintf('\nWarning: No edges found! All isolated nodes. Consider not using these features.'); end ; end; end
if isempty(eccentricity), eccentricity = 0; end

%5) Diameter
diameter = max(eccentricity);
feats(5) = diameter;
feature_list(5) = {'Diameter'};

%6) Radius
radius = min(eccentricity);
feats(6) = radius;
feature_list(6) = {'Radius'};


% eccentricity for largest 90% of path lengths

%%% CHOOSE ONE OF THE FOLLOWING %%%
%%%%% George's definition of 90% %%%%%
pathlengths_nonempty90 = cellfun(@(x) x(1:round(0.9*length(x))), pathlengths_nonempty, 'UniformOutput', false);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Ajay's definition of 90% %%%%%
% pathlengths_nonempty90 = cellfun(@(x) x(x >= prctile(x,10)), pathlengths_nonempty, 'UniformOutput', false);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pathlengths_nonempty90(cellfun(@isempty, pathlengths_nonempty90, 'UniformOutput', true)) = [];
eccentricity90 = cellfun(@max, pathlengths_nonempty90, 'UniformOutput', true);

%7) Average Eccentricity 90%
feats(7) = sum(eccentricity90)/N;
feature_list(7) = {'Average Eccentricity 90\%'};

if isempty(eccentricity90), eccentricity90 = 0; end

%8) Diameter 90%
diameter90 = max(eccentricity90);
feats(8) = diameter90;
feature_list(8) = {'Diameter 90\%'};


%9) Radius 90%
radius90 = min(eccentricity90);
feats(9) = radius90;
feature_list(9) = {'Radius 90\%'};

%10) Average Path Length
feats(10) = sum(cellfun(@sum, pathlengths, 'UniformOutput', true)) / sum(cellfun(@length, pathlengths, 'UniformOutput', true));
feature_list(10) = {'Average Path Length'};


%% clustering coefficients
if ~exist('sym_edges','var')
    En = 0;
    kn = 1;
else
    [~,network] = graphconncomp(sym_edges);
    
    for n=1:N
        nodes = find(network==n);
        En(nodes) = sum(sum(edges(nodes, nodes)));
        kn(nodes) = length(nodes);
    end
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
if N == iso_nodes
    feats(13) = NaN;
    fprintf('All isolated nodes');
else
    feats(13) = sum( Cn(kn > 1) ) / (N - iso_nodes);
end
feature_list(13) = {'Clustering Coefficient E'};

%14) Number of connected components
feats(14) = length(kn(kn>1));
feature_list(14) = {'Number of connected components'};


%15) Giant connected component ratio
feats(15) = max(kn) / N;
feature_list(15) = {'giant connected component ratio'};

%16) Average Connected Component Size
if N == iso_nodes
    feats(16) = 1;
else
    feats(16) = mean(kn(kn>1));
    feature_list(16) = {'average connected component size'};
end


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
if N == iso_nodes
    feats(23:26) = 0;
    
else
    edge_lengths = weighted(:);
    edge_lengths(edge_lengths==0) = []; % remove zero edge lengths
    
    
    feats(23) = sum(edge_lengths)/length(edge_lengths); % mean edge-length
    feats(24) = std(edge_lengths); % standard deviation
    feats(25) = skewness(edge_lengths); % skewness
    feats(26) = kurtosis(edge_lengths); % kurtosis
end
feature_list(23) = {'mean edge length'};
feature_list(24) = {'standard deviation edge length'};
feature_list(25) = {'skewness edge length'};
feature_list(26) = {'kurtosis edge length'};


%for i = 1:length(feats), fprintf('\n%i %s: %0.2f',i, feature_list{i}, feats(i));end