function [features,feature_names] = haralick(img,graylevels,sig,dist,is_valid,feature_names,comtx_type,varargin)
% [FEATURES,FEATURE_NAMES] = HARALICK(IMG,GRAYLEVELS,SIG,DIST,IS_VALID,feature_names,COMTX_TYPE)
%   HARALICK computes the Haralick features for the 2-D UINT16 image IMG.
%   The dynamic range of the image is given by the scalar GRAYLEVELS.  
%   All values of IMG must lie in the interval [0:GRAYLEVELS-1]. The 
%   half window size HWS designates the area around each pixel in 
%   which the coocurrence matrix is calculated.  DIST determines 
%   the city-block distance (from the center point) to points
%   that cooccur with the center point.  IS_VALID is a map of valid pixels.
%   FEATURE_NAMES allows a subset of Haralick features to be selected.  The
%   results are returned in FEATURES, a 3-D matrix with dimensions
%   [size(IMG) length(feature_names)].  COMTX_TYPE is either 'square_ring', 
%   'ring', or 'annulus' which produces a gaussian weighted ring.  FEATURE_NAMES 
%   returns the names of the features that were calculated.
%  
%   See also haralick3.

% Prevents numeric problems
MIN_VALUE = 1e-10; 

if isempty(feature_names)
    feature_names = {'contrast_energy','contrast_inverse_moment', ...
        'contrast_ave','contrast_variance','contrast_entropy', ... 
        'intensity_ave','intensity_variance','intensity_entropy', ...
        'entropy','energy', ...
        'correlation', ...
        'information_measure1','information_measure2'};
end

if ~isa(img,'uint16')
    img = uint16(img);
    fprintf('Warning: IMG must be uint16. Converting.\n');
%     error('IMG must be uint16');
end

if any(img(:) < 0 | img(:) >= graylevels)
    error('IMG values fall outside graylevels.');
end

% Initialize output
[nrows,ncols] = size(img);
features = zeros(length(feature_names),nrows,ncols,'single');

tic;
% Coocurrence mask is a square "ring" of ones at radius DIST
if ~exist('comtx_type','var')
    comtx_operator = shape2mask('square_ring',dist,'square_ring');
else
    comtx_operator = shape2mask(comtx_type,dist,varargin{:});
end

comtx_operator(ceil(size(comtx_operator,1)/2),ceil(size(comtx_operator,2)/2)) = 0;

hws = ceil(2.5*sig);
[x,y] = meshgrid(-hws:hws,-hws:hws);
weights = exp(-(x.^2+y.^2)/2/sig^2);

% For every pixel build a window, GLCM, and calculate features
for i = 1:nrows
    % y coords of window, removing y coords outside of image
    yinds=i-hws:i+hws;
    yinds(yinds<1 | yinds>nrows)=[];
    
    for j = 1:ncols
        % x coords of window, removing x coords outside of image
        xinds=j-hws:j+hws;
        xinds(xinds<1 | xinds>ncols)=[];

        % get window
        win = img(yinds,xinds);  
        
        if ~is_valid(i,j), continue; end
        is_valid_win = is_valid(yinds,xinds).*weights(yinds-i+hws+1,xinds-j+hws+1); 
        SGLD = graycomtx(win,comtx_operator,graylevels,is_valid_win);
        SGLD(SGLD < MIN_VALUE) = 0;
        
        %Display probability matrix
        %dispimg(SGLD);

        %%% Calculate Statistics %%%%
        [pi,pj,p] = find(SGLD);  
        if length(p) <= 1
            continue;
        end
        p = p/sum(p); pi = pi-1; pj = pj-1;
        
        %marginal of x
        px_all = sum(SGLD,2);                                       
        [pxi,junk,px] = find(px_all);
        px = px/sum(px); pxi = pxi-1;
        
        %marginal of y
        py_all = sum(SGLD,1)';                                       
        [pyi,junk,py] = find(py_all); 
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
        contrast_variance = sum( (contrast - contrast_ave).^2 .* pcontrast );
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
        
        for k = 1:length(feature_names)
            features(k,i,j) = single(eval(feature_names{k}));
        end
        
            
        if any(isnan(features(:,i,j)))
            here = 1;
        end
    end

    
    if mod(i,5)==0, % print a dot for every row. change number to skip rows
        dt=toc;
        if dt<0.5,
            fprintf('-');
        else
            fprintf('.');pause(0.04);    % to facilitate a break
        end
        tic
    end
    milestones=round(linspace(1,nrows,11));
    milestone_percents=0:10:100;
    if any(milestones==i),
        fprintf('%d%%',milestone_percents(milestones==i));pause(0.04);
    end
    if i == milestones(end), fprintf('\n'); end    
end
     
disp('');

if any(isinf(features(:))) || any(isnan(features(:))) || any(~isreal(features(:)))
    ninfs = sum(isinf(features(:)));
    nnans = sum(isnan(features(:)));
    fprintf('There are some Infs (%d) or NaNs (%d) in the feature image.\n',ninfs,nnans)
end

features = shiftdim(features,1); 
