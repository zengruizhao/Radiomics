% 
% Usage: [projection, reconstruction] = CalculatePCAProjection(means, eigenvectors, eigenvalues, data)
% 
% Takes in a PCA embedding, and a new datapoint, and calculates the
% projection.
% 
% data is a 
% 
% If means == [], then it is assumed they are 0, or that you have already
% mean-subtracted the data.
% 
% If eigenvalues ~= [], then the projections will be in terms of standard
% deviations from the mean.  Otherwise, the projections will be the raw
% projection values.
% 
% The optional second output will recalculate a reconstruction of the
% original, high dimensional data given the low dimensional projection.
% 
% Created by Rob Toth
% 
function projection = CalculatePCAProjection(means, eigenvectors, eigenvalues, data)

  % what data has been input?
  no_eigenvalues = isempty( eigenvalues );
  no_means       = isempty( means       );

  % are the sizes good?
  size_e = size(eigenvectors);
  valid = ...
          (no_eigenvalues || size_e(1)           == length(eigenvalues)) && ...
          (no_means       || numel(data)         == numel(means))        && ...
                             prod(size_e(2:end)) == numel(data);

  if valid

    % reshape everything
    eigenvectors = reshape(eigenvectors, size_e(1), prod( size_e(2:end) ));

    % calculate the inverse
    if ~no_means
      data = data(:) - means(:);

      % assume that if we've already subtracted out the mean, we've
      % already eliminated any nan's
      nans = isnan(data);
      if any(nans) % can't handle NaN's very well...
        data(nans) = 0;
      end

    end % have you already mean-subtracted it?

    projection = eigenvectors * data(:);
    %b = eigenvectors(:, ~nans) * temp(~nans); % this goes slower

    % divide by eigenvalues if they exist
    if ~no_eigenvalues
      projection = projection ./ sqrt(eigenvalues(:));
    end

  else 
      projection = 0;
  end % if valid

end % function
