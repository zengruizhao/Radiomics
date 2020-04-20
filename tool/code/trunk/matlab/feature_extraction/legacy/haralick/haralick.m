function feats = haralick(img,fg_bg_mask,graylevels,hws,dist,fg_bg)

%function feats = haralick(img,fg_bg_mask,graylevels,hws,dist,fg_bg)
%fg_bg_mask is a mask of the foreground as ones (fg_bg =1) or background as ones (fg_bg =0)
%Returns 13 Haralick features in a structure containing names and features
%Uses MEX file graycomtx_3d for co-occurence matrix calculations
%James Monaco
% FEATS is a struct: img3 are the features, names are the names
% Options: 
% graylevels : 64,128,256
% hws :1,2,3
% dist: 1,2
% IMG needs to be uint16.


ttime=tic;
feats.names = {'contrast_energy','contrast_inverse_moment', ...
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
[nrows,ncols,nsl] = size(img);
feats.img3 = zeros(length(feats.names),nrows,ncols);

tic;
% For every pixel build a window, GLCM, and calculate features
% comtx_operator = ones(2*dist+1);
% comtx_operator(dist+1,dist+1) = 0;
if fg_bg == 1 
	foreground = fg_bg_mask;
elseif fg_bg ==0
    background = fg_bg_mask;
    if ~islogical(background) || numel(background)==1,
        background=img==background;
    end
    foreground=~background;
else 
	error('fg_bg can only be 1 or 0.');
end

if ndims(img) ==3 
    s=2;
else
    s=1;
end
% z coords of window, removing z coords outside of image
zinds=s-1:s+1;
zinds(zinds<1 | zinds>nsl)=[];    

    for i = 1:nrows
        % y coords of window, removing y coords outside of image
        yinds=i-hws:i+hws;
        yinds(yinds<1 | yinds>nrows)=[];

        for j = 1:ncols
            if foreground(i,j,s)    % don't bother for pixels in background
	            % x coords of window, removing x coords outside of image
	            xinds=j-hws:j+hws;
	            xinds(xinds<1 | xinds>ncols)=[];

	            % get window
	            win = img(yinds,xinds,zinds);  
				fgwin=foreground(yinds,xinds,zinds);
	            
                SGLD = graycomtx_3d(win,graylevels,dist,fgwin);

	            %Display probability matrix
	            %dispimg(SGLD);
	            SGLD = double(SGLD);

	            %%% Calculate Statistics %%%%
	            [pi,pj,p] = find(SGLD>(eps^2));  
	            if length(p) <= 1
	                continue;
	            end
	            p = p/sum(p); pi = pi-1; pj = pj-1;

	            %marginal of x
	            px_all = sum(SGLD,2);                                       
	            [pxi,junk,px] = find(px_all>(eps^2));
	            px = px/sum(px); pxi = pxi-1;
% 	            if length(px) <=3
% 	                continue;
% 	            end             

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

	            for k = 1:length(feats.names)
	                feats.img3(k,i,j) = eval(feats.names{k});
	            end

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
    end
                
if any(isinf(feats.img3(:))) || any(isnan(feats.img3(:))) || any(~isreal(feats.img3(:)))
    ninfs=sum(isinf(feats.img3(:)));
    nnans=sum(isnan(feats.img3(:)));
    fprintf('There are some Infs (%d) or NaNs (%d) in the feature image.\n',ninfs,nnans)
end

feats.img3 = shiftdim(feats.img3,1); 
fprintf('  Time: %d\n',toc(ttime));