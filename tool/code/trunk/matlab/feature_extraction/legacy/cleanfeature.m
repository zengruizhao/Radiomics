function cleanfeat = cleanfeature(feature,N,exclude,noround)
%cleanfeat = cleanfeature(feature,N,exclude)
% remove NaNs & +/-Infs, rescale to 0 to N-1
%
% you can input entire feature or just data from one class, whatever you want
%
% logical mask EXCLUDE indicates foreground with 1

if nargin<4,
    noround=false;
end
if nargin<3 || isempty(exclude),
    exclude=Inf;
end

if islogical(exclude), % got logical mask
    includemask=exclude; % assume mask indicates foreground with 1
end
if ~isreal(feature), keyboard;end
cleanfeat=zeros(size(feature));
% clean feature of infs and nans, rescale from 0 to N-1
for j=1:size(feature,3),        % if several slices
    for i=1:size(feature,4),    % if several features
        cfeature=feature(:,:,j,i);
        
        % background mask - 1 is foreground, 0 background
        if ~islogical(exclude), % exclude is a value, not a mask
            includemask=cfeature~=exclude;
        end
        
        % set background to 0
        cfeature(~includemask)=0;
        
        % remove infs and nans from entire image
        badpoints=isinf(cfeature) | isnan(cfeature);
        
        % replace with min real value (why not?)
        smallestval=min(cfeature(includemask));
        if isnan(smallestval) || isinf(smallestval), smallestval=0; end
        cfeature(includemask & badpoints)=smallestval;
        
        % or background
        cfeature(~includemask & badpoints)=0;
        
        % was (could be) simply:
        % cfeature(isinf(cfeature) | isnan(cfeature))=0;
        
        % rescale non-background points to integers valued 0 to N-1
        if ~noround,
            cfeature(includemask)=round((N-1)*rescale(cfeature(includemask)));
        else
            cfeature(includemask)=(N-1)*rescale(cfeature(includemask));
        end
        % cfeature=round((N-1)*rescale(cfeature));
        
        cleanfeat(:,:,j,i)=cfeature;
    end
end
