function [CGT, c, info, feats,network,edges] = extract_CGT_features(bounds,a,r)
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
info.angle_bin_size = 10; % default: 10 degree bins
info.angular_adjust = 0; % default: sets <1,0> at 0 degrees

if info.angular_adjust >= 180
    error('please select angular adjustment between 0 and 180')
end

i = 1;
% fprintf('account for circular bounds\n')
% circular = zeros(1,length(bounds));
% for j = 1:length(bounds)
%     if abs(bounds(j).features.aspect_ratio - 1) < 0.1
%         circular(j) = 1;
%     end
% end

% calculate principal axis
[axis] = fitEllipseToBoundary(bounds);

% adjust vectors to all point up
for j = 1:length(axis(:,1))
    if axis(j,2) <0
        axis(j,:) = -axis(j,:);
    end
end

% calculate angles from principal axis
%    angle_degrees=radtodeg(atan(axis(:,2)./axis(:,1)))

angle_degrees = 180/pi *atan(axis(:,2)./axis(:,1)) + 90;

angle_degrees = angle_degrees + info.angular_adjust;

%% entropy of co-occurence

w = info.angle_bin_size; % width of bin
%discretize angles to every d degrees
discrete_angles = floor(angle_degrees/w)*w;

% account for circular glands
%discrete_angles(circular==1) = 0;

% account for greater than or equal to 180
discrete_angles(discrete_angles >= 180) = discrete_angles(discrete_angles >= 180) - 180;

% build graph
alpha = info.alpha;
r = info.radius;
[VX,VY,x,y,edges] = construct_ccgs_optimized(bounds,alpha, r);

% initialize co-occurence
bin = [0:w:180-w]; % discretizations!!!
%p = zeros(180/w, 180/w);

%% based on number of neighborhoods rather than number of bounds
for j = 1:length(bounds)-1
    for k = j+1:length(bounds)
        edges(k,j) = edges(j,k);
    end
end

% find gland networks
[numcomp,group] = graphconncomp(sparse(edges));

%max(network)

for ii = 1:max(group) % define a neighborhood for each gland network (number of neighborhoods = number of networks)
    p = zeros(180/w, 180/w);
    
    neighborhood_angles = discrete_angles(group == ii); % aggregate angles in gland network
    %w = 5 % size of discretization
    % p = hist(angles,180/w) %
    
    %discrete_angles(i)
    
    % aggregate co-occurrences
    n=0;
    for jj = 1:length(bin) % find out how often each pair of bins co-occur
        for kk = jj:length(bin)
            
            if sum(ismember(neighborhood_angles, bin(jj))) && sum(ismember(neighborhood_angles, bin(kk)))
                p(jj,kk) = p(jj,kk) + 1; % add instances to co-occurrence
            end
            n = n+1;
        end
    end
    
    c(ii) = {p./n}; % normalize co-occurence matrix
    feats{ii} = haralick_no_img(c{ii});
end

% remove single gland networks
temp_network = hist(group,numcomp);
[a,group_ind] = find(temp_network > 1); 

% remove networks that don't give co-occurrence features
for k = 1:length(feats{1}.names)
    n = 0;
    for i = 1:length(feats)
        if isfield(feats{i},'val')
        else
            group_ind(group_ind == i) = [];
            continue
        end
    end
end
[origgroup_ind,newgroup_ind] = sort(group_ind,'ascend');

% define network and apply new network indices following network prunes
network = []; 
num_connected_comp = max(newgroup_ind);

for j = 1:num_connected_comp
    network(group == origgroup_ind(j)) = newgroup_ind(j);
end

% replicate feats based on the size of network
%netfeats = feats;
for j = 1:max(group)
    networksize(j) = sum(group == j);
end



% check should have 7300 bounds
sum(networksize(networksize>1)); % check 7035 bounds for all neighborhoods > 1 node

%% mean and standard deviation across bounds for each haralick feature
for k = 1:length(feats{1}.names)
    feat = [];
    n = 0;
%     for i = 1:numcomp
%         if isfield(feats{i},'val')
%             n = n + 1;
%             feat(n) = feats{i}.val(k);
%         else
%             continue;
%         end
%     end
    for i = 1:length(feats)
        if isfield(feats{i},'val')
            n = n + 1;
            feat(end+1:end+networksize(i)) = repmat(feats{i}.val(k),1,networksize(i));
        else
            continue
        end
    end
    
    if n > 0
        CGT(k*3-2) = mean(feat);
        CGT(k*3-1) = std(feat);
        CGT(k*3) = range(feat);
    else
        CGT(k*3-2) = 0;
        CGT(k*3-1) = 0;
        CGT(k*3) = 0;
    end
end

info.alpha = alpha;
info.radius = r;
info.angle_bin_size = w;

count = 1;
modifier = [{'mean '} {'standard deviation '} {'range '}];
for j = 1:numel(feats{1}.names)
for i = 1:numel(modifier)
    CGTFeatureDescription{count} = [modifier{i} 'tensor ' feats{1}.names{j}];
    count = count + 1;
end
end
    