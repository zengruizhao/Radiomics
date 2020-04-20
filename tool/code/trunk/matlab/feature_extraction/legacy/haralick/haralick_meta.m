function feats = haralick_meta(img,graylevels,dist)

%function feats = haralick_meta(img,graylevels,dist)
%For calculation of Haralick features within a metavoxel i.e. entire
%metavoxel forms a single window within which features are calculated
%Returns 13 Haralick features 
%Uses MEX file graycomtx_3d for co-occurence matrix calculations
%James Monaco
% Options: 
% graylevels : 64,128,256
% dist: 1,2
% IMG needs to be uint16.

ttime=cputime;
names = {'contrast_energy','contrast_inverse_moment', ...
    'contrast_ave','contrast_var','contrast_entropy', ... 
    'intensity_ave','intensity_variance','intensity_entropy', ...
    'entropy,','energy', ...
    'correlation', ...
    'information_measure1','information_measure2'};

if ~isa(img,'uint16')
    error('Data must be uint16');
end

if any(img(:) < 0 | img(:) > graylevels)
    error('Data values fall outside graylevels.');
end

% Initialize output
feats = zeros(length(names),1);

% For every pixel build a window, GLCM, and calculate features
% comtx_operator = ones(2*dist+1);
% comtx_operator(dist+1,dist+1) = 0;
foreground = true(size(img));

if ndims(img) ==3 
    error('3D not yet supported');
else
    s=1;
end

SGLD = graycomtx_3d(img,graylevels,dist,foreground);

%Display probability matrix
%dispimg(SGLD);
SGLD = double(SGLD);

%%% Calculate Statistics %%%%
[pi,pj,p] = find(SGLD>(eps^2));  
if length(p) <= 1    
    return;
end
p = p/sum(p); pi = pi-1; pj = pj-1;

%marginal of x
px_all = sum(SGLD,2);                                       
[pxi,junk,px] = find(px_all>(eps^2));
px = px/sum(px); pxi = pxi-1;
% if length(px) <=3
%     continue;
% end             

%marginal of y
py_all = sum(SGLD,1)';                                       
[pyi,junk,py] = find(py_all>(eps^2)); 
py = py/sum(py); pyi = pyi-1;


%%% Calculate Contrast Features %%%%
all_contrast = abs(pi-pj);
[sorted_contrast,sind] = sort(all_contrast);
ind = [find(diff(sorted_contrast)); length(all_contrast)];
contrast = sorted_contrast(ind);
pcontrast = cumsum(p(sind));
pcontrast = diff([0; pcontrast(ind)]);

contrast_energy = sum( contrast.^2 .* pcontrast );                  
contrast_inverse_moment = sum( (1./(1+contrast.^2)) .* pcontrast );      
contrast_ave = sum( contrast .* pcontrast ); 
contrast_var = sum( (contrast - contrast_ave).^2 .* pcontrast );
contrast_entropy = -sum( pcontrast.*log(pcontrast) );

%%% Calculate Intensity Features %%%%
all_intensity = (pi+pj)/2;
[sorted_intensity,sind] = sort(all_intensity);
ind = [find(diff(sorted_intensity)); length(all_intensity)];
intensity = sorted_intensity(ind);
pintensity = cumsum(p(sind));
pintensity = diff([0; pintensity(ind)]);

intensity_ave = sum( intensity .* pintensity );                     
intensity_variance = sum( (intensity-intensity_ave).^2 .* pintensity ); 
intensity_entropy = -sum( pintensity.*log(pintensity) ); 

%%% Calculate Probability Features %%%%
entropy = -sum( p.*log(p) );                     
energy = sum( p.*p );                     

%%% Calculate Correlation Features %%%%
mu_x = sum(pxi.*px);                                 
sigma_x = sqrt(sum( (pxi-mu_x).^2 .* px ));            
mu_y = sum(pyi.*py);                                  
sigma_y = sqrt(sum( (pyi-mu_y).^2 .* py));

if sigma_x==0 || sigma_y==0
    warning('Zero standard deviation.');
else
    correlation = sum( (pi-mu_x).*(pj-mu_y).* p ) / (sigma_x*sigma_y); 
end

%%% Calculate Information Features %%%%    
[px_grid,py_grid] = meshgrid(px,py);
[log_px_grid,log_py_grid] = meshgrid(log(px),log(py)); 
h1 = -sum( p .* log(px_all(pj+1).*py_all(pi+1)) );
h2 = -sum( px_grid(:).*py_grid(:).*(log_px_grid(:)+log_py_grid(:)) );  
hx = -sum( px.*log(px) );
hy = -sum( py.*log(py) );

information_measure1 = (entropy-h1)/max(hx,hy);
information_measure2 = sqrt(1-exp(-2*(h2-entropy)));

for k = 1:length(names)
    feats(k) = eval(names{k});
end
             
if any(isinf(feats(:))) || any(isnan(feats(:))) || any(~isreal(feats(:)))
    ninfs=sum(isinf(feats(:)));
    nnans=sum(isnan(feats(:)));
    fprintf('There are some Infs (%d) or NaNs (%d) in the feature image.\n',ninfs,nnans)
end

feats = shiftdim(feats,1); 
if cputime-ttime>1, fprintf('\nTime taken: %d\n',cputime-ttime); end

end