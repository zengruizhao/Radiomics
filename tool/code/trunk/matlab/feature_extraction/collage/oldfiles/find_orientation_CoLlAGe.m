function [ I_gradient_inner, I_gradient_inner_mag, dominant_orientation_roi_optimized ]=find_orientation_CoLlAGe(Fx_o,Fy_o,k,r,c)

I_gradient_inner=atan2(Fy_o,Fx_o);
S_inner=size(I_gradient_inner);
I_gradient_inner_mag=zeros(S_inner(1),S_inner(2));

% optimized George Lee 9/24/2015
tic
for i= k+1:k+r
    for j= k+1:k+c       
        pixel_start_r=i;
        pixel_start_c=j;
        G_x=[];G_y=[];
        
        % optimizing G_x G_y calculation
        G_x = Fx_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
        G_y = Fy_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
             
       G=[G_x(:) G_y(:)];
       [~,~, V]=svd(G);%PCA
       dominant_orientation=atan2(V(1,1),V(1,2)); %Find dominant direction
       dominant_magnitude=sqrt(V(1,1)^2+V(1,2)^2); % not required
       I_gradient_inner(i,j)=dominant_orientation;
       I_gradient_inner_mag(i,j)=dominant_magnitude; % not required
    end
end

dominant_orientation_roi_optimized= I_gradient_inner(k+1:k+r,k+1:k+c);
toc

% if you just want the first output, use this
%I_gradient_inner = bsxfun(@(A,B) calculate_orientation(A,B,k,Fx_o,Fy_o), k+1:k+r,k+1:k+c);
dominant_orientation_roi_optimized= I_gradient_inner(k+1:k+r,k+1:k+c);

% 
% function [I_gradient_inner, I_gradient_inner_mag] = calculate_orientation(r,c)
% 
%     pixel_start_r=i;
%     pixel_start_c=j;
%     G_x=[];G_y=[];
% 
%     % optimizing G_x G_y calculation
%     G_x = Fx_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
%     G_y = Fy_o(pixel_start_r-k:pixel_start_r+k, pixel_start_c-k:pixel_start_c+k);
% 
%     G=[G_x(:) G_y(:)];
%     [~,~, V]=svd(G);%PCA
%     dominant_orientation=atan2(V(1,1),V(1,2)); %Find dominant direction
%     dominant_magnitude=sqrt(V(1,1)^2+V(1,2)^2); % not required
%     I_gradient_inner(i,j)=dominant_orientation;
%     I_gradient_inner_mag(i,j)=dominant_magnitude; % not required
% 


%Prateek 7/1/2015

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

end

