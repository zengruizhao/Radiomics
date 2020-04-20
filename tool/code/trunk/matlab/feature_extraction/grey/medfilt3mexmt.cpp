#include "mex.h"
#include <cmath>
#include <algorithm>

#include "../../general/maxNumCompThreads.h"

/*   undef needed for LCC compiler  */
#undef EXTERN_C
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
    #include <unistd.h>
	#include <pthread.h>
#endif

#ifndef min
#define min(X, Y)  (X < Y ? X : Y)
#endif
#ifndef max
#define max(X, Y)  (X < Y ? Y : X)
#endif

inline bool greater (int i,int j) { return (j<i); }

inline double logb(double x, double b) { return log(x)/log(b); }

// prototype the break handling functions in libut (C library)
#ifdef __cplusplus
    extern "C" bool utIsInterruptPending();
    extern "C" void utSetInterruptPending(bool);
#else
    extern bool utIsInterruptPending();
    extern void utSetInterruptPending(bool);
#endif

struct medfilt3_threadargs{
    double*** volmat;
    double* medvals;
    int* ws;
    int rows;
    int cols;
    int slices;
    size_t ThreadID;
    size_t Nthreads;
};

/* GLOBALS */
size_t THREADLIMIT=4;

#ifdef _WIN32
unsigned __stdcall medfilt3(void *ThreadArgsV) {
#else
void *medfilt3(void *ThreadArgsV) {
#endif
// void medfilt3(double ***volmat, double *medvals, int *ws, 
//         int rows, int cols, int slices) {
    
    double *blockvals;
    size_t ii, i, j, k, ib, jb, kb, blen, nblockpix, slicepixels;
    int hwsx, hwsy, hwsz;
    
    size_t block_start_row, block_start_col, block_start_slice;
    size_t block_end_row, block_end_col, block_end_slice;
    
    medfilt3_threadargs *ThreadArgs=(medfilt3_threadargs *) ThreadArgsV;
    
    double ***volmat, *medvals;
    int *ws, rows, cols, slices;
    size_t ThreadOffset, Nthreads;
    
    volmat=ThreadArgs->volmat;
    medvals=ThreadArgs->medvals;
    ws=ThreadArgs->ws;
    rows=ThreadArgs->rows;
    cols=ThreadArgs->cols;
    slices=ThreadArgs->slices;
    ThreadOffset=ThreadArgs->ThreadID;
    Nthreads=ThreadArgs->Nthreads;
    
    slicepixels=rows*cols;
    
    hwsy=(int) floor((float) ws[0]/2);
    hwsx=(int) floor((float) ws[1]/2);
    hwsz=(int) floor((float) ws[2]/2);
    
    nblockpix=ws[0]*ws[1]*ws[2];
    blockvals=(double *) calloc(nblockpix,sizeof(double));
    
    ii=ThreadOffset*rows; // jump to the starting column for this thread
    for (k = 0; k<slices; ++k) {
        for (j = ThreadOffset; j<cols; j+=Nthreads, ii+=rows*(Nthreads-1)) {
            for (i = 0; i<rows; ++i, ++ii) {
                
                block_start_row = max(0, i-hwsy);
                block_start_col = max(0, j-hwsx);
                block_start_slice = max(0, k-hwsz);
                block_end_row = min(rows-1, i+hwsy);
                block_end_col = min(cols-1, j+hwsx);
                block_end_slice = min(slices-1, k+hwsz);
                
                for (kb = block_start_slice, blen=0; kb <= block_end_slice; ++kb)
                    for (jb = block_start_col; jb <= block_end_col; ++jb)
                        for (ib = block_start_row; ib <= block_end_row; ++ib, ++blen) {
                            blockvals[blen]=volmat[ib][jb][kb];
                        }
                
                std::sort(&blockvals[0], &blockvals[blen]);
//                 std::sort(blockvals, blockvals+blen);
//                 medvals[i+j*rows+k*slicepixels] = blen % 2 ? blockvals[blen / 2] : (blockvals[blen/2 - 1] + blockvals[blen/2]) / 2;
                if (blen % 2)
                    medvals[i+j*rows+k*slicepixels] = blockvals[blen / 2];
                else if (blockvals[blen / 2]>0) {
                    medvals[i+j*rows+k*slicepixels] = (blockvals[blen/2 - 1] + blockvals[blen/2]) / 2;
                } else 
                medvals[i+j*rows+k*slicepixels]=0;
            }
            
        }
    }
    
    free(blockvals);
    
    /* end thread */
    #ifdef _WIN32
	_endthreadex( 0 );
    return 0;
	#else
	pthread_exit(NULL);
	#endif
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    double *vol, ***volmat, *medvals, *wsd;
    int rows, cols, slices, i, j, k, ndims, slicepixels;
    const mwSize *imdims;
    int ws[3];
    size_t nwsels;
    
    if(nrhs > 2 || nrhs < 1)
        mexErrMsgTxt("medfilt3mexmt(volume,ws)");
    
    if(!mxIsDouble(prhs[0]))
        mexErrMsgTxt("Input volume must be DOUBLE.");
    
    vol = mxGetPr(prhs[0]);
    imdims=mxGetDimensions(prhs[0]);
    ndims=(int) mxGetNumberOfDimensions(prhs[0]);
    if (ndims!=3) mexErrMsgTxt("Input volume must have 3 dimensions.");
    rows=(int) imdims[0]; cols=(int) imdims[1]; slices=(int) imdims[2];
    slicepixels=rows*cols;

    nwsels = mxGetNumberOfElements(prhs[1]);
    if (nwsels==3) {
        wsd = mxGetPr(prhs[1]);
        ws[0] = (int) wsd[0]; ws[1] = (int) wsd[1]; ws[2] = (int) wsd[2];
    } else if (nwsels==1) {
        wsd = mxGetPr(prhs[1]);
        ws[0] = (int) *wsd; ws[1] = (int) *wsd; ws[2] = (int) *wsd;
    } else
        mexErrMsgTxt("ws must be a 1 or 3 element vector.");
    
    volmat = (double ***) mxMalloc(rows*sizeof(double **));
    for (i=0; i<rows; i++) {
        volmat[i] = (double **) mxMalloc(cols*sizeof(double *));
        for (j=0; j<cols; j++)
            volmat[i][j] = (double *) mxMalloc(slices*sizeof(double));
    }
    
    for (k=0; k<slices; k++)
        for (j=0; j<cols; j++)
            for (i=0; i<rows; i++)
                volmat[i][j][k]=vol[i+j*rows+k*slicepixels];
    
//     meddims[0] = rows; meddims[1] = cols; meddims[2] = slices;
    plhs[0] = mxCreateNumericArray(3, imdims, mxDOUBLE_CLASS, mxREAL);
    medvals = mxGetPr(plhs[0]);
    
    // Threading
    size_t *ThreadID;
    medfilt3_threadargs **ThreadArgs;
    medfilt3_threadargs *ThreadArgsi;
    
    /* Handles to the worker threads */
	#ifdef _WIN32
		HANDLE *ThreadList; 
    #else
		pthread_t *ThreadList;
	#endif
    
    size_t Nthreads;
    size_t maxthreads=(size_t) getNumCores();
    Nthreads=(maxthreads>THREADLIMIT ? THREADLIMIT : maxthreads); // hard thread limit
//     if (Nthreads>nmaxthreads) Nthreads=nmaxthreads; // input thread limit
    
    /* Reserve room for handles of threads in ThreadList */
    #ifdef _WIN32
		ThreadList = (HANDLE *) malloc(Nthreads*sizeof(HANDLE));
    #else
		ThreadList = (pthread_t *) malloc(Nthreads*sizeof(pthread_t));
	#endif
	
	ThreadID = (size_t *) malloc(Nthreads*sizeof(size_t));
	ThreadArgs = (medfilt3_threadargs **) malloc(Nthreads*sizeof(medfilt3_threadargs *));
    
    // Find kNNs for each point by traversing kd tree
    for (i=0; i<Nthreads; i++) {
        
        ThreadID[i]=i;
        
        ThreadArgsi = (medfilt3_threadargs *) malloc(sizeof(medfilt3_threadargs));
        ThreadArgsi->volmat=volmat;
        ThreadArgsi->medvals=medvals;
        ThreadArgsi->ws=ws;
        ThreadArgsi->rows=rows;
        ThreadArgsi->cols=cols;
        ThreadArgsi->slices=slices;
        ThreadArgsi->ThreadID=ThreadID[i];
        ThreadArgsi->Nthreads=Nthreads;
        
        /* Start thread  */
        ThreadArgs[i]=ThreadArgsi; // now we can overwrite ThreadArgsi for the next thread
        
        #ifdef _WIN32
            ThreadList[i] = (HANDLE)_beginthreadex( NULL, 0, &medfilt3, ThreadArgs[i] , 0, NULL );
        #else
            pthread_create((pthread_t*)&ThreadList[i], NULL, &medfilt3, ThreadArgs[i]);
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
    
    // medfilt3(volmat,medvals,ws,rows,cols,slices);
}
