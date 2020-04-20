/* 
 * Px_displaced, Py_displaced, Pz_displaced, Bproducts, localinds --> tlocalx, tlocaly, tlocalz
 */

#include <math.h>
#include "mex.h"
#include "../../general/maxNumCompThreads.h"

/*   undef needed for LCC compiler  */
#undef EXTERN_C
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#endif
        
struct bprod2tlocal_threadargs{
    float *bprods;
    double *Pxdispl;
    double *Pydispl;
    double *Pzdispl;
    long *tlis;
    double *tlocalx;
    double *tlocaly;
    double *tlocalz;
    size_t npts;
    size_t ThreadID;
    size_t Nthreads;
};

/* GLOBALS */
size_t THREADLIMIT=4;

#ifdef _WIN32
unsigned __stdcall bprod2tlocal(void *ThreadArgsV) {
#else
void *bprod2tlocal(void *ThreadArgsV) {
#endif
    
    size_t m, n, l, i, bpind, tlival, dimskip;
    
    bprod2tlocal_threadargs *ThreadArgs=(bprod2tlocal_threadargs *) ThreadArgsV;
    
    float* bprods;
    double *Pxdispl, *Pydispl, *Pzdispl, *tlocalx, *tlocaly, *tlocalz;
    long *tlis;
    size_t npts;
    size_t ThreadOffset, Nthreads;
    
    bprods=ThreadArgs->bprods;
    Pxdispl=ThreadArgs->Pxdispl;
    Pydispl=ThreadArgs->Pydispl;
    Pzdispl=ThreadArgs->Pzdispl;
    tlis=ThreadArgs->tlis;
    tlocalx=ThreadArgs->tlocalx;
    tlocaly=ThreadArgs->tlocaly;
    tlocalz=ThreadArgs->tlocalz;
    npts=ThreadArgs->npts;
    ThreadOffset=ThreadArgs->ThreadID;
    Nthreads=ThreadArgs->Nthreads;
    
    for (l=0; l<4; l++) {
        for (m=0; m<4; m++) {
            for (n=0; n<4; n++) {
                dimskip=npts*(m + n*4 + l*16);
                for (i=ThreadOffset; i<npts; i+=Nthreads) {
                    //bpind=i + m*npts + n*4*npts + l*16*npts;
                    //bpind=i + npts*(m + n*4 + l*16);
                    bpind=i + dimskip;
                    tlival=(size_t) tlis[bpind]-1; // tlis>=1 in mexFunction
                    tlocalx[i]+=(bprods[bpind]*Pxdispl[tlival]);
                    tlocaly[i]+=(bprods[bpind]*Pydispl[tlival]);
                    tlocalz[i]+=(bprods[bpind]*Pzdispl[tlival]);
                }
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

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    double *Pxdispl, *Pydispl, *Pzdispl, *tlocalx, *tlocaly, *tlocalz;
    long *tlis;
    float *bprods;
    
    size_t i, npts, mdimsize, ndimsize, ldimsize, ncps, nmaxthreads;
    
    mwSize bprodndims, pxpypzndims, tlocaldims[3];
    const mwSize *bproddims, *pxpypzdims;
    
    /* Check for input errors */
    if (nlhs>3 || nrhs>6 || nrhs<5)
        mexErrMsgTxt("Wrong number of output parameters, usage:  [tlocalx,tlocaly,tlocalz] = bprodstotlocals3mexmt(Bproducts,Pxdispl,Pydispl,Pzdispl,tlis,[nmaxthreads])");
    
    if (!mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]) || !mxIsDouble(prhs[3]))
        mexErrMsgTxt("bprodstotlocals3mexmt: Input arguments must be double.");
    if (!mxIsSingle(prhs[0]))
        mexErrMsgTxt("bprodstotlocals3mexmt: Bproducts must be single.");
    if (!mxIsClass(prhs[4],"uint32"))
        mexErrMsgTxt("bprodstotlocals3mexmt: tlis must be uint32.");
    
    if ((mxGetNumberOfDimensions(prhs[1]) != mxGetNumberOfDimensions(prhs[2])) ||
            (mxGetNumberOfElements(prhs[1]) != mxGetNumberOfElements(prhs[2])) ||
            (mxGetNumberOfDimensions(prhs[1]) != mxGetNumberOfDimensions(prhs[3])) ||
            (mxGetNumberOfElements(prhs[1]) != mxGetNumberOfElements(prhs[3]))) mexErrMsgTxt("Input parameters Pxdispl, Pydispl and Pzdispl must have the same size");
    if ((mxGetNumberOfDimensions(prhs[0]) != mxGetNumberOfDimensions(prhs[4])) ||
            (mxGetNumberOfElements(prhs[0]) != mxGetNumberOfElements(prhs[4]))) mexErrMsgTxt("Input parameters Bproducts and tlis must have the same size");
    
    /* Get the sizes of each input argument */
    bprodndims = mxGetNumberOfDimensions(prhs[0]);
    if (bprodndims!=4) mexErrMsgTxt("Bproducts and tlis must be npts-by-4-by-4-by-4.");
    bproddims = mxGetDimensions(prhs[0]);
    npts = bproddims[0];
    mdimsize = bproddims[1]; ndimsize = bproddims[2]; ldimsize = bproddims[3];
    if ((mdimsize!=4) || (ndimsize!=4) || (ldimsize!=4)) mexErrMsgTxt("Bproducts and tlis must be npts-by-4-by-4-by-4.");
    
    pxpypzndims = mxGetNumberOfDimensions(prhs[1]);
    pxpypzdims = mxGetDimensions(prhs[1]);
    ncps=mxGetNumberOfElements(prhs[1]);
    //if (pxpyndims!=2) 
    
    // Input arguments pointers
    //bprods = mxGetPr(prhs[0]);
    bprods = (float *) mxGetData(prhs[0]);
    Pxdispl = mxGetPr(prhs[1]);
    Pydispl = mxGetPr(prhs[2]);
    Pzdispl = mxGetPr(prhs[3]);
    //tlis = mxGetPr(prhs[4]);
    tlis = (long *) mxGetData(prhs[4]);
    for (i=0; i<(npts*64); i++)
        if ((tlis[i]>ncps) || (tlis[i]<1)) mexErrMsgTxt("tlis out of range for Pxdispl, Pydispl, Pzdispl.");
    
    if (nrhs>=6)
        nmaxthreads=(size_t) mxGetScalar(prhs[5]);
	else
        nmaxthreads=64;
    
    // Output arguments
    tlocaldims[0]=npts; tlocaldims[1]=1;
    plhs[0] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    plhs[2] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    tlocalx = mxGetPr(plhs[0]);
    tlocaly = mxGetPr(plhs[1]);
    tlocalz = mxGetPr(plhs[2]);
    
    // Threading
    size_t *ThreadID;
    bprod2tlocal_threadargs **ThreadArgs;
    bprod2tlocal_threadargs *ThreadArgsi;
    
    /* Handles to the worker threads */
	#ifdef _WIN32
		HANDLE *ThreadList; 
    #else
		pthread_t *ThreadList;
	#endif
    
    size_t Nthreads;
    size_t maxthreads=(size_t) getNumCores();
    Nthreads=(maxthreads>THREADLIMIT ? THREADLIMIT : maxthreads); // hard thread limit
    if (Nthreads>nmaxthreads) Nthreads=nmaxthreads; // input thread limit
    
    /* Reserve room for handles of threads in ThreadList */
    #ifdef _WIN32
		ThreadList = (HANDLE *) malloc(Nthreads*sizeof(HANDLE));
    #else
		ThreadList = (pthread_t *) malloc(Nthreads*sizeof(pthread_t));
	#endif
	
	ThreadID = (size_t *) malloc(Nthreads*sizeof(size_t));
	ThreadArgs = (bprod2tlocal_threadargs **) malloc(Nthreads*sizeof(bprod2tlocal_threadargs *));
    
    // Find kNNs for each point by traversing kd tree
    for (i=0; i<Nthreads; i++) {
        
        ThreadID[i]=i;
        
        ThreadArgsi = (bprod2tlocal_threadargs *) malloc(sizeof(bprod2tlocal_threadargs));
        ThreadArgsi->bprods=bprods;
        ThreadArgsi->Pxdispl=Pxdispl;
        ThreadArgsi->Pydispl=Pydispl;
        ThreadArgsi->Pzdispl=Pzdispl;
        ThreadArgsi->tlis=tlis;
        ThreadArgsi->tlocalx=tlocalx;
        ThreadArgsi->tlocaly=tlocaly;
        ThreadArgsi->tlocalz=tlocalz;
        ThreadArgsi->npts=npts;
        ThreadArgsi->ThreadID=ThreadID[i];
        ThreadArgsi->Nthreads=Nthreads;
        
        /* Start thread  */
        ThreadArgs[i]=ThreadArgsi; // now we can overwrite ThreadArgsi for the next thread
        
        #ifdef _WIN32
            ThreadList[i] = (HANDLE)_beginthreadex( NULL, 0, &bprod2tlocal, ThreadArgs[i] , 0, NULL );
        #else
            pthread_create((pthread_t*)&ThreadList[i], NULL, &bprod2tlocal, ThreadArgs[i]);
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
    
    /* Do the actual computations in a subroutine */
//     bprod2tlocal(bprods,Pxdispl,Pydispl,Pzdispl,tlis,tlocalx,tlocaly,tlocalz,npts);
}
