function val=chamfer_mat1(neigh)

%Chamfer_Mat1 Applies the first chamfer matrix transform
%
%Chamfer_mat1(neigh) Is passed a neighborhood and returns the pixel value.
%
%Neigh is a 3x3 block with the center pixel the pixel whos value is being
%changed.
%
%See also chamfer_map, chamfer_mat2
%Rachel Sparks

%Distance Transform
dist_trans=[14 10 14; 10 0 NaN;NaN NaN NaN];

    if neigh(2,2)>0
        inter=neigh+dist_trans;
        val=min(min(inter));
    else 
        inter=neigh-dist_trans;
        val=max(max(inter));
    end


end