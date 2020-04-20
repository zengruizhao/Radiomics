%2D GABOR FILTER BANK CREATION
%For varying the parameters associated with GABOR2DKERNS
%BANDWIDTH (3) = [0.0500,0.2500,0.4500]
%WAVELENGTH (5) = pi./[2*sqrt(2),4,4*sqrt(2),8,8*sqrt(2)]
%ORIENTATION (6) = (0:5)*pi/6
%TOTAL 3*5*8 = 90
%Satish Viswanath, Nov 2009
%
%b=1,l=[5.47,8.2,10.93],xy = (0:7)*pi/8: from Kruizinga et al, "Nonlinear
%Operator for Oriented Texture". 
%b=1,l=[1./sqrt(2)*(2.^(0:6))],xy=(0:3)*pi/4: from Jain et al, "Unsupervised 
%Texture Segmentation Using Gabor Filters", assuming ncol = 256 = 2^6.
%You can use the l_infinity norm to combine for each set of orientations, 
%l_infinity = max(Gab,[],3), where size(Gab,3) = # of Gabor features extracted.
clear;close all;
g=0;
%for b = [0.4,0.7,1.3,1.5]
%for b = 0.1:.05:0.3
b=1; %bandwidth
    %for l = [1,1.4,1.7,2]
    for l=sort([1./sqrt(2)*(2.^(2:6)),8.2]) %lambda = wavelength
    %for l=[1.8:0.43:9]
    %l=1.5;
        %for xy = (0:3)*pi/4
        for xy = (1:7)*pi/8 % orientation, theta
        %xy=0;
            g = g+1;
            [gb_c,gb_s] = gabor2dkerns(xy,l,b,0,1);
            gabor(g).cos = gb_c;
            gabor(g).sin = gb_s;
            gabor(g).title = ['XY Orient=',num2str(xy),...
                         ', Bandwidth=',num2str(b),',Wavelength=',num2str(l)];
            gabor(g).params = [xy,l,b];

            % To view the filter(s)
%             %temp = sqrt(gb_c.^2 + gb_s.^2);
%             temp = abs(fftshift(fftn(gb_c,[256 256])));
%             temp = uint8(255*rescale(temp));            
%             image(temp);colormap gray; 
%             title(['XY Orient=',num2str(xy),...
%                     ', Bandwidth=',num2str(b),', Wavelength=',num2str(l)]);
%             pause;            
        end
    end
%end
norient = length([(0:7)*pi/8]);

% saving
% save gaborbank2d.mat gabor norient
        