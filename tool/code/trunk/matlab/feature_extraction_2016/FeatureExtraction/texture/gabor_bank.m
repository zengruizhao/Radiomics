function [gabor_stats] = gabor_bank(sigma,theta,lambda,psi,gamma,nstds)
% Create gabor-filter banks using the parameters defined by input variables.
%
% Input Variables:
%   I:      Image
%   sigma:  Gaussian envelope standard deviation (0.5)
%   theta:  Orientation of the normal to parallel stripes (0)
%   lambda: Wavelength of the sinusoid (1)
%   psi: 	Phase Offset (0)
%   gamma:  Spatial aspect ratio (1)
%   nstds:  Window size (3)

% Ensure all parameters are vectors (just vectorize matrices)
sigma = sigma(:);
theta = theta(:);
lambda = lambda(:);
psi = psi(:);
gamma = gamma(:);
nstds = nstds(:);

nsigma      = length(sigma);
ntheta      = length(theta);
nlambda     = length(lambda);
npsi        = length(psi);
ngamma      = length(gamma);
nnstds      = length(nstds);

ngabor = 1;

% Sigma
for tsigma = 1:nsigma
    s = sigma(tsigma);
    
    % Theta
    for ttheta = 1:ntheta
        t = theta(ttheta);
        
        % Lambda
        for tlambda = 1:nlambda
            l = lambda(tlambda);
            
            % Psi
            for tpsi = 1:npsi
                p = psi(tpsi);
                
                % Gamma
                for tgamma = 1:ngamma
                    g = gamma(tgamma);
                    
                    % STDS
                    for tnstds = 1:nnstds
                        n = nstds(tnstds);
                        
                        % Create Gabor filters
                        [gbr gbi] = gabor_fn(s,t,l,p,g,n);
                        
                        gabor_stats(ngabor).sigma   = s;
                        gabor_stats(ngabor).theta   = t;
                        gabor_stats(ngabor).lambda  = l;
                        gabor_stats(ngabor).psi     = p;
                        gabor_stats(ngabor).gamma   = g;
                        gabor_stats(ngabor).nstds   = n;
                        gabor_stats(ngabor).gbr     = gbr;
                        gabor_stats(ngabor).gbi     = gbi;
                        
                        ngabor = ngabor + 1;
                    end
                end
            end
        end
    end
end

                            
