function [Haus]=Hausdorff_Dimension(image)
%HAUSDORFF_DIMENSION calcuates object Hausdorff Dimension using the box counting method.
%
%[HAUS]=HAUSDORFF_DIMENSION(IMAGE) returns a number greater than 0.
%IMAGE is a image mask, where points inside the object are nonzero.
%
%HAUSDORFF_DIMENSION is the number that represents the generalize  fractal 
%dimension of a 2D matrix. 
%
%Using the box method this is calculated by  finding the  ratio HAUS=log (P)/log(R).
%P is the number of pixels containing the object and R is the dimension of each pixel.
%By calculating P for R=1->size(IMAGE) we can find HAUS by fitting a line
%to log(P)/log(R) over all P,R.
%
%see Theiler, J. Estimating Fractal Dimension. Journal of the Opical
%Society of America 7(6):1055-1073, 1990. for information on methods to
%estimate HAUS.

%first zero pad the array to get a square matrix
im_size=size(image);
padding=max(im_size)-im_size;
new_image=padarray(edge(image),padding,'post');

%define the step of the grid size as 2.
gridsize=2;
k=2;

%calulate the number of pixels containing the object over various grid
%sizes.
true_box(1)=sum(new_image(:));
    while gridsize<(im_size/2)
                true_box(k)=0;
        for i=1:gridsize:(length(new_image)-gridsize+1)
             for j=1:gridsize:(length(new_image)-gridsize+1)
                    rescale_image=new_image(i:i+(gridsize-1),j:j+(gridsize-1));

                if sum(rescale_image(:))>0
                    true_box(k)=true_box(k)+1;
                end
             end
        end
        gridsize=gridsize*2;    
        k=k+1;
    end
    
    %define the dimension array.
    r=[1 2.^(1:(k-2))];
    
    %find the linear fit
    b=polyfit(log(1./r),log(true_box),1);
    
    %return the slope of the line
    Haus=b(1);
end