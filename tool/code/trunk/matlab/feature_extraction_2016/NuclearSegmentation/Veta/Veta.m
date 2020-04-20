function [bounds, nuclei, properties] = Veta(curIM, scales)
% Veta segmentation

curIM_norm = normalizeStaining(curIM);
curIM_normRed=curIM_norm(:,:,1);

p.scales = scales;
disp('Begin nuclei segmentation using multi resolution watershed');
[nuclei, properties] = nucleiSegmentationV2(curIM_normRed,p);

% save centroid and boundary information in bounds
for i = 1:length(nuclei)
    % boundaries
    bounds(i).r = nuclei{i,1}(:,1)';
    bounds(i).c = nuclei{i,1}(:,2)';
    
    % centroids
    bounds(i).centroid_r = properties(i).Centroid(2);
    bounds(i).centroid_c = properties(i).Centroid(1);
    
end

% for i = 1:numel(bounds)
%      plot([bounds(i).r bounds(i).r(1)], [bounds(i).c bounds(i).c(1)]); hold on
%      plot(bounds(i).centroid_c,bounds(i).centroid_r,'r.');
% end

% plot boundaries on the image
figure;imshow(curIM);hold on;
for k = 1:length(nuclei)
    plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 2);
end
hold off;

end