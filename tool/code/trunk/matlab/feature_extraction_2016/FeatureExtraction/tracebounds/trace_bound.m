function bound = trace_bound(bimg,start_i,start_j)
% BOUND = TRACE_BOUND(BIMG)
%    TRACE_BOUND expects BIMG to be a matrix of zeros and ones.  The ones 
%    represent the object or object boundary.  BOUND is an Nx2 matrix containing
%    the N boundary indices in [row column] format.  The boundary is closed.
%    The optional parameters START_I and START_J can be used to 
%    provide a starting point on the boundary.  They must indicate the leftmost boundary 
%    point on a particular row.

if nargin == 1
    [i,j] = find(bimg);
    [junk,index] = min(i);

    start_i = i(index);
    start_j = j(index);
end

[rc,cc] = size(bimg);

ibox = [0  -1 -1 -1 0 1 1  1]; %cw from upper left corner
jbox = [-1 -1  0  1 1 1 0 -1];

cnt = 5;
i = start_i;
j = start_j;
bound = [start_i start_j];
while 1
    cnt = mmod(cnt+5,8);
    for k = 1:8
        i2 = i+ibox(cnt); j2 = j+jbox(cnt);
        if i2<1 || i2>rc || j2<1 || j2>cc || bimg(i2,j2)==0
            cnt = mmod(cnt+1,8);
        else
            break;
        end
    end
    i = i2; j = j2;
    if (i == start_i && j == start_j) || k == 8
        break;
    end
    bound(end+1,:) = [i j];
end