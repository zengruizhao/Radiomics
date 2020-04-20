function [feats, description] = extract_CRL_features(I, bounds, properties)
    
para.CGalpha_min=0.4; para.CGalpha_max=0.4;
para.alpha_res=0.02;% larger the alpha, sparse the graph
para.radius=0.2;

[I_norm, ~, ~] = normalizeStaining(I);

[feats,description,~] = Lextract_CRL_features_v2(bounds,I_norm,properties,para);
        