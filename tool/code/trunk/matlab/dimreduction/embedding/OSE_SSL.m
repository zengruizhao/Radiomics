function [v_est,d,W_est]=OSE_SSL(W,labels,dim,gamma, options)
%OSE_SSL takes in a weight matrix and labels and approximates eigenvectors using the
%OSE-SSL
%
%W is a mxp weight matrix, where m is the number of elements used to
%estimate the eigenvectors and p is the number of embedding points
%when m=p the returned eigenvectors are exact.
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
%v_est are the estimated eigenvectors
%d_est are teh estimated eigenvalues
%W_est is the estimated weight matrix.
%%%%

    % Initialize options if necessary.
    if ~exist('options', 'var') || isempty(options) || ~isstruct(options)
        options = struct;
    end

    % Determine size of weight matrix
    [a,b]=size(W);
    
    %% Get the training eigenvectors and eigenvalues
    [v,d, W_SSGE, DS_SSGE]=ssGraphEmbed(W(1:a, 1:a),labels,dim,gamma,options);
    
    % If matrix is square return ssGraphEmbed results
    if a==b 
        W_est = W_SSGE;
        return;
    end %a==b
    
%% We first alter the weight matrix (only for OSE portion) according to 3 rules
    W_OSE = W(:, a+1:end);
    
% Rule 1. K Nearest Neighbors 
    if isfield(options, 'k')
        [~,OSE_inds]=sort(W_OSE);
         E_OSE=zeros(size(OSE_inds));
         for l=1:size(OSE_inds,2)
             E_OSE(OSE_inds(1:options.k,l),l)=1;
         end
   
         W_OSE(~E_OSE)=Inf;
    end
    
% Rule 2. All neighbors within a threshold    
    if isfield(options, 'epsilon')
        W_OSE(W_OSE>options.epsilon)=Inf;
    end
    
% Rule 3. Exponential function of all elements
    W_OSE = exp(-W_OSE/gamma);
    
    
%% Estimate the embedding locations
    d2=sum(W_OSE,1);
    d_est=[DS_SSGE sqrt(1./d2)];
    
    W_OSE =W_OSE.*(d_est(1:a)'*d_est(a+1:end));
    
    W = cat( 2, W_SSGE, W_OSE ); 
    
%get the approximate eigenvectors and eigenvalues    
    for i=1:dim
        v(:,i) = v(:,i)./d_est(1:a)'; % rescale the original eigenvector appropriately
        v_est(:,i)=(1/d(i))*W'*v(:,i);
        v_est(:,i)=v_est(:,i).*d_est';
        
    end% i
    
    W_est= W(:,a+1:end)'*inv(W(1:a,1:a))*W(:,a+1:end);
end %function