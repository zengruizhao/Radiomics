function [feats] = morph_features(x,y)
% Function to extract a set of morphological features.
% Input: 
% [x,y]         Input coordinates of the boundary points. 
%
% Output:
% feats         Feature vector.
% 
% The features are arranged in the following manner:


% Order the coordinates
xy = order([x,y]');
% Get the centroid and area
[xc,yc,area]=centroid(xy);
x = xy(:,1);
y = xy(:,2);

% % Plot the polygons and the centroids
% figure; 
% scatter(x,-y,'r'); 
% hold on; 
% plot(xc,-yc,'o');


distance=sqrt((x-xc).^2+(y-yc).^2);

dist_min=min(distance);
dist_max=max(distance);

% Draw circle from centroid to max radius
theta = 0:.01:2*pi;
x_c1 = zeros(1,length(theta));
y_c1 = zeros(1,length(theta));
for i=1:length(theta)
    x_c1(i)=dist_max*cos(theta(i))+xc;
    y_c1(i)=-dist_max*sin(theta(i))-yc;
end
% hold on; plot(x_c1,y_c1,'k');

% Draw circle from centroid to min radius
theta = 0:.01:2*pi;
x_c2 = zeros(1,length(theta));
y_c2 = zeros(1,length(theta));
for i=1:length(theta)
    x_c2(i)=dist_min*cos(theta(i))+xc;
    y_c2(i)=dist_min*sin(theta(i))+yc;
end
% hold on; plot(x_c2,y_c2,'k')


%==========================================================================
% Get variance and standard deviation
var     = cov(distance);
stdv    = std(distance);

%==========================================================================
%get maximum area and Area Ratio
max_area    = pi*dist_max^2;
Area_Ratio  = area/max_area;

%==========================================================================
%Ratio between average distance and maximum distance

dist_mean   = mean(distance);
Dist_Ratio  = dist_mean/dist_max;
%==========================================================================
%normalizing distance to find the variance and std

dists       = distance/dist_max;
dists_std   = std(dists);
dists_var   = cov(dists);

%==========================================================================
%new distance ratio defined
dratio      = distratio(xy);
%==========================================================================
%area to perimeter ratio
[paratio,peri]  = periarea(xy,area);

%==========================================================================
%the SMOOTHNESS METRIC  
D = distance;
s = length(D);
sm = zeros(1,length(D));

for i=1:s;
    if i==1;
        sm(i) = abs(D(i)-((D(i+1)+D(s))/2));
    else if i==s;
            sm(i) = abs(D(i)-((D(1)+D(i-1))/2));
        else if i>=2 && i<=s-1
                sm(i) = abs(D(i)-((D(i+1)+D(i-1))/2));
            end
        end
    end
end
smooth = sum(sm);


%==========================================================================
% Fourier Descriptors of boundary

z   = frdescp([x,y]);
% dim = size(z,1);
% z = z(dim/2-n/2+1:dim/2+n/2);
fd  = z(1:10);

%============
% Invariant moments
B   = bound2im([x,y]);
phi = invmoments(B);

%===========================
% Get the fractal dimension

frac_dim    = fractal_dim([x,y]);

%==========================================
% Para are all the features that are extracted.   

feats = [Area_Ratio;
    Dist_Ratio;
    dists_std;
    dists_var;
    dratio;
    paratio;
    smooth;
    phi';
    frac_dim;
    fd];

%para=[Area_Ratio;Dist_Ratio;dists_std;dists_var];

% fid = fopen(name,'w');
%         fprintf(fid,'%15.10f\n',para);
%         fprintf(fid,'%15.10f \t %15.10f\n',[x,y]);
% fclose(fid);


%to get all the features for glands and get the mean automatically.
% files=['g1.xls';'g2.xls'; 'g3.xls'; 'g4.xls'];
% for i=1:size(files,1);
%  fid = fopen(files(i,:));
%  feature(i,:) = fscanf(fid,'%g',[1 115]); %  
%  fclose(fid)
% end
% feature=feature';
% mean_feature=mean(feature,2);
% mean_feature=mean_feature';
% save mean_feature mean_feature

 % % old one
% %CAVE =[0.4999;0.7178;0.1419;0.0211];
% CAVE = [0.47542;0.70094; 0.14158; 0.021017; 0.76584];
% 
% % old one
% %BAVE =[0.2308;0.5183;0.1984;0.0450];
% BAVE = [0.27297;0.58401; 0.20965; 0.045246; 0.63293];
% 
% n1=norm(para-CAVE);
% n2=norm(para-BAVE);
% 
% if n1 < n2
%     disp('============================================')
%     disp('===============Sorry, Cancerous=============')
%     disp('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
% else if n2 < n1
%         disp('========================================')
%         disp('=================Benign=================')
%         disp('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
%     end
% end




