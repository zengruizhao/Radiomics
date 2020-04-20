
addpath(genpath('./NuclearSegmentation/Veta'));
addpath(genpath('./FeatureExtraction'));
addpath(genpath('./CCM_CRL'));

%% step 1: load image
img = imread('test.tif');

%% step 2: segment nuclei and save boundaries
scales = 8:2:14;
[bounds, nuclei, properties] = Veta(img, scales);

%% step 3: extract features
% 1. graph features(51)
[graphfeats] = extract_all_features(bounds,img,1);

% 2. morphological features(100)
[morphfeats] = extract_all_features(bounds, img, 2);

% 3. CGT features(39)
[CGTfeats] = extract_all_features(bounds, img, 3);

% 4. cluster graph features(26)
[clustergraphfeats] = extract_all_features(bounds, img, 4);

% 5. haralick features(26)
[haralickfeats] = extract_all_features(bounds, img, 5);

% 6. texture features (720) (Grayscale(15), Gabor(24), Laws(25), Local Binary Pattern(16) = 80 * (HSV 3channels) * (mean, std, mode))
[texturefeats] = extract_texture_features(img);

% 7. CCM (Locom) features (1*20*13*6=1560)
[CCMfeats, description_CCM] = extract_CCM_features(img, bounds, properties);

% 8. CRL features (24)
[CRLfeats, description_CRL] = extract_CLM_features(img, bounds, properties);

% 9. Hosoya features
[Hosoyafeats, description_Hosoya] = extract_Hosoya_features(bounds);

% 10. collage features
[collagefeats] = extract_collage_features(img);
