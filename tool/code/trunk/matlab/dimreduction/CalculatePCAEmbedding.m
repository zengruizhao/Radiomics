%  
% Fast Principal Componenet Analysis (PCA) on a set of data
% 
% Usage: [eigenvectors, eigenvalues, means] = CalculatePCAEmbedding(data, options)
% 
% data is in observations x dimensions
% If length(size(data)) > 2, then the data is first collapsed and then
% expanded before being return.  Hence, size(means) will retain the same
% shape as the 2+ dimensions of data.  For example, if data is
% 100x256x256x3 that means there are 100 observations of images of size
% 256x256x3.  means will be 256x256x3 and eigenvectors will be dx256x256x3.
% 
% options (optional) is a struct containing the "keep" field
%   keep (default = 0.98) can be a percentage, indicating how much variance
%   to keep, or an integer containing how many eigenvectors to keep
% 
% Created by Rob Toth
% 
function [eigenvectors, eigenvalues, means] = CalculatePCAEmbedding(data, options)

    % options
    if ~exist('options', 'var') || isempty(options) || ~isstruct(options), options = struct; end
    if ~isfield(options, 'keep'), options.keep = 0.98; end
    
    % initialize the data
    sizes = size(data);
    needs_resizing = length(sizes) > 2;
    observations = sizes(1);
    dimensions = prod(sizes(2:end));
    if needs_resizing
        data = reshape(data, [observations, dimensions]);
    end
    
    % can't be integer for mean and cov to work properly
    if isinteger(data), data = double(data); end
        
    % calculate mean and cov
    means = nanmean(data, 1);
    data = data - repmat(means, [observations, 1]);
    data(isnan(data)) = 0; % changed all the "NaN's"
    covariance = 1/observations * (data * data');
    
    % calculate eigenvectors and eigenvalues
    [eigenvectors, eigenvalues] = eig(covariance);
    eigenvectors = eigenvectors' * data;
    eigenvalues = diag(eigenvalues);
    
    % sort the eigenvalues
    [~, order] = sort(eigenvalues, 'descend');
    eigenvalues = eigenvalues(order);
    eigenvectors = eigenvectors(order, :);
    
    % only select a few eigenvectors
    if options.keep <= 0
        keep = 1;
    elseif isinteger(options.keep)
        keep = options.keep;
    elseif options.keep >= 1
        keep = round(options.keep);
    else % it's the variance percentage
        variance = cumsum(eigenvalues) / sum(eigenvalues);
        keep = find(variance >= options.keep, 1, 'first');
    end % keep
    if keep < 1, keep = 1; end
    eigenvalues = eigenvalues(1:keep);
    eigenvectors = eigenvectors(1:keep, :);
    
    % normalize - NECESSARY!
    for i = 1:keep
        eigenvectors(i, :) = eigenvectors(i, :) / norm(eigenvectors(i, :));
    end % for i
    
    % now, resize back
    if needs_resizing
        means = reshape(means, sizes(2:end));
        eigenvectors = reshape(eigenvectors, [keep, sizes(2:end)]);
    end % if needs_resizing

end % function