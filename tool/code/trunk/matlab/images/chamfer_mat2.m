function val=chamfer_mat2(neigh)
%Chamfer_Mat2 Applies the second chamfer matrix transform
%
%Chamfer_mat2(neigh) Is passed a neighborhood and returns the pixel value.
%
%Neigh is a 3x3 block with the center pixel the pixel whos value is being
%changed.
%
%See also chamfer_map, chamfer_mat1
%Rachel Sparks

%The sliding block distance transform
dist_trans=[NaN NaN NaN;NaN 0 10;14 10 14];

%Apply the sliding block based on the value being applied.
    if neigh(2,2)>0
        inter=neigh+dist_trans;
        val=min(min(inter));
    else 
        inter=neigh-dist_trans;
        val=max(max(inter));
    end
end