function [ik,kcenters] = jkmeans(data,k,varargin)
% JKMEANS
%   [IK,KCENTERS] = jkmeans(DATA,K,MAXDIST,MAXITER,PRECENTERS);
%
%   IK - cluster membership array. same size as data
%   KCENTERS - cluster centers
% 
%   DATA - NxD array, where D is the number of parameters of variables of 
%       the data
%   K - number of clusters
%   MAXDIST - maximum distance change in N-dimensional space for stopping
%   (zero is a good choice)
%   MAXITER - maximum number of iterations - optional (default = 60)
%   PRECENTERS - optional cluster centers to start with, <=K centers
%
% Jonathan Chappelow

if k<2,
    error('Need more than one cluster.');
end

% default values
maxiter=60;

% parse inputs
if ~isempty(varargin),
    max_dist=varargin{1};
    if isempty(max_dist),
        max_dist=0.0001;
    end
    if length(varargin)>1,
        maxiter=varargin{2};
        if isempty(maxiter) || ~(maxiter>0),
            maxiter=60;
        end
        if length(varargin)>2,
            precenter=varargin{3};
        end
    end
end

ik=[];
kcenters=[];

% prepare data
if ~isfloat(data), data=double(data); end
data(isnan(data))=0;    % kill NaNs
% assume data already straightened OR grayscale unstraightened
[yd,cdims]=size(data);

% Begin clustering
fprintf('Selecting suitable initial cluster means... ');
newcenters=1;
while newcenters;
    newcenters=0;
%     max_dist=.0001;
    % uniform random selection of color space:
    % kcenters=rand(1,k) + min(data);
    
    % Pick initial cluster centers:
    % 1. uniform random selection of data space within data range:
    kcenters=rand(cdims,k) .* repmat(max(data)'-min(data)',1,k) + repmat(min(data)',1,k);
    fprintf('random centers selected...\n');
    
    % 2. sample from data - bad if there is a lot of smooth background
    % kcenters=[data(round(rand(1,k)*yd),:)]';
    
    % 3. from input...
    if exist('precenter','var'),
        sp=size(precenter,2);
        kcenters(:,1:size(precenter,2))=precenter;
        display(kcenters)
        if k>sp,
            % user-specified k-centers
            kcentermat_spec=repmat(permute(kcenters(:,1:sp),[1 3 2]),1,k-sp);
            
            % choose the remaining required k-centers randomly
            kcentermat_rand=repmat(kcenters(:,(sp+1):end),[1 1 sp]);
            
            % calculate distance of randomly chosen centers from specified centers
            center_dists=squeeze(sqrt(sum((kcentermat_spec-kcentermat_rand).^2,1)))'; % vector euclidean
            abs_colordiffs=squeeze(abs(kcentermat_spec-kcentermat_rand)); % each element absolute

            % distances between specified k-centers
            allcenter_dists=squeeze( sqrt(sum( (repmat(kcenters,[1 1 k]) - ...
                repmat(permute(kcenters, [1 3 2]),[1 k 1])).^2,1)) );
            allcenter_dists=triu(allcenter_dists);
            allcenter_dists=allcenter_dists(allcenter_dists>0)';
            
            % Make sure k-centers are not too close
            if any(center_dists(:)<30) || any(abs_colordiffs(:)<15) || any(allcenter_dists<30),
                newcenters=1; % too close, choose again
            end
        end
        fprintf('some user-specfied center(s) used...\n');
    end

    % Begin cluster mean calculations
    % warning off;
    moved=1;
    iter=1;
    oldclustersizes=zeros(1,k);
    clustersizes=zeros(1,k);
    while moved && iter<=maxiter && ~newcenters,
        % Use a ton of memory to calculate all high-dimensional distances at once
%         distances=sqrt(sum( ( repmat(data',[1 1 k]) - ... 
%             repmat(permute(kcenters, [1 3 2]),[1 yd 1]) ).^2, 1) );
        kdiffs=bsxfun(@minus,data,permute(kcenters,[3 1 2]));
        distances=sum(kdiffs.^2,2); % sqrt(sum(kdiffs.^2,2))
        distances=squeeze(distances);
        
        % For every point, find which cluster center is closest
        [mindist,ik]=min(distances,[],2);
        
        % Calculate the new cluster centers (mean the data)
        kcentersnew=zeros(cdims,k);
        for i=1:k,
            indk=find(ik==i);
            clustersizes(i)=length(indk);
            kcentersnew(:,i)=mean(data(indk,:))';
        end

        % Verify that each cluster has values
        if any(any(isnan(kcentersnew))),    % if not...
            newcenters=1;
            break;  % pick new cluster centers and start over
        else
            newcenters=0;
            % warning on;
        end
        
        % Calculate movement of each cluster center in the color space
        movement=sqrt(sum((kcenters-kcentersnew).^2,1));
        
        % Display movement in colorspace of each cluster center:
        % fprintf('Movement ');fprintf('%4.4f ',movement);fprintf('\n');
        
        % Distplay change in cluster center size
        if iter==1,
            fprintf('Initial size ');
        else
            fprintf('change       ');
        end
        fprintf('%6.0f ',clustersizes-oldclustersizes);fprintf('\n');
        
        % Calculate movement from colorspace distance or cluster size change
        moved=any(movement>max_dist);
        % moved=any(clustersizes-oldclustersizes);
        
        kcenters=kcentersnew;
        oldclustersizes=clustersizes;
        iter=iter+1;
    end
end

fprintf('Final size   ');fprintf('%6.0f ',clustersizes);fprintf('\n');

% assign pixel values in order of cluster size
npix=zeros(k,1);
for i=1:k,
    ipix{i}=find(ik==i); %#ok<AGROW>
    npix(i)=length(ipix{i});
end
[y,ind]=sort(npix);
for i=1:k,
    ik(ipix{ind(i)})=i;
end
