function createdataset

Gfiles = what('GraphFeats')
for i = 1:length(Gfiles.mat)
load(['GraphFeats/' Gfiles.mat{i}])
data(i,:) = feats;
end

Mfiles = what('MorphFeats')
for i = 1:length(Mfiles.mat)
load(['MorphFeats/' Mfiles.mat{i}])
data(i,:) = allfeats;
end