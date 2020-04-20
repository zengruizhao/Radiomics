function [feats, description] = extract_CLM_features(I, bounds, properties)
    
para.CGalpha_min=0.4; para.CGalpha_max=0.4;
para.alpha_res=0.02;% larger the alpha, sparse the graph
para.radius=0.2;

[feats,description,~] = Lextract_CRL_features_v2(bounds,I,properties,para);
        