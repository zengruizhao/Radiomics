%3D GABOR FILTER BANK CREATION
%For varying the parameters associated with GABOR3DKERNS
%BANDWIDTH (4) = [0.1,0.4,0.7,1]
%WAVELENGTH (4) = [1.1,1.4,1.7,2]
%ORIENTATIONS (6 x 2 sets) = [0,pi/6,pi/3,pi/2,2pi/3,5pi/6]
%TOTAL 4*4*6*6 = 576
%Satish Viswanath, Jul 2008
%Updated Gabor params, Mar 2013
%Based off parameters from GABORBANK2D

g=0;
%for b = 0.1:.3:1.2
%b=0.5;
b=1;
    for l=sort([1./sqrt(2)*(2.^(1:4))]) %lambda = wavelength
    %for l = 1.1:0.3:2                       
    %l=1.5;
        for xy = (0:5)*pi/6
        %xy=0;
            for xz = (0:3)*pi/4
            %xz=0;
                g = g+1;
                [gb_c,gb_s] = gabor3dkerns(xy,xz,l,b);
                gabor(g).cos = gb_c;
                gabor(g).sin = gb_s;
                gabor(g).title = ['XY Orient=',num2str(xy),', XZ Orient=',num2str(xz),...
                             ', Bandwidth=',num2str(b),', Wavelength=',num2str(l)];
                gabor(g).params = [xy,xz,l,b];
                
                % To view the filter(s)
%                 g = abs(fftshift(fftn(gb_c,[100 100 100])));
%                 g1 = uint8(255*rescale(g));
%                 for i=1:100
%                     image(g1(:,:,i));colormap gray; 
%                     title(['XY Orient=',num2str(xy),', XZ Orient=',num2str(xz),...
%                             ', Bandwidth=',num2str(b),', Wavelength=',num2str(l)]);
%                     pause;
%                 end
            end
        end
    end
%end

n_xyorient = length([(0:5)*pi/6]);
n_xzorient = length([(0:3)*pi/4]);
%save gaborbank3d.mat gabor -v7.3 %note: will be a huge file!
        