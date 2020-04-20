function [dominant_orientation] = calculate_orientation(pixel_start_r,pixel_start_c,k,Fx_o,Fy_o)
%[dominant_orientation] = calculate_orientation(i,j,k,Fx_o,Fy_o)

%[I_gradient_inner, I_gradient_inner_mag] = calculate_orientation(i,j,k,Fx_o,Fy_o)

G_x=[];G_y=[];

% optimizing G_x G_y calculation
G_x = Fx_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
G_y = Fy_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);

G=[G_x(:) G_y(:)];
[~,~, V]=svd(G);%PCA
dominant_orientation=atan2(V(1,1),V(1,2)); %Find dominant direction
%dominant_magnitude=sqrt(V(1,1)^2+V(1,2)^2); % not required

%I1,I2 = [atan2(V(1,1),V(1,2)); sqrt(V(1,1)^2+V(1,2)^2)];


