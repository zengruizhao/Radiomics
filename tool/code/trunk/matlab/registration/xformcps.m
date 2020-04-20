function cpointsout = xformcps(cpoints,M)
% XFORMCPS Transform 2D or 3D control points with a homogeneous linear
%   transformation matrix.
%
%   cpointsout = xformcps(cpoints,M)
%
%   Matrix MUST be homogeneous!  Points may be.
%                       2D          3D
%   size(M,1)           3           4
%   size(cpoints,1)     2|3         3|4
%
%JC

dM=size(M,1);
dCP=size(cpoints,1);

if ~any(dCP==[2 3 4]),
    error('size(cpoints,1) must be 2, 3, or 4.'); end

ddiff=dM-dCP;
if ddiff,
    cpoints=[cpoints; zeros(ddiff-1,size(cpoints,2)); ones(1,size(cpoints,2))];
end
cpointsout=M*cpoints;
cpointsout=cpointsout(1:dCP,:);
