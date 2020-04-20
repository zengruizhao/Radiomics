function FirstOrder_features = extract_firstOrder(Image, mask)
    temp = Image(mask==1);
    FirstOrder_features(1) = median(temp);
    FirstOrder_features(2) = mean(temp);
    FirstOrder_features(3) = max(temp);
    FirstOrder_features(4) = std(temp);
    FirstOrder_features(5) = skewness(temp);
    FirstOrder_features(6) = kurtosis(temp);
    FirstOrder_features(7) = max(temp) - min(temp);
    FirstOrder_features(8) = mean(temp.^2);