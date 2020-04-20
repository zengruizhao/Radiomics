function B=bound2im(b,M,N,x0,y0)
%Bound2IM converts a boundary to an image.
%   B=bound2im(b) converts b, an npx2 or 2xnp into a binary image with 1s
%   in the locations defined by the coordinates in b and 0s elsewhere.
%
%B=bound2im(b,M,N) places the boundary approximately centered in an MxN
%image.  If any part of the boundary is outside the MxN rectaangle, an
%error is issued.
%
%B=Bound2IM(b,M,N,x0,y0) places the boundary in an image of size mxn, with
%the topmost boundary point located at x0 and the left more point located
%at yo.  if the shifted boundary is outside the mxn rectangle, an error is
%issued.  x0 and y- must be positive intergers.

[np, nc]=size(b);
if np<nc
    b=b'; % to convert to size np by 2
    [np,nc] = size(b);
end
%make sure the coordinates are integers.

x=round(b(:,1));
y=round(b(:,2));
%set up the default size paramters.
x=x-min(x)+1;
y=y-min(y)+1;
B=false(max(x),max(y));
C=max(x)-min(x)+1;
D=max(y)-min(y)+1;

if nargin == 1
    %Use the preceding default values
elseif nargin == 3
    if C > M | D > N
        error('The boundary is outside the m-by-n region.');
    end
    %The image size will be mxn.  set up parameters for this
    B=false(M,N);
    %Distribute extra rows approx. even between top and bottom
    NR=round((M-C)/2);
    NC=round((N-C)/2);  % the same for colums
    x=x+NR;  % offset the bundary to new position
    y=y+NC;
elseif nargin == 5
    if x0 < 0 | y0 < 0
        error('x0 and y0 must be positive integers.');
    end
    x=x+round(x0)-1;
    y=y+round(y0)-1;
    C=C+x0-1;
    D=D+y0-1;
    if C>M | D>N
        error('The shifted boundary is outside of mxn region.');
    end
    B=false(M,N);
else 
    error('Incorrect number of inputs.');
end
B(sub2ind(size(B),x,y))=true;

    