function [Itargbound,edgei]=imboundim(Itemplate,Itarget,tempthresh,dimshift)
% IMBOUNDIM
%   EDGEIM=IMBOUNDIM(ITEMP,ITARG,TEMPTHRESH,COLORSHIFT)
%   Given a template image ITEMP and a target image ITARG, find the largest
%   contiguous edgte on ITEMP and draw it on ITARG.  Both ITEMP and ITARG
%   must be grayscale, and ITARG must be 8-bit (0 to 255).
%
%   The edge on ITEMP is found by thresholding followed by morphological
%   closing, where ITEMP>TEMPTHRESH is used for rough segmentation.
%
%   Optionally, COLORSHIFT can be used to specify green (0), blue (1), or
%   red (2).  Default is green.
%
%   Make your ITARG 8-bit integer (0 to 255) or this will not work!
%
%JC

% Default color is green
if nargin<4, dimshift=0; end

% Rough segmentation of Itemplate
Itempthresh=Itemplate>tempthresh;

% Morphological closing
% threshclosed=myimclose(Itempthresh,ones(5));
threshclosed=imclose(Itempthresh,strel(ones(5)));

% Calculate edges
[dx,dy]=gradient(double(threshclosed));
edgei=sqrt(dx.^2+dy.^2);
edgei=edgei~=0;
edgei=bwlabel(edgei);

% Find largest contiguous edge
[thecount,countwhat]=count(edgei(edgei>0));
[thecount,countorder]=sort(thecount,'descend');
countwhat=countwhat(countorder);
edgei=edgei==countwhat(1);

% Add boundaries (edgei) to Itarget
maxintensity=max(Itarget(:));
if size(Itarget,3)==3,
    Itargbound=Itarget;
    
    IboundC=Itarget(:,:,2);
    IboundC(edgei)=maxintensity;   % default to green
    Itargbound(:,:,2)=IboundC;
    
    IboundC=Itarget(:,:,1);
    IboundC(edgei)=0;
    Itargbound(:,:,1)=IboundC;
    
    IboundC=Itarget(:,:,3);
    IboundC(edgei)=0;
    Itargbound(:,:,3)=IboundC;
else
    IregdLIboundG=Itarget;
    IregdLIboundG(edgei)=maxintensity;   % default to green
    IregdLIboundRB=Itarget;
    IregdLIboundRB(edgei)=0;    % red and blue planes
    Itargbound=cat(3,IregdLIboundRB,IregdLIboundG,IregdLIboundRB); % assemble
end

Itargbound=circshift(Itargbound,[0 0 dimshift]);    % apply COLORSHIFT
Itargbound=rescale(Itargbound);
