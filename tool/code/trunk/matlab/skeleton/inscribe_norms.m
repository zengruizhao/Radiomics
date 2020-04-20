function [norms]=inscribe_norms(inter,loc,thresh)
    %first center and normalize vectors
    
        vec=inter-ones(size(inter,1),1)*loc;
        vec=vec./(sqrt(sum(vec.^2,2))*ones(1,3));
        
        vec=vec(~isnan(vec(:,1)),:);
        j=1;
    while ~isempty(vec)

        %find the singular value decomposition of vec.
        [eig_v,eig_d]=eig(vec'*vec);
        [y,i]=max(diag(eig_d));

        %decide if the norm is the positive or negative value:
        %pick the direction with the most vectors in it
        norms(:,j)=eig_v(:,i);
        %discard all vectors within the threshold of the principle eigenvalue
        vec_angles_pos=dot(vec',(ones(size(vec,1),1)*norms(:,j)')');
        vec_pos=vec(cos(thresh)>vec_angles_pos,:);
        
        vec_angles_neg=dot(vec',(ones(size(vec,1),1)*-norms(:,j)')');
        vec_neg=vec(cos(thresh)>vec_angles_neg,:);
        
        if length(vec_neg)>length(vec_pos)
            true_vecs=vec(cos(thresh)<vec_angles_pos,:);
            [eig_v,eig_d]=eig(true_vecs'*true_vecs);
            [y,i]=max(diag(eig_d));

            flip=-1.*(sign(norms(:,j))~=sign(eig_v(:,i)));
            flip(flip==0)=1;
            norms(:,j)=flip.*eig_v(:,i);

            vec=vec_pos;
        else

            true_vecs=vec(cos(thresh)<vec_angles_neg,:);
            [eig_v,eig_d]=eig(true_vecs'*true_vecs);
            [y,i]=max(diag(eig_d));

            flip=-1.*(sign(norms(:,j))==sign(eig_v(:,i)));
            flip(flip==0)=1;
            norms(:,j)=flip.*eig_v(:,i);
            vec=vec_neg;

        end
        if j>2&&isequal(norms(:,j-1),norms(:,j))
            vec=[];
            norms=norms(:,1:j-1);
        end
        j=j+1;
    end
end