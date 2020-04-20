% Internal code to extract features, which calls the appropriate functions.
% 
% Usage: features = fextract(image, feats_to_extract)
% 
% feats_to_extract is a boolean of length 3, indicating whether or not to
% extract grey level, gabor, and haralick features respectively.
% 
% Created by Rob Toth and others 23-Jul-2010.
% 
function features = fextract(image, feats_to_extract)

    [h, w, ~] = size(image);
    
    features = [];
    greyfeats = []; harfeats = []; gabfeats = [];
    
    windows = [5, 15, 35];
    Scale = [1, 7];
    Orient = ((0:4).*pi)./4;

    if feats_to_extract(1)
        greyfeats = greylevel(image, windows);
        greyfeats = single(reshape(greyfeats,h,w,size(greyfeats,3)*size(greyfeats,4)));
    end

    for win = windows
        
        % gabor
        if feats_to_extract(2) && (exist('gabor', 'file') == 2)
            temp = gabor(image,Scale,Orient,win);
            temp = single(reshape(temp,h,w,size(temp,3)*size(temp,4)));
            gabfeats = cat(3, gabfeats, temp);
        end

        % haralick
        if feats_to_extract(3) && (exist('haralick', 'file') == 2)
            temp = haralick(image, win, 256, 0, 1);
            temp = single(reshape(temp,h,w,size(temp,3)*size(temp,4)));
            harfeats = cat(3, harfeats, temp);
        end        

    end % for windowsize
    clear temp

    features = cat(3, image, features, greyfeats, harfeats, gabfeats);

end % function 

