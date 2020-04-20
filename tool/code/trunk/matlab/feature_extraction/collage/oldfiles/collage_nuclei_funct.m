%This function should provide a summary of collage nuclei features
function [collagefeats, collagelabels,param]=collage_nuclei_funct(origImage, tumorMask, MPP, ext, vargin)
if nargin <4
    ext='';
end
%tumorMask = imclearborder(tumorMask);
[feature_concat,param]=compute_CoLlAGe2D_update(origImage, tumorMask, MPP);
collagelabels{1}=['Collage:Max', ext];
collagelabels{2}=['Collage:Mean', ext];
collagelabels{3}=['Collage:Median', ext];
collagelabels{4}=['Collage:Min', ext];
collagelabels{5}=['Collage:Stdev', ext];
collagefeats=[max(feature_concat), mean(feature_concat), median(feature_concat), min(feature_concat), std(feature_concat)];