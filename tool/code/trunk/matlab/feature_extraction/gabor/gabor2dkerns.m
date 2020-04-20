function [gb_c,gb_s]=gabor2dkerns(theta,lambda,b,psi,gamma)
% GABOR	2DKERNS Generate 2D Gabor kernels.
%   [GB_C,GB_S]=GABOR2DKERNS(THETA,LAMBDA,B,PSI,GAMMA) calculates the cosine
%   and sine (pi/2 phase difference) Gabor kernels for the specified
%   parameter values.
%
%   THETA = Orientation, LAMBDA = Wavelength
%   B = Bandwidth (set B=1 to reduce params, meaning SIGMA=0.56*LAMBDA)
%   PSI = Phase shift (usually 0)
%   GAMMA = Anistropic scaling factor in Y (for isotropy, GAMMA = 1)
%
%	Note that one typically COMBINES the cosine and sine responses 
%	to get a single "magnitude" Gabor response.
%
%Satish Viswanath, Jul 2008; Jon Chappelow Jan 2010
    
% Sigma (scale) is a function of: (1) bandwidth and (2) lambda
if b==1
    sigma_x = 0.56*lambda;
    sigma_y = 0.56*lambda;
else
    sigma = (1/pi)*sqrt(log(2)/2)*((2^b+1)/(2^b-1))*lambda;
    sigma_x = sigma; sigma_y = sigma; % isotropic
end

% Window size is based on sigma
sz_x=ceil(7*sigma_x);
if mod(sz_x,2)==0, sz_x=sz_x+1;end % make window size odd
sz_y=ceil(7*sigma_y);
if mod(sz_y,2)==0, sz_y=sz_y+1;end
fprintf('Window Size: %g x %g.\n',sz_y,sz_x);
 
[x, y]=meshgrid(-fix(sz_x/2):fix(sz_x/2),fix(-sz_y/2):fix(sz_y/2));
 
% Rotation by theta
x_theta=x*cos(theta)-y*sin(theta);
y_theta=x*sin(theta)+y*cos(theta);
 
gb_c=exp(-.5*(x_theta.^2/sigma_x^2+gamma^2*y_theta.^2/sigma_y^2)).*cos(2*pi/lambda*x_theta+psi);
gb_s=exp(-.5*(x_theta.^2/sigma_x^2+gamma^2*y_theta.^2/sigma_y^2)).*sin(2*pi/lambda*x_theta+psi);

%gb = sqrt(gb_r.^2+gb_i.^2); % sample combined response

end
