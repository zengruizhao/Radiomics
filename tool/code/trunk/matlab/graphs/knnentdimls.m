function [m,H,knnl_mean,knnl_std] = knnentdimls(data, k, sampsizes, N)
% KNNENTDIMLS Simultaneous estimation of intrinsic entropy and
%   dimensionality by characterizing growth rate of kNN graph lengths.
%
%   [m, H] = KNNENTDIMLS(DATA, k, SAMPSIZES, N) calculates intrinsic
%   dimensionality m and entropy H from data array DATA of size
%   npoints-by-ndims using k neighbors to construct the kNN graphs. Graphs
%   are constructed for random samples of SAMPSIZES points in DATA, where
%   SAMPSIZES in the Q-by-1 array of different sample sizes. For each of
%   the Q sample sizes, kNN length is calculated for N different random
%   samples.
%
%   [m, H, KNNL_MEAN, KNNL_STD] = KNNENTDIMLS(...) returns optional
%   output arguments KNNL_MEAN and KNNL_STD, Q-by-1 arrays containing the
%   mean and std of the kNN lengths for each of the Q sample sizes, 
%   calculated over the N random samplings.
%
% Based on the alorithm in Table II of:
%
%   J. A. Costa and A. O. Hero, "Geodesic entropic graphs for dimension and
%   entropy estimation in manifold learning", IEEE TSP 52(8), pp 2210-2221.
%   http://ieeexplore.ieee.org/search/wrapper.jsp?arnumber=1315941
%
%   IMPORTANT NOTE:
%   This function uses two functions of Hero and Costa's making. These are,
%   (1) estimation of beta values for a range of k and dimensions, and (2)
%   kNNlengthmex to calculate the total length of the kNN graph.
%
% Jon Chappelow

% Load beta estimates from uniform distrib. (from Hero and Costa's routine)
load renyi_betas_dk_new.mat beta_hat
[nbetadims,nbetaks]=size(beta_hat);
if k > nbetaks, error('No pre-calculated beta for k of %g.',k); end
gamma=1;

% Check number of points, dimensions, and sample sizes
[npts,ndims]=size(data);
nsampsizes=numel(sampsizes); % sampsizes is p_1,...,p_Q in the paper

% Preallocate arrays for mean and std of kNN lengths for each sample size
knnl_mean=zeros(nsampsizes,1);
knnl_std=zeros(nsampsizes,1);

% Preallocate array for kNN lengths at each of N trials performed for each sample size
knnlens=zeros(N,1);

% Calculate kNN lengths for a linear system from multiple data sample sizes
for p=1:nsampsizes,
    % Compute kNN graph length for a given sample size for multiple random samples
    nsamps=sampsizes(p);
    for it = 1:N,
        % Randomly sample data
        % shuffledinds=randperm(npts);
        shuffledinds=randomizemex(npts); % my mex fuction to shuffle data (faster than randperm)
        randsampinds=shuffledinds(1:nsamps);
        sampled_data=reshape(data(randsampinds,:)',[nsamps*ndims 1]);
        
        % Calculate kNN graph length using Costa and Hero's mex function
        knnlens(it)=kNNlengthmex(sampled_data, nsamps, ndims, k, gamma);
    end
    
    % Sample Mean & std of kNN graph lengths over each of N trials
    knnl_mean(p)=mean(knnlens);
    knnl_std(p)=std(knnlens);
end

% Solve the system generated above to obtain m and H estimates:
%  l_bar = A * [a; b] + err

% l_bar: log lengths of kNN graphs
l_bar=log(knnl_mean);

% A: log sample size of the nsampsizes samples
A = [log(sampsizes(:)) ones(nsampsizes,1)];

% Solve for [a; b]
ab = A\l_bar; % == pinv(A)*l_bar == (A'*A)\A'*l_bar;
a_hat=ab(1); b_hat=ab(2);

% First, calculate m_hat
m=gamma/(1-a_hat);
m_hat=round(m);

% Make sure we have a beta for the m_hat
if m_hat > nbetadims || m_hat < 1,
    error('No pre-calculated beta for m of %g (%g).',m_hat,m);
end

% Finally, calculate entropy H using calculated m
H = (m_hat/gamma)*(b_hat-log(beta_hat(m_hat,k))) * log2(exp(1));
% H = (m/gamma)*(b_hat-log(beta_hat(m_hat,k))) * log2(exp(1));
% express in terms of bits. for other bases use logX(exp(1)), or no conversion for nats
