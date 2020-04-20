function [found_scale, found_rot] = logpolreg(I,Idef)
% LOGPOLREG Register two images via log-polar transformation of the images.
%    [FOUND_SCALE, FOUND_ROT] = LOGPOLREG(I,IDEF) determines a scale factor
%    FOUND_SCALE and rotation FOUND_ROT that when applied to image IDEF 
%    brings it into best alignment with image I.
%
%    Example:
%        app_rot=deg2rad(10), app_scale=3
%        Irotscale=imdef([app_rot 0 0 app_scale app_scale],I,'linear');
%        % above or whatever function to rotate and scale I
%        [found_scale, found_rot]=logpolreg(I, Irotscale);
%
%Jonathan Chappelow
%Rutgers University

[Ipol] = imcart2logpol(I,'linear',0);
[Ipoldef,rrange,thrange] = imcart2logpol(Idef,'linear',0);

xc2=xcorr2(Ipol,Ipoldef);
[i,j]=find(xc2==max(xc2(:)));

dscale=size(Ipol,1)-i;
drot=size(Ipol,2)-j;

found_scale=rrange(end)/rrange(end-dscale);
found_rot=drot*(thrange(1)-thrange(2));
