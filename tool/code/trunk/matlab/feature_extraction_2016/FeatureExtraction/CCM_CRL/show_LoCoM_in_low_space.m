%% load pre computed feature sets
load OralCavityFullFeatures_epistroma.mat
%% feature selection using ttest, without data whitening
K=100;

dataw_epi=data_epi;
dataw_stroma=data_stroma;
dataw_epistroma=data_epistroma;

% 
% dataw_epi=simplewhiten(data_epi);
% dataw_stroma=simplewhiten(data_stroma);
% dataw_epistroma=simplewhiten(data_epistroma);

[TTidx,confidence_epi] = prunefeatures_new(dataw_epi, labels, 'ttestp');
idx_TTest_epi=TTidx(1:K);
set_confidence_TTest_epi=confidence_epi(1:K);
setTopF_TTest_epi=feature_list_epi(idx_TTest_epi)';

[TTidx,confidence_stroma] = prunefeatures_new(dataw_stroma, labels, 'ttestp');
idx_TTest_stroma=TTidx(1:K);
set_confidence_TTest_stroma=confidence_stroma(1:K);
setTopF_TTest_stroma=feature_list_stroma(idx_TTest_stroma)';

[TTidx,confidence_epistroma] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');
idx_TTest_epistroma=TTidx(1:K);
set_confidence_TTest_epistroma=confidence_epistroma(1:K);
setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest_epistroma)';
%% plot top 3 features in 3d space
idx_TTest=idx_TTest_epistroma;
dataw=dataw_epistroma;
data_r=dataw(:,idx_TTest(1:3));
dataPCA=data_r;
labels=logical(labels);
%%% show in figure
figure;hold on;t = 0.001;
plot3(dataPCA(labels,1),dataPCA(labels,2),dataPCA(labels,3),'ks','MarkerSize',20,'MarkerFace','k');
% [A , C] = MinVolEllipse([dataPCA(1:5,1),dataPCA(1:5,2),dataPCA(1:5,3)]', t);
%  Ellipse_plot(A,C);
plot3(dataPCA(~labels,1),dataPCA(~labels,2),dataPCA(~labels,3),'rp','MarkerSize',20,'MarkerFace','r');
% [A , C] = MinVolEllipse([dataPCA(6:11,1),dataPCA(6:11,2),dataPCA(6:11,3)]', t);
%  Ellipse_plot(A,C);
% xlabel('PCA value');
str=sprintf('Top %d LoCoM features',size(data_r,2));
legend('Progressor','Non-progressor');
title(str);
set(gca,'FontSize',25);
az = 162;
el = 44;
view(az, el);
hold off;


% %% project the top features statistics using PCA into 1D space
% idx_TTest=idx_TTest_epistroma;
% dataw=dataw_epistroma;
% data_r=dataw(:,idx_TTest(1:3));
% 
% coeff = pca(data_r);
% mainCoeff=coeff(:,1);
% dataPCA=data_r*mainCoeff;
% 
% setPro=find(labels==1);
% setNonPro=find(labels==0);
% 
% %%% show in figure
% figure;hold on;
% plot(dataPCA(setPro),ones(1,length(setPro)),'bs','MarkerSize',15);
% plot(dataPCA(setNonPro),ones(1,length(setNonPro)),'rp','MarkerSize',15);
% 
% xlabel('PCA value');
% str=sprintf('Top %d features - PCA',size(data_r,2));
% legend('Progressor','Non-progressor');
% title(str);
% set(gca,'FontSize',20);
% hold off;
% 
% %% project the top features using PCA into 2D space
% coeff = pca(data_r);
% mainCoeff=coeff(:,1:2);
% dataPCA=data_r*mainCoeff;
% 
% %%% show in figure
% figure;hold on;
% plot(dataPCA(setPro,1),dataPCA(setPro,2),'bs','MarkerSize',15);
% plot(dataPCA(setNonPro,1),dataPCA(setNonPro,2),'rp','MarkerSize',15);
% 
% % xlabel('PCA value');
% str=sprintf('Top %d features - PCA',size(data_r,2));
% legend('Progressor','Non-progressor');
% title(str);
% set(gca,'FontSize',20);
% hold off;
% 
% %% project the top features using PCA into 3D space
% coeff = pca(data_r);
% mainCoeff=coeff(:,1:3);
% dataPCA=data_r*mainCoeff;
% 
% %%% show in figure
% figure;hold on;t = 0.001;
% 
% plot3(dataPCA(setPro,1),dataPCA(setPro,2),dataPCA(setPro,3),'ks','MarkerSize',20,'MarkerFace','k');
% % [A , C] = MinVolEllipse([dataPCA(1:5,1),dataPCA(1:5,2),dataPCA(1:5,3)]', t);
% %  Ellipse_plot(A,C);
%  
% plot3(dataPCA(setNonPro,1),dataPCA(setNonPro,2),dataPCA(setNonPro,3),'rp','MarkerSize',20,'MarkerFace','r');
% % [A , C] = MinVolEllipse([dataPCA(6:11,1),dataPCA(6:11,2),dataPCA(6:11,3)]', t);
% %  Ellipse_plot(A,C);
%  
% % xlabel('PCA value');
% str=sprintf('Top %d features - PCA',size(data_r,2));
% legend('Progressor','Non-progressor');
% title(str);
% set(gca,'FontSize',25);
% az = 114;
% el = 70;
% view(az, el);
% hold off;