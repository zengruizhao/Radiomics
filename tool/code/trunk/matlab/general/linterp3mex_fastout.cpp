/* 3D linear interpolation
 * based on code by Andriy Myronenko, myron@csee.ogi.edu
 *
 * DI=mirt3D_mexinterp(D,XI,YI,ZI,[BG]) interpolates the 3D data at XI,YI,ZI.
 * D is defined on a regular grid 1:size(D,1), 1:size(D,2), 1:size(D,3).
 * Points outside the boundary are set to BG (optional), or NaN if not specified.
 * Using MATLAB, this is: DI = interp3(D,XI,YI,ZI,'linear',BG)
 *
 * D can be a stack of many 3D volumes (4D).
 */

#include <math.h>
#include "mex.h"

void linterp(double* D, double* X, double* Y, double* Z, double* I,
        size_t ninterps, size_t nrows, size_t ncols, size_t npages, size_t nvols, double bgval) {
    
    // unsigned long long int (__int64) for the array indexes:
    size_t n, i, volskip, nrc, nrcp;
    // signed long long ints for the relative indexes:
    long long ndx, fx, fy, fz;
    long long in1, in2, in3, in4, in5, in6, in7, in8;
    // doubles for the interpolated data, fractional coefficients, and potentially oob locations
    double x, y, z, x1, z1, tmp;
    double w1, w2, w3, w4, w5, w6, w7, w8;
    
    nrc=nrows*ncols;
    nrcp=nrc*npages;
    
    for (n=0; n<ninterps; n++) {
        
        x=X[n];
        y=Y[n];
        z=Z[n];
        
        fx=(size_t) floor(x);
        fy=(size_t) floor(y);
        fz=(size_t) floor(z);
        
        if (fx<1 || x>ncols || fy<1 || y>nrows || fz<1 || z>npages) {
            // Set bgval when outside volume
            for (i=0; i<nvols; i++) I[n+i*ninterps]=bgval;
        } else  {
            ndx =  fy + (fx-1)*nrows + (fz-1)*nrc;
            
            if (x==ncols) {
                x=x+1; ndx=ndx-nrows;
            }
            if (y==nrows) {
                y=y+1; ndx=ndx-1;
            }
            if (z==npages) {
                z=z+1; ndx=ndx-nrc;
            }
            y=y-fy;
            x=x-fx;
            z=z-fz;
            
            in1=ndx-1;
            in2=ndx;
            // in3=ndx+nrows-1;
            in3=in1+nrows;
            // in4=ndx+nrows;
            in4=in3+1;
            
            // in5=ndx+nw-1;
            in5=in1+nrc;
            // in6=ndx+nw;
            in6=in5+1;
            // in7=ndx+nrows+nw-1;
            in7=in5+nrows;
            // in8=ndx+nrows+nw;
            in8 = in7+1;
            
            // for each volume
            for (i=0; i<nvols; i++){
                x1=1-x;
                z1=1-z;
                
                tmp=x1*z1;
                w2=y*tmp;
                w1=tmp-w2;
                
                tmp=x*z1;
                w4=y*tmp;
                w3=tmp-w4;
                
                tmp=x1*z;
                w6=y*tmp;
                w5=tmp-w6;
                
                tmp=x*z;
                w8=y*tmp;
                w7=tmp-w8;
                
                volskip=i*nrcp;
                I[n+i*ninterps]=D[in1+volskip]*w1+D[in2+volskip]*w2+
                        D[in3+volskip]*w3+D[in4+volskip]*w4+
                        D[in5+volskip]*w5+D[in6+volskip]*w6+
                        D[in7+volskip]*w7+D[in8+volskip]*w8;
            }   
        }   
    }
}


// ------- Main function definitions ----------
/* Input arguments */
#define IN_D		prhs[0]
#define IN_X		prhs[1]
#define IN_Y		prhs[2]
#define IN_Z		prhs[3]

/* Output arguments */
#define OUT_I		plhs[0]

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    double *D, *X, *Y, *Z, *I, bgval;
    size_t ninterps, i, nrows, ncols, npages, nvols;
    mwSize ndim, Xndim, newXndim, *newdims;
    const mwSize *dims, *Xdims;
    
    
    /* Check for input errors */
    if (nlhs>1)
        mexErrMsgTxt("Wrong number of output parameters, usage:  Output_images = mirt3D_mexinterp(Input_images, X, Y, Z)");
    if (nrhs>5 || nrhs<4)
        mexErrMsgTxt("Wrong number of input parameters, usage:  Output_images = mirt3D_mexinterp(Input_images, X, Y, Z, [bgval])");
    
    if (!mxIsDouble(IN_D) || !mxIsDouble(IN_X) || !mxIsDouble(IN_Y) || !mxIsDouble(IN_Z))
        mexErrMsgTxt("mirt3D_mexinterp: Input arguments must be double.");
    
    if ((mxGetNumberOfDimensions(IN_X) != mxGetNumberOfDimensions(IN_Y)) ||
            (mxGetNumberOfDimensions(IN_X) != mxGetNumberOfDimensions(IN_Z)) ||
            (mxGetNumberOfElements(IN_X) != mxGetNumberOfElements(IN_Y)) ||
            (mxGetNumberOfElements(IN_X) != mxGetNumberOfElements(IN_Z))) mexErrMsgTxt("Input parameters X, Y, Z must have the same size");
    
    if (nrhs==5) {
        if (mxGetNumberOfElements(prhs[4])!=1) mexErrMsgTxt("bgval must be a scalar.");
        bgval=mxGetScalar(prhs[4]);
    } else {
        bgval=mxGetNaN();
    }
    
    /* Get the sizes of each input argument */
    // Interpolated locations
    Xndim = mxGetNumberOfDimensions(IN_X);
    Xdims = mxGetDimensions(IN_X);
    ninterps=mxGetNumberOfElements(IN_X);
    
    // Data
    ndim = mxGetNumberOfDimensions(IN_D);
    dims = mxGetDimensions(IN_D);
    nrows = dims[0];
    ncols = dims[1];
    npages = dims[2];
    
    nvols=1;
    newXndim=Xndim;
    if (ndim>3) {       /*Check if we have several volumes*/
        if ((Xndim==2) && (Xdims[1]==1))
            newXndim=newXndim-1;	/*Check if interpolate along column vectors*/
        newdims = (mwSize*) mxCalloc(newXndim+1, sizeof(mwSize));	/*Allocate space for the new number of dimensions for output*/
        for (i = 0; i < newXndim; i++)
            newdims[i]=Xdims[i];	/*Copy original dimensions*/
        newdims[newXndim]=dims[3];	/*Add the number of  images as a last dimension*/
        newXndim=newXndim+1;        /*Set the new number of dimensions*/
        nvols=dims[3];
    } else {
        newdims = (mwSize*) mxCalloc(newXndim, sizeof(mwSize));
        for (i = 0; i < newXndim; i++)
            newdims[i]=Xdims[i];
    }
    
    // Input arguments
    D = mxGetPr(IN_D);
    X = mxGetPr(IN_X);
    Y = mxGetPr(IN_Y);
    Z = mxGetPr(IN_Z);
    
    // Output arguments
    //OUT_I = mxCreateNumericArray(newXndim, newdims, mxDOUBLE_CLASS, mxREAL);
    //I = mxGetPr(OUT_I);
    OUT_I=mxCreateDoubleMatrix(0,0,mxREAL);
    mxSetDimensions(OUT_I,newdims,newXndim);
    I=(double *) mxMalloc(ninterps*sizeof(double));
    mxSetPr(OUT_I,I);
    
    /* Do the actual computations in a subroutine */
    linterp(D, X, Y, Z, I, ninterps, nrows, ncols, npages, nvols, bgval);
}
