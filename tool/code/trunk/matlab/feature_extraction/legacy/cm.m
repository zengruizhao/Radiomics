function glcm = graycomat(block,graylevels,background,d,bgselfonly)
% GLCM = graycomat(BLOCK,N,BGL,D)
%
% Generate 2D co-occurence matrix for a block of image data.  2D ONLY.
%
% GLCM is always N-by-N.  You decide N, but data MUST MUST MUST be scaled
% in that range first.  This function does not and will not do that.  Too
% many gratuitous assumptions about the data must be made.
%
% Can specify background intensity BGL if you desire to exclude 
% co-occurences with this level from the GLCM. Use [] if unsure.
%
% JC

% distance
if nargin<5,
    bgselfonly=0;
    if nargin<4,
        if nargin<3,
            background=[];
        end
        d=1;
    end
end
% theta is all.  No assumptions about orientation.

[nrows,ncols,planes]=size(block);
if planes>1, error('Not for 3D volumes');end

glcm=zeros(graylevels,graylevels);
block=block+1;

% Begin centered on zero (just off the edge)
% indexes of adjacent pixels (mesh grid):
ineighborinds0=repmat((-d:d)',1,2*d+1);ineighborinds0=ineighborinds0(:);
jneighborinds0=repmat((-d:d), 2*d+1,1);jneighborinds0=jneighborinds0(:);
center=ineighborinds0==0 & jneighborinds0==0;
ineighborinds0(center)=[];jneighborinds0(center)=[];
for i=1:nrows,
    % move down
    ineighborinds=ineighborinds0+i;
    for j=1:ncols,
        jneighborinds=jneighborinds0+j;
        % coordinates of current neighbors
        %neighborinds=[ineighborinds jneighborinds];

        % remove pixels outside image (window)
        realneighbors=ineighborinds>0 & jneighborinds>0 & ineighborinds<=nrows & jneighborinds<=ncols;
        % neighborinds(any(neighborinds<=0,2) | neighborinds(:,1)>nrows | neighborinds(:,2)>ncols,:)=[];

        if isempty(background) || (block(i,j)-1)~=background,
            % count occurences of same neighboring intensities
            %neighbors=sub2ind([nrows ncols],neighborinds(:,1),neighborinds(:,2));
            %x=double(sort(block(neighbors)));
            %thecount=diff(find([1; diff(x); 1]));
            %counted=x(diff([x; max(x)+1]));
            % glcm(single row, multiple columns)
            %glcm(block(i,j)+1,counted+1)=glcm(block(i,j)+1,counted+1)+thecount';
            realneighborinds=find(realneighbors)';
            for ni=realneighborinds,%1:size(neighborinds,2),
                %glcm(block(i,j),block(neighborinds(ni,1),neighborinds(ni,2))) = ...
                %glcm(block(i,j),block(neighborinds(ni,1),neighborinds(ni,2))) + 1;
                glcm(block(i,j), block(ineighborinds(ni),jneighborinds(ni))) = ...
                    glcm(block(i,j), block(ineighborinds(ni),jneighborinds(ni))) + 1;
            end
        end
    end
end

% Remove background cooccurence.
if bgselfonly, %only remove background self-co-occurence
    glcm(background+1,background+1)=0;
else % remove background co-occurence with any other gray level
    % Avoids poisoning the GLCM with informatioon about the boundary.
    glcm(background+1,:)=0;
    glcm(:,background+1)=0;
end