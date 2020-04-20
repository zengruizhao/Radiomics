function feature_concat = extract_collage_features(I)

mask = zeros(size(I, 1), size(I, 2));
mask(101:220, 101:220) = 1;
[feature_concat] = compute_CoLlAGe2D(I, mask, 2);
