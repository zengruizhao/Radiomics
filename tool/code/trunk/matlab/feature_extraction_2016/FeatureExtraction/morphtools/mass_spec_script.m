feats = zeros(25,11334);

for ii = 1:11334
    if(mod(ii,1000) == 0)
        fprintf('Bound %d\n',ii);
    end
    
    r = bounds(1,ii).r;
    c = bounds(1,ii).c;
    feats(:,ii) = morph_features(c', r');
end