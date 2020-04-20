function masked = applymask(array, mask, bg)

if nargin<3,
    bg=cast(0,class(array));
end

if size(array,3)==3 && size(mask,3)~=3,
    mask=repmat(mask,[1 1 3]);
elseif size(array,3)~=1
    error('Array and mask sizes do not agree.');
end

masked = array;
masked(mask)=bg;
