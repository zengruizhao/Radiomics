function [set_idx,datas,feature_lists]=Lfind_featureidx_by_excluded_name(feature_name_set,data,feature_list)

set_idx=[];
for j=1:length(feature_name_set)
    feature_name=feature_name_set{j};
    for i=1:length(feature_list)
        if ~isempty(strfind(feature_list{i},feature_name))
            set_idx=[set_idx i];
        end
    end
end
datas=data(:,setdiff(1:size(data,2),set_idx));
feature_lists=feature_list(setdiff(1:length(feature_list),set_idx));