/* 3D linear interpolation
 *
 * DI=linterp3mex(D,XI,YI,ZI,[BG]) interpolates the 3D data at XI,YI,ZI.
 * D is defined on a regular grid 1:size(D,1), 1:size(D,2), 1:size(D,3).
 * Points outside the boundary are set to BG (optional), or NaN if not specified.
 * Using MATLAB, this is: DI = interp3(D,XI,YI,ZI,'linear',BG)
 *
 * D can be a stack of many 3D volumes (4D).
 *
 * by Jon Chappelow
 * Originally based on code by Andriy Myronenko
 */

#include <math.h>
#include "mex.h"
#include "../general/maxNumCompThreads.h"

/*   undef needed for LCC compiler  */
#undef EXTERN_C
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#endif
        
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

struct linterp_threadargs{
    double* D;
    double* X;
    double* Y;
    double* Z;
    double* I;
    size_t ninterps;
    size_t nrows;
    size_t ncols;
    size_t npages;
    size_t nvols;
    double bgval;
    size_t ThreadID;
    size_t Nthreads;
};

/* GLOBALS */
size_t THREADLIMIT=8;
size_t THREADLIMIT_SOFT=3;

#ifdef _WIN32
unsigned __stdcall linterp(void *ThreadArgsV) {
#else
void *linterp(void *ThreadArgsV) {
#endif
    // unsigned long long int (__int64) for the array indexes:
    size_t n, i, ii, volskip, nrc, nrcp;
    // signed long long ints for the relative indexes:
    signedsize_t ndx, fx, fy, fz;
    signedsize_t inds[8], rowjump, coljump, pagejump;
    // doubles for the interpolated data, fractional coefficients, and potentially oob locations
    double x, y, z, x_lowfrac, y_lowfrac, z_lowfrac, x_highfrac, z_highfrac, xzfrac, pixval;
    double weights[8];
    
    linterp_threadargs *ThreadArgs=(linterp_threadargs *) ThreadArgsV;
    
    double *D, *X, *Y, *Z, *I, bgval;
    size_t ninterps, nrows, ncols, npages, nvols, ThreadOffset, Nthreads;
    
    D=ThreadArgs->D;
    X=ThreadArgs->X;
    Y=ThreadArgs->Y;
    Z=ThreadArgs->Z;
    I=ThreadArgs->I;
    ninterps=ThreadArgs->ninterps;
    nrows=ThreadArgs->nrows;
    ncols=ThreadArgs->ncols;
    npages=ThreadArgs->npages;
    nvols=ThreadArgs->nvols;
    bgval=ThreadArgs->bgval;
    ThreadOffset=ThreadArgs->ThreadID;
    Nthreads=ThreadArgs->Nthreads;
            
    nrc=nrows*ncols;
    nrcp=nrc*npages;
    
    for (n=ThreadOffset; n<ninterps; n+=Nthreads) {
        
        x=X[n];
        y=Y[n];
        z=Z[n];
        
        if (x<1 || x>ncols || y<1 || y>nrows || z<1 || z>npages) {
            // Set bgval when outside volume
            for (i=0; i<nvols; i++) I[n+i*ninterps]=bgval;
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
            
            // for each volume
            for (i=0; i<nvols; i++){
                volskip=i*nrcp;
                pixval=0;
                for (ii=0; ii<8; ii++){
                    pixval+=D[inds[ii]+volskip] * weights[ii];
                }
                I[n+i*ninterps]=pixval;
            }   
        }   
    }
    
    /* end thread */
    #ifdef _WIN32
	_endthreadex( 0 );
    return 0;
	#else
	pthread_exit(NULL);
	#endif
	
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
    size_t ninterps, i, nrows, ncols, npages, nvols, nmaxthreads;
    mwSize ndim, Xndim, newXndim, *newdims;
    const mwSize *dims, *Xdims;
    
    
    /* Check for input errors */
    if (nlhs>1)
        mexErrMsgTxt("Wrong number of output parameters, usage:  DI = linterp3mexmt(D, X, Y, Z, [bgval], [nmaxthreads])");
    if (nrhs>6 || nrhs<4)
        mexErrMsgTxt("Wrong number of input parameters, usage:  DI = linterp3mexmt(D, X, Y, Z, [bgval], [nmaxthreads])");
    
    if (!mxIsDouble(IN_D) || !mxIsDouble(IN_X) || !mxIsDouble(IN_Y) || !mxIsDouble(IN_Z))
        mexErrMsgTxt("linterp3mex: Input arguments must be double.");
    
    if ((mxGetNumberOfDimensions(IN_X) != mxGetNumberOfDimensions(IN_Y)) ||
            (mxGetNumberOfDimensions(IN_X) != mxGetNumberOfDimensions(IN_Z)) ||
            (mxGetNumberOfElements(IN_X) != mxGetNumberOfElements(IN_Y)) ||
            (mxGetNumberOfElements(IN_X) != mxGetNumberOfElements(IN_Z))) mexErrMsgTxt("Input parameters X, Y, Z must have the same size");
    
    if (nrhs>=5  && !mxIsEmpty(prhs[4])) {
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
    
    // threads
    size_t threadlim;
    if (nrhs>=6) {
        nmaxthreads=(size_t) mxGetScalar(prhs[5]);
        threadlim=THREADLIMIT;
    } else {
        nmaxthreads=getNumCores();
        threadlim=THREADLIMIT_SOFT;
    }
    
    size_t Nthreads=(nmaxthreads>threadlim ? threadlim : nmaxthreads); // hard thread limit
    
    if ((nrows==1) || (ncols==1)) mexErrMsgTxt("linterp3mex: Data must not be 1D.");
    
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
    OUT_I = mxCreateNumericArray(newXndim, newdims, mxDOUBLE_CLASS, mxREAL);
    I = mxGetPr(OUT_I);
    //OUT_I=mxCreateDoubleMatrix(0,0,mxREAL);
    //mxSetDimensions(OUT_I,newdims,newXndim);
    //I=(double *) mxMalloc(ninterps*sizeof(double));
    //mxSetPr(OUT_I,I);
    
    // Threading
    size_t *ThreadID;
    linterp_threadargs **ThreadArgs;
    linterp_threadargs *ThreadArgsi;
    
    /* Handles to the worker threads */
	#ifdef _WIN32
		HANDLE *ThreadList; 
    #else
		pthread_t *ThreadList;
	#endif
    
    /* Reserve room for handles of threads in ThreadList */
    #ifdef _WIN32
		ThreadList = (HANDLE *) malloc(Nthreads*sizeof(HANDLE));
    #else
		ThreadList = (pthread_t *) malloc(Nthreads*sizeof(pthread_t));
	#endif
	
	ThreadID = (size_t *) malloc(Nthreads*sizeof(size_t));
	ThreadArgs = (linterp_threadargs **) malloc(Nthreads*sizeof(linterp_threadargs *));
    
    // Find kNNs for each point by traversing kd tree
    for (i=0; i<Nthreads; i++) {
        
        ThreadID[i]=i;
        
        ThreadArgsi = (linterp_threadargs *) malloc(sizeof(linterp_threadargs));
        ThreadArgsi->D=D;
        ThreadArgsi->X=X;
        ThreadArgsi->Y=Y;
        ThreadArgsi->Z=Z;
        ThreadArgsi->I=I;
        ThreadArgsi->ninterps=ninterps;
        ThreadArgsi->nrows=nrows;
        ThreadArgsi->ncols=ncols;
        ThreadArgsi->npages=npages;
        ThreadArgsi->nvols=nvols;
        ThreadArgsi->bgval=bgval;
        ThreadArgsi->ThreadID=ThreadID[i];
        ThreadArgsi->Nthreads=Nthreads;
        
        /* Start thread  */
        ThreadArgs[i]=ThreadArgsi; // now we can overwrite ThreadArgsi for the next thread
        
        #ifdef _WIN32
            ThreadList[i] = (HANDLE)_beginthreadex( NULL, 0, &linterp, ThreadArgs[i] , 0, NULL );
        #else
            pthread_create((pthread_t*)&ThreadList[i], NULL, &linterp, ThreadArgs[i]);
        #endif
    }
    
    #ifdef _WIN32
            for (i=0; i<Nthreads; i++) WaitForSingleObject(ThreadList[i], INFINITE);
            for (i=0; i<Nthreads; i++) CloseHandle( ThreadList[i] );
    #else
            for (i=0; i<Nthreads; i++) pthread_join(ThreadList[i], NULL);
    #endif
    
	for (i=0; i<Nthreads; i++) free(ThreadArgs[i]);
    free(ThreadArgs);
    free(ThreadID);
    free(ThreadList);
}
