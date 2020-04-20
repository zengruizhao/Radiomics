function [v,d,W,ds]=ssGraphEmbed (W,labels,dim,gamma,options)
% ssGraphEmbed takes in a square weight matrix and labels and calculated
% eigenvectors
%
%W is a mxm weight matrix, where m is the number of elements used to
%estimate the eigenvectors
%
%labels is a 1xm matrix of relevant sample labels
%
%dim is the dimensionality of the eigenvectors to return
%
%gamma is the scaling factor
%
%options gives how the weight matrix should be altered (option.k set if you
%want the include k nearest neighbors options.epsilon set if you want all
%neighbors within a specific distance).
%
%v are the top eigenvectors
%d are the top eigenvalues
%W is the weight matrix after all appropriate transformations.
%ds is the diagnoal scaling factor (really only useful for calculating OSE)
%%%%   

    % Initialize options if necessary.
    if ~exist('options', 'var') || isempty(options) || ~isstruct(options)
            options = struct;
    end

%% We first alter the weight matrix according to 4 rules

% Rule 1. K Nearest Neighbors 
    if isfield(options, 'k')
        [~, ~, E] = Isomap(W, 'k', options.k, []);
        W(~E)=Inf;
    end
    
% Rule 2. All neighbors within a threshold    
    if isfield(options, 'epsilon')
        W(W>options.epsilon)=Inf;
    end
    
% Rule 3. Exponential function of all elements
	W = exp(-W/gamma);

% Rule 4. Alter according to label informations
    if ~isempty(labels);
        %find all non zero components
        [i,j]=find(W~=0);
        
        % If the labels are the same make them more similar
        same=labels(i)==labels(j)&~isnan(labels(i))&~isnan(labels(j));
        isame=sub2ind(size(W),i(same),j(same));
        W(isame)=W(isame).*(1+W(isame));

        % If the labels are the different make them less similar
        diff=labels(i)~=labels(j)&~isnan(labels(i))&~isnan(labels(j));
        idiff=sub2ind(size(W),i(diff),j(diff));
        W(idiff)=W(idiff).*(1-W(idiff));
    end
    
%% Alter W to get into the correct form    
    ds = 1./sqrt(sum(W));
    
    %Right and left multiply by D^{-1/2}.-- Made slightly more efficient
    W= W.*(ds'*ds);
    %%%%%%%%%%%%%%% below was the original code%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%for j=1:size(W,1)
	%  W(j,:) = W(j,:).*ds;
	%end
	%for j=1:size(W,2)
	%  W(:,j) = W(:,j).*ds';
	%end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Calculate eigenvectors
	options.disp = 0; options.isreal = 1; options.issym = 1;
    [v,d] = eigs(W,dim+1,'LR',options);
	[d,sind] = sort(diag(d),'descend');
	v = v(:,sind(2:end));
	d = d(2:end);

	% Left multiply by D^{-1/2} and return.
	for j=1:dim
	  v(:,j) = v(:,j).*ds';
    end
    
end % function