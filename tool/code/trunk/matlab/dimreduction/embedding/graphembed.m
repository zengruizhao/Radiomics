function [v,d,W,ds] = graphembed(W,m,gamma)
% [v,d,W,ds] = graphembed(W,m,gamma)
% [v,d,W,ds] = GRAPHEMBED(W,m,gamma) Graph Embedding
% 
% W is NxN matrix of N samples. v are the Nxm embeddings corresponding 
% to the m eigenvalues d.
% W is the transformed matrix, and ds is the diagonal scaling factor.
% James Monaco
% Edit - To use compiled 64-bit distances MEX file for use on the cluster to calculate distance vector 
%      - To load raw feature data using volume names in cell format
% Rachel Sparks
% Edit - To return ds and W;

%for vn = 1:numel(vols)

%	fname = vols{vn};
%	disp(fname);
%	load(['MRS+MRI/',fname,'.mat']);

	debug = false;

%     if m >= size(data,2)
%         error('m must be < size(X,2).');
%     end
%     if isa(data,'single'), data = double(data); end;
% 
% 	% distmtx needs  D x N so transpose
% 	W = distmtx(data');
% 	clear data
	W = exp(-W/gamma);
	%W = exp(-W/500);
    
    if debug
	    D = diag(sum(W));
	    %L = D - W; %#ok<NASGU>
    end
    
    if debug, disp('Calculating normalized cuts..'); end;
	ds = 1./sqrt(sum(W));

	%Right and left multiply by D^{-1/2}.
	for j=1:size(W,1)
	  W(j,:) = W(j,:).*ds;
	end
	for j=1:size(W,2)
	  W(:,j) = W(:,j).*ds';
	end

	options.disp = 0; options.isreal = 1; options.issym = 1;
    [v,d] = eigs(W,m+1,'LR',options);
	[d,sind] = sort(diag(d),'descend');
	v = v(:,sind(2:end));
	d = d(2:end);

	%Left multiply by D^{-1/2}.
	for j=1:m
	  v(:,j) = v(:,j).*ds';
	end

	wsse = 2*d;

	if debug
	    %L = diag(sum(W)) - W;
	    disp('********************* DEBUG ************************');
	    wsse/2 %#ok<NOPRT>
	    v'*D*v %#ok<NOPRT>
	    %v'*L*v %#ok<NOPRT>
	    disp('********************* DEBUG ************************');
	end


end
