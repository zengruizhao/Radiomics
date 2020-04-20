function phi=invmoments(F)
%INVMOMENTS Comput invariant moments of image
% PHI=INVMOMENTS(F) computes the moement invariants of the image F.  PHI is
% a seven-element row vector containing the moment invariants.

%F must be a 2-D, real, nonsparse, numeric 0r logical matrix.

if (ndims(F) ~= 2) | issparse(F) | ~isreal(F) | ~(isnumeric(F) | islogical(F))
    error(['F must be a 2-D, real, nonsparse, numeric or logical matrix.']);
end

F=double(F);
phi=compute_phi(compute_eta(compute_m(F)));
