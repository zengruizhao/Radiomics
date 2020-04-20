function [cfeats bad_idx N W] = get_callaghan_features(N, W, centroids, neighbors)
% N is logical matrix of potential neighbors (rows) for centroids (cols) returned from 'callaghan_c'
% W is distances between potential neighbors (rows) and centroids (cols) returned from 'callaghan_c'
% centroids is location [x y] of all centroids
% neighbors is location [x y] of all potential neighbors
%
% Outputs:
% cfeats is 22 features for each "valid centroid"
% bad_idx is indices of invalid centroids for which features were not calculated
% N is same as input, but with "bad" centroids removed
% W is same as input, but with "bad" centroids removed
% NOTE: must remove 'bad_idx' from 'centroids' outside of this function, since features were not calculated for them.

%%%% setup

% isolate centroids with fewer than 3 NN
bad_idx = sum(N) < 4;

% cut out "bad" centroids
N = N(:,~bad_idx);
W = W(:,~bad_idx);
centroids = centroids(~bad_idx,:);

% cut unneccessary distances (keep only those from nuclei in neighborhoods)
W = W .* N;

% define cfeats
cfeats = zeros(size(centroids,1),22);


%%%% CALCULATING FEATURES

%%% Features that can be calculated via matrix ops (i.e. no 'for' loop needed) %%%
% NOTE: make sure features calculated over all centroids are in column format

% Number of nearest neighbors
NumNN = sum(N)'; 

% Distances from lumen centroid to nearest neighbors
Dstd = std(W)';
Dmean = mean(W)';
Ddisor = 1 - (1 ./ (1 + Dstd./Dmean));
Dmax = max(W)';
Dmin = zeros(size(centroids,1),1);
for i=1:size(centroids,1), Dmin(i) = min(nonzeros(W(:,i))); end
Drange = Dmax-Dmin;
Dmedian = median(W)';
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%% Features that need to be calculated per lumen (in a 'for' loop) %%%
for a=1:size(centroids,1)
    % general info for current lumen
    N_pos = neighbors(N(:,a),:); % locations of neighboring nuclei
    N_idx = find(N(:,a)); % indices of neighboring nuclei
    pos = centroids(a,:); % location of lumen centroid
    
    % Max circular fit
    CfitDmax=ones(NumNN(a),1)*Dmax(a) - nonzeros(W(:,a));
    Cdmeanmax = mean(CfitDmax); % mean deviation from circle
    
    % Min circular fit
    CfitDmin=nonzeros(W(:,a)) - ones(NumNN(a),1)*Dmin(a);
    Cdmeanmin = mean(CfitDmin); % mean deviation from circle
    
    % Mean circular fit
    CfitDmean = abs(ones(NumNN(a),1)*Dmean(a) - nonzeros(W(:,a)));
    Cdmeanmean = mean(CfitDmean); % mean deviation from circle
 
    % Median circular fit
    CfitDmedian = abs(ones(NumNN(a),1)*Dmedian(a) - nonzeros(W(:,a)));
    Cdmeanmedian = mean(CfitDmedian); % mean deviation from circle
    
    % Angles and distances between adjacent nuclei, in theory.  NOTE: this may not be strictly true; need to think about it
    Ang=[]; DNN=[];
    for b=1:NumNN(a)-1 % for all neighbors of 
        for c=b+1:NumNN(a)
            Ang(end+1)=acosd(dot((N_pos(b,:)-pos),(N_pos(c,:)-pos))/(W(N_idx(b),a)*W(N_idx(c),a)));
            DNN(end+1)=sqrt(sum((N_pos(b,:)-N_pos(c,:)).^2));
        end
    end
    Ang=sort(Ang);
    Ang=Ang(1:NumNN(a));
    amean = mean(Ang); % mean
    astd = std(Ang); % std dev
    adisor = 1 - (1/(1 + std(Ang)/mean(Ang))); % disorder
    
    DNN=sort(DNN);
    DNN=DNN(1:NumNN(a));
    NNDmean = mean(DNN); % mean
    NNDstd = std(DNN); % std dev
    NNDdisor = 1 - (1/(1 + std(DNN)/mean(DNN))); % disorder
    
    %%% Elliptical fit %%%
    fitelli=fitellipse(N_pos(:,1),N_pos(:,2));
    
    % quantify how evenly nuclei are distributed along major axis
    x = fitelli(3) * cos(fitelli(5));
    y = fitelli(4) * sin(fitelli(5));
    nx = x*cos(fitelli(5))-y*sin(fitelli(5)) + fitelli(1);
    ny = x*sin(fitelli(5))+y*cos(fitelli(5)) + fitelli(2);
    slp=(ny-fitelli(2))/(nx-fitelli(1));
    C=fitelli(2)-slp*fitelli(1);
    Axeven=N_pos(:,2)-(slp*N_pos(:,1)+C);
    AxevenV = abs( nnz(Axeven>=0) - nnz(Axeven<0) );

    % length of major (or minor?) axis 
    fitelli3=abs(fitelli(3));
    
    % length of minor (or major?) axis ... whatever, it's the other axis
    fitelli4=abs(fitelli(4));
    
    % distance between nuclei and best fit ellipse
    t = 0:.01:pi*2;
    x = fitelli(3) * cos(t);
    y = fitelli(4) * sin(t);
    nx = (x*cos(fitelli(5))-y*sin(fitelli(5)) + fitelli(1))';
    ny = (x*sin(fitelli(5))+y*cos(fitelli(5)) + fitelli(2))';
    for b=1:NumNN(a)
        pos1=ones(length(t),2);
        pos1(:,1)=N_pos(b,1);
        pos1(:,2)=N_pos(b,2);
        pos2=([nx ny]-pos1).^2;
        Dfit(b)=min(sqrt(pos2(:,1)+pos2(:,2)));
    end
    
    Dfitmean=mean(Dfit); % mean
    Dfitstd=std(Dfit); % std dev
    Dfitdisor= 1 - (1 / (1 + Dfitstd/Dfitmean)); % disorder
    
    
    % compile feature vector for current lumen (only per lumen features; tack on other afterwards)
    cfeats(a,1)=NumNN(a);
    cfeats(a,2)=Dstd(a);
    cfeats(a,3)=Dmean(a);
    cfeats(a,10)=Ddisor(a);
    cfeats(a,11)=Dmax(a);
    cfeats(a,12)=Drange(a);
    cfeats(a,4)=Cdmeanmax;
    cfeats(a,5)=Cdmeanmin;
    cfeats(a,13)=Cdmeanmean;
    cfeats(a,14)=Cdmeanmedian;
    cfeats(a,6)=amean;
    cfeats(a,7)=astd;
    cfeats(a,15)=adisor;
    cfeats(a,8)=NNDmean;
    cfeats(a,9)=NNDstd;
    cfeats(a,16)=NNDdisor;
    cfeats(a,17)= AxevenV;
    cfeats(a,18)= fitelli3;
    cfeats(a,19)= fitelli4;
    cfeats(a,20)= Dfitmean;
    cfeats(a,21)= Dfitstd;
    cfeats(a,22)= Dfitdisor;
end