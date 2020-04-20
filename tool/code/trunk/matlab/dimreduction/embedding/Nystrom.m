function [v_est,d,W_est]=Nystrom(W,dim,gamma)
%Nystrom takes in a weight matrix and approximates eigenvectors using the
%Nystrom method
%
%W is a mxp weight matrix, where m is the number of elements used to
%estimate the eigenvectors and p is the number of embedding points
%when m=p the returned eigenvectors are exact.
%
%dim is the dimensionality of the eigenvectors to return
%
% gamma is a scaling factor;
%%%%

    [a,b]=size(W);
    
    [v,d,W_GE, DS_GE]=graphembed(W,dim,gamma);
    
    %if the matrix is square just return graphembedding results
    if a==b 
        W_est = W_GE;
        return;
    end %a==b
    
    %first transform non-square portion of W appropriately;
    W(:,a+1:end) = exp(-W(:,a+1:end)/gamma);
    
    %get estimates of the normalizing vectors
    d2=sum(W(:,a+1:end),1);
    d_est=[DS_GE sqrt(1./d2)];
    
    W(:,1:a) = W_GE;
    W(:,a+1:end) = W(:,a+1:end).*(d_est(1:a)'*d_est(a+1:end));
    
%get the approximate eigenvectors and eigenvalues
    for i=1:dim
        v(:,i) = v(:,i)./d_est(1:a)'; % rescale the original eigenvector appropriately
        v_est(:,i)=(1/d(i))*W'*v(:,i);
        v_est(:,i)=v_est(:,i).*d_est';
        
    end %i
    
    W_est= W(:,a+1:end)'*inv(W(1:a,1:a))*W(:,a+1:end);
end %function