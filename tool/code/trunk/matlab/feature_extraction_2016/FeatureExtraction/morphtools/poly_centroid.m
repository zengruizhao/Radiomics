function [centroid_r,centroid_c,area] = poly_centroid(bound_r,bound_c)

mr = mean(bound_r);
mc = mean(bound_c);
bound_r = bound_r - mr;
bound_c = bound_c - mc;

cross_prods = bound_r.*bound_c([2:end 1]) - bound_r([2:end 1]).*bound_c;

area = 1/2 * sum(cross_prods); 

centroid_r = sum((bound_r + bound_r([2:end 1])) .* cross_prods) / (6*area);
centroid_c = sum((bound_c + bound_c([2:end 1])) .* cross_prods) / (6*area);

centroid_r = centroid_r + mr;
centroid_c = centroid_c + mc;

area = abs(area);
