function [hosoya_all, CCGinfo] = extract_Hosoya_features(bounds)

para.CGalpha_min=0.4;
para.CGalpha_max=0.4;
para.alpha_res=0.02;% larger the alpha, sparse the graph
para.radius=0.2;

CCGinfo=[];
set_alpha=[para.CGalpha_min:para.alpha_res:para.CGalpha_max];

for f=1:length(set_alpha)
    curpara.alpha=set_alpha(f); curpara.radius=para.radius;
    [hosoya_all{f}, CCGinfo{f}]= Lextract_Hosoya_features(bounds,curpara.alpha,curpara.radius);
end
        
