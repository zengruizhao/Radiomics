% by cheng lu, on 30rd Dec. 2015
% calculate all cell cluster morphological/subgraphs features.
% the ccg visualization parameter are stroed in CCGinfo.
function [feats,feats_description,CGinfo] = Lextract_CCM_features_v2(bounds,I,properties,para)
% compute the mean and standard deviation of CGTs across all bounds
% c = co-occurrence matrix

CGinfo=[]; % cell graph information
feats=[];
feats_description=[];

set_alpha=[para.CGalpha_min:para.alpha_res:para.CGalpha_max];
for f=1:length(set_alpha)
    curpara.alpha=set_alpha(f); curpara.radius=para.radius;
%     [feat,feat_description,CGinfo{f}] = Lextract_CCM_features_single_correct(bounds,I,properties,...
%         curpara.alpha,curpara.radius);
    [feat,feat_description,CGinfo{f}] = Lextract_CCM_features_single_correct_v2(bounds,I,properties,...
        curpara.alpha,curpara.radius);
%     CGinfo{f}=CGinfotemp;
    %%% get feature description
    temp= feat_description;
    for i=1:length(temp)
        cur=temp{i};
        str=sprintf('-a=%.2f',curpara.alpha);
        cur(end+1:end+length(str))=str;
        temp{i}=cur;
    end
    
    feats=cat(2,feats,feat); feats_description=cat(2,feats_description,temp);
%     fprintf('%d features at %f\n', length(feat),curpara.alpha);
end

end
