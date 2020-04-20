/* 3D linear interpolation
 *
 * DI=linterp3mex(D,XI,YI,ZI,[BG]) interpolates the 3D data at XI,YI,ZI.
 * D is defined on a regular grid 1:size(D,1), 1:size(D,2), 1:size(D,3).
 * Points outside the boundary are set to BG (optional), or NaN if not specified.
 * Using MATLAB, this is: DI = interp3(D,XI,YI,ZI,'linear',BG)
 *
 * D can be a stack of many 3D volumes (4D).
 *
 * mex -v -largeArrayDims COMPFLAGS="$COMPFLAGS /openmp" linterp3mex.cpp
 *
 * by Jon Chappelow
 * With code by Andriy Myronenko for handling multiple volumes
 */

#include <math.h>
#include "mex.h"

// Check windows arch
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#pragma message("64-bit Windows")
#else
#define ENVIRONMENT32
#pragma message("32-bit Windows")
#endif
#endif

// Check GCC arch
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#pragma message("64-bit GCC")
#else
#define ENVIRONMENT32
#pragma message("32-bit GCC")
#endif
#endif

#ifdef ENVIRONMENT64
typedef signed long long int signedsize_t;
#else
typedef signed long int signedsize_t;
#endif

template <class T> void linterp(T* D, T* X, T* Y, T* Z, T* I,
        size_t ninterps, size_t nrows, size_t ncols, size_t npages, size_t nvols, T bgval) {
    
    // unsigned long long int (__int64) for the array indexes:
    size_t volskip, nrc, nrcp;
    long long ii, n;
    // signed long long ints for the relative indexes:
    signedsize_t ndx, fx, fy, fz;
    signedsize_t inds[8], rowjump, coljump, pagejump; // indexes of the 8 neighbors
    // doubles for the interpolated data, fractional coefficients, and potentially oob locations
    T x, y, z, x_lowfrac, y_lowfrac, z_lowfrac, x_highfrac, z_highfrac, xzfrac, pixval;
    T weights[8]; // weights of the 8 neighbors
    
    nrc=nrows*ncols;
    nrcp=nrc*npages;
    
    #pragma omp parallel for shared(X, Y, Z, ninterps, I, D) private(n, pixval, x, y, z, fx, fy, fz, ndx, y_lowfrac, x_lowfrac, z_lowfrac, x_highfrac, z_highfrac, rowjump, coljump, pagejump, inds, weights, xzfrac)
    for (n=0; n<ninterps; n++) {
        
        x=X[n];
        y=Y[n];
        z=Z[n];
        
        if (x<1 || x>ncols || y<1 || y>nrows || z<1 || z>npages) {
            // Set bgval when outside volume
            for (size_t i=0; i<nvols; i++) I[n+i*ninterps]=bgval;
        } else  {
            
//             fx=(signedsize_t) floor((x==ncols ? --x : x));
//             fy=(signedsize_t) floor((y==nrows ? --y : y));
//             fz=(signedsize_t) floor((z==npages ? --z : z));
            fx=(signedsize_t) floor(x);
            fy=(signedsize_t) floor(y);
            fz=(signedsize_t) floor(z);
            
            ndx =  fy + (fx-1)*nrows + (fz-1)*nrc; // matlab inds, -1 below
            
            // fractions
            y_lowfrac=y-fy;
            x_lowfrac=x-fx;
            z_lowfrac=z-fz;
            x_highfrac=1-x_lowfrac;
            z_highfrac=1-z_lowfrac;
            
            rowjump=(y==ncols ? 0 : 1);
            coljump=(x==nrows ? 0 : nrows);
            pagejump=(z==npages ? 0 : nrc);
            
            // (x,y,z), (x,y+1,z)
            inds[0]=ndx-1;
            inds[1]=inds[0]+rowjump;
            // (x+1,y,z), (x+1,y+1,z)
            inds[2]=inds[0]+coljump;
            inds[3]=inds[2]+rowjump;
            // (x,y,z+1), (x,y+1,z+1)
            inds[4]=inds[0]+pagejump;
            inds[5]=inds[4]+rowjump;
            // (x+1,y,z+1), (x+1,y+1,z+1)
            inds[6]=inds[4]+coljump;
            inds[7]=inds[6]+rowjump;
            
//             inds[0]=ndx-1;
//             inds[1]=ndx;
//             inds[2]=inds[0]+nrows;
//             inds[3]=inds[2]+1;
//             inds[4]=inds[0]+nrc;
//             inds[5]=inds[4]+1;
//             inds[6]=inds[4]+nrows;
//             inds[7]=inds[6]+1;
            
            xzfrac=x_highfrac*z_highfrac;
            weights[1]=y_lowfrac*xzfrac;
            weights[0]=xzfrac-weights[1];
            
            xzfrac=x_lowfrac*z_highfrac;
            weights[3]=y_lowfrac*xzfrac;
            weights[2]=xzfrac-weights[3];
            
            xzfrac=x_highfrac*z_lowfrac;
            weights[5]=y_lowfrac*xzfrac;
            weights[4]=xzfrac-weights[5];
            
            xzfrac=x_lowfrac*z_lowfrac;
            weights[7]=y_lowfrac*xzfrac;
            weights[6]=xzfrac-weights[7];
            
//             for (ii=0; ii<4; ii++){
//                 if (ii==0)      xzfrac=x_highfrac*z_highfrac;
//                 else if (ii==1) xzfrac=x_lowfrac*z_highfrac;
//                 else if (ii==2) xzfrac=x_highfrac*z_lowfrac;
//                 else            xzfrac=x_lowfrac*z_lowfrac;
//                 
//                 weights[ii*2+1]=y_lowfrac*xzfrac;
//                 weights[ii*2]=xzfrac-weights[ii*2+1];
//             }

            // for each volume
            for (size_t i=0; i<nvols; i++){
                volskip=i*nrcp;
                pixval=0;
                
                //#pragma omp parallel for shared(inds, volskip, weights) private(ii) reduction(+: pixval)
                for (ii=0; ii<8; ii++){
                    pixval += D[inds[ii]+volskip] * weights[ii];
                }
                
                I[n+i*ninterps]=pixval;
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
//     double *D, *X, *Y, *Z, *I;
    double bgval;
    size_t ninterps, i, nrows, ncols, npages, nvols;
    mwSize ndim, Xndim, newXndim, *newdims;
    const mwSize *dims, *Xdims;
    
    
    /* Check for input errors */
    if (nlhs>1)
        mexErrMsgTxt("Wrong number of output parameters, usage:  DI = linterp3mex(D, X, Y, Z)");
    if (nrhs>5 || nrhs<4)
        mexErrMsgTxt("Wrong number of input parameters, usage:  DI = linterp3mex(D, X, Y, Z, [bgval])");
    
    const bool isDouble=(mxIsDouble(IN_D) && mxIsDouble(IN_X) && mxIsDouble(IN_Y) && mxIsDouble(IN_Z));
    const bool isSingle=(mxIsClass(IN_D,"single") && mxIsClass(IN_X,"single") && mxIsClass(IN_Y,"single") && mxIsClass(IN_Z,"single"));
    
    if (!isDouble && !isSingle)
        mexErrMsgTxt("linterp3mex: Input arguments must be float.");
    
    if ((mxGetNumberOfDimensions(IN_X) != mxGetNumberOfDimensions(IN_Y)) ||
            (mxGetNumberOfDimensions(IN_X) != mxGetNumberOfDimensions(IN_Z)) ||
            (mxGetNumberOfElements(IN_X) != mxGetNumberOfElements(IN_Y)) ||
            (mxGetNumberOfElements(IN_X) != mxGetNumberOfElements(IN_Z)))
        mexErrMsgTxt("Input parameters X, Y, Z must have the same size");
    
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
    
    if ((nrows==1) || (ncols==1)) mexErrMsgTxt("linterp3mex: Data must not be 1D.");
    if (npages==1) mexErrMsgTxt("linterp3mex: Data not 3D.");

    /* Andriy's code for handling multiple volumes */
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
    
    // Output arguments
    OUT_I = mxCreateNumericArray(newXndim, newdims, (isDouble ? mxDOUBLE_CLASS : mxSINGLE_CLASS), mxREAL);
//     I = mxGetPr(OUT_I);
    //OUT_I=mxCreateDoubleMatrix(0,0,mxREAL);
    //mxSetDimensions(OUT_I,newdims,newXndim);
    //I=(double *) mxMalloc(ninterps*sizeof(double));
    //mxSetPr(OUT_I,I);
    
    /* Do the actual computations in a subroutine */
    if (isSingle) {
        float *D, *X, *Y, *Z, *I, bgvals;
        I = (float *) mxGetData(OUT_I);
        D = (float *) mxGetData(IN_D);
        X = (float *) mxGetData(IN_X);
        Y = (float *) mxGetData(IN_Y);
        Z = (float *) mxGetData(IN_Z);
        bgvals=(float) bgval;
        linterp<float>(D, X, Y, Z, I, ninterps, nrows, ncols, npages, nvols, bgvals);
    } else {
        // Input arguments
        double *D, *X, *Y, *Z, *I;
        I = mxGetPr(OUT_I);
        D = mxGetPr(IN_D);
        X = mxGetPr(IN_X);
        Y = mxGetPr(IN_Y);
        Z = mxGetPr(IN_Z);
        linterp<double>(D, X, Y, Z, I, ninterps, nrows, ncols, npages, nvols, bgval);
    }
}
