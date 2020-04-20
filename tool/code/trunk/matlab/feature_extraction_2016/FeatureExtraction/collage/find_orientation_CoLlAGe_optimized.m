function [ dominant_orientation_roi ]=find_orientation_CoLlAGe_optimized(Fx_o,Fy_o,k,r,c)

I_gradient_inner=atan2(Fy_o,Fx_o);% caculate degree, just to initialize gradient matrix.

% optimized George Lee 9/24/2015
for pixel_start_r= k+1:k+r
    for pixel_start_c= k+1:k+c
        G_x = Fx_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
        G_y = Fy_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
        G=[G_x(:) G_y(:)];
        [~,~, V]=svd(G);%PCA
        I_gradient_inner(pixel_start_r,pixel_start_c)=atan2(V(1,1),V(1,2)); %calculating dominant direction
    end
end

dominant_orientation_roi= I_gradient_inner(k+1:k+r,k+1:k+c);


%% not really quite able to get this one working
% rows = k+1:k+r;
% cols = k+1:k+c;
% fun = @(A,B) calculate_orientation(A,B,k,Fx_o,Fy_o);
% I_gradient_inner2 = bsxfun(fun,rows,cols);
% dominant_orientation_roi_optimized2= I_gradient_inner2(k+1:k+r,k+1:k+c);


%% single for-loop...not really any faster
%
% I_gradient_inner2=atan2(Fy_o,Fx_o);
% [p,q] = meshgrid(rows,cols);
% pairs = [p(:) q(:)];
% subs = sub2ind( size(Fx_o), pairs);
%
% tic
% for i = 1:size(subs,1)
%     I_gradient_inner2(subs(i,1),subs(i,2)) = calculate_orientation(subs(i,1),subs(i,2),k,Fx_o,Fy_o);
% end
% toc
% dominant_orientation_roi_optimized2= I_gradient_inner2(k+1:k+r,k+1:k+c);


%% Prateek 7/1/2015
%I_gradient_inner=atan2(Fy_o,Fx_o);
%S_inner=size(I_gradient_inner);
%I_gradient_inner_mag=zeros(S_inner(1),S_inner(2));

% tic
% for i= k+1:k+r
%     for j= k+1:k+c
%         pixel_start_r=i;
%         pixel_start_c=j;
%         G_x=[];G_y=[];
%
%         for a= pixel_start_r-k:pixel_start_r+k
%             for b= pixel_start_c-k:pixel_start_c+k
%
%                 G_x=[G_x;Fx_o(a,b)];
%                 G_y=[G_y;Fy_o(a,b)];
%
%             end
%         end
%
%        G=[G_x G_y];
%        [U S V]=svd(G);%PCA
%        dominant_orientation=atan2(V(1,1),V(1,2)); %Find dominant direction
%        dominant_magnitude=sqrt(V(1,1)^2+V(1,2)^2); % not required
%        I_gradient_inner(i,j)=dominant_orientation;
%        I_gradient_inner_mag(i,j)=dominant_magnitude; % not required
%     end
% end
%
% dominant_orientation_roi= I_gradient_inner(k+1:k+r,k+1:k+c);
% toc


