#include "mex.h"
#include "math.h"
#include "../general/maxNumCompThreads.h"

/*   undef needed for LCC compiler  */
#undef EXTERN_C
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#endif

/* GLOBALS */
size_t THREADLIMIT=3;

struct ksplat_threadargs{
    double* Bx;
    double* By;
    double* H;
    double* Fx;
    double* Fy;
    mwSize Bsizex;
    mwSize Bsizey;
    mwSize Hsizex;
    mwSize Hsizey;
    size_t ThreadID;
    size_t ThreadStage;
    size_t Nthreads;
    double* totalweight;
};

/*
 * [Fx,Fy]=ksplat2mex(Bx,By,H);
 *
 * inputs,
 * Bx,By : The backward transformation fields
 * H : The Splatting kernel
 * outputs,
 * Fx,Fy : The forward transformation fields
 *
 * Based on code originally by D.Kroon
 */

/* Convert 2D/3D matrix index to 1D index */
__inline int sub2ind2d(int irow, int jcol, int nrows) { return (irow + jcol*nrows); }

#ifdef _WIN32
unsigned __stdcall ksplat(void *ThreadArgsV) {
#else
void *ksplat(void *ThreadArgsV) {
#endif
    int khw, khh;
    double dx, dy;
    int index, index2, t, iHx, iHy, ncoords, iH, colskip;
    int x0, x1, y0, y1;
    double fracs[4], pct;
    double xhighfrac, yhighfrac;
    double Tlocalx, Tlocaly;
    
    /* X,Y coordinates of current pixel */
    int x, y, tx, ty, tx0, tx1, ty0, ty1, Bsizeyi, Hsizeyi;
    //int tx01[2], ty01[2], ix, iy;
            
    ksplat_threadargs *ThreadArgs=(ksplat_threadargs *) ThreadArgsV;
    
    double *Bx, *By, *H, thread_width, block_jump;
    //volatile double *totalweight, *Fx, *Fy;
    double *totalweight, *Fx, *Fy;
    size_t Bsizex, Bsizey, Hsizex, Hsizey;
    size_t ThreadID, Nthreads, ThreadStage, ThreadXstart, ThreadXend;
    
    Bx=ThreadArgs->Bx;
    By=ThreadArgs->By;
    H=ThreadArgs->H;
    Fx=ThreadArgs->Fx;
    Fy=ThreadArgs->Fy;
    Bsizex=ThreadArgs->Bsizex;
    Bsizey=ThreadArgs->Bsizey;
    Hsizex=ThreadArgs->Hsizex;
    Hsizey=ThreadArgs->Hsizey;
    ThreadID=ThreadArgs->ThreadID;
    ThreadStage=ThreadArgs->ThreadStage; // 0 or 1
    Nthreads=ThreadArgs->Nthreads;
    totalweight=ThreadArgs->totalweight;
    
    Bsizeyi=(int) Bsizey;
	Hsizeyi=(int) Hsizey;
    
    thread_width=((double) Bsizex)/((double) Nthreads*2.0);
    block_jump=(double) (2.0*ThreadID+ThreadStage);
    
    ThreadXstart=(size_t) floor(thread_width*block_jump);
    ThreadXend=(size_t) floor(thread_width*(block_jump+1.));
    
    /* Measure total weight of any and all pixels mapped to each pixel */
    ncoords=Bsizex*Bsizey;
    
    /* Gaussian kernel half-width/height */
    khw=(int) floor((float) Hsizex/2);
    khh=(int) floor((float) Hsizey/2);
    
    /*  Loop through all image pixel coordinates (of destination) -- find their source via Bx,By (the fractional destination) */
    for (x=ThreadXstart; x<ThreadXend; x++) {
        colskip=x*Bsizeyi;
        for (y=0; y<Bsizey; y++) {
            //index=sub2ind2d(y, x, Bsizeyi);
            index=y+colskip;
            dx=Bx[index];
            dy=By[index];
            //if ((fabs(dx)<0.001) && (fabs(dy)<0.001)) continue;
            Tlocalx=x+dx;
            Tlocaly=y+dy;
            
            /* Determine the coordinates of the pixel(s) which will become the current pixel */
            /* (using linear interpolation) */
            x0=(int) floor(Tlocalx);
            y0=(int) floor(Tlocaly);
            //if ((x0<0) || (y0<0)) continue;
            x1=x0+1;
            y1=y0+1;
            
            /* Linear interpolation constants (percentages) */
            xhighfrac=Tlocalx-floor(Tlocalx); yhighfrac=Tlocaly-floor(Tlocaly);
            fracs[3]=xhighfrac * yhighfrac;
            fracs[0]=1+fracs[3]-xhighfrac-yhighfrac; //(1-xhighfrac) * (1-yhighfrac);
            fracs[1]=yhighfrac-fracs[3]; //(1-xhighfrac) * yhighfrac;
            fracs[2]=xhighfrac-fracs[3]; //xhighfrac * (1-yhighfrac);
            
            /* Loop through the whole kernel */
            x0-=khw; x1-=khw;
            y0-=khh; y1-=khh;
            for (iHx=0, iH=0; iHx<Hsizex; iHx++) {
                tx0=x0+iHx; tx1=x1+iHx;
                for (iHy=0; iHy<Hsizey; iHy++, iH++) {
                    /* Process all 4 neighbors of current kernel pixel */
                    //index2=sub2ind2d(iHy, iHx, Hsizeyi);
                    //if (H[iH]<0.00001) continue;
                    if (H[iH]<0.0001) continue;
                    
                    //tx0=x0+iHx; tx1=x1+iHx;
                    ty0=y0+iHy; ty1=y1+iHy;
                    for(t=0; t<4; t++) {
                        
                        if(t==0) {tx=tx0; ty=ty0;}
                        else if(t==1) {tx=tx0; ty=ty1;}
                        else if(t==2) {tx=tx1; ty=ty0;}
                        else {tx=tx1; ty=ty1;}
                        
                        if ((tx>=0)&&(ty>=0)&&(tx<Bsizex)&&(ty<Bsizey)) {
                            pct=fracs[t] * H[iH];
                            //if (pct<(0.00001/(dx+dy))) continue;
                            //if (pct<0.000005) continue;
                            if (pct<0.0001) continue;
                            //index=sub2ind2d(ty, tx, Bsizeyi);
                            index=ty+tx*Bsizeyi;
                            Fx[index]-=dx*pct;
                            Fy[index]-=dy*pct;
                            totalweight[index]+=pct;
                        }
                    }
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


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    /* MATLAB Input and output variables */
    double *Fx, *Fy, *Bx, *By, *H, *totalweight;
    
    /* Deformation field */
    mwSize Bsizex, Bsizey;
    const mwSize *Bdims;
    size_t ncoords, ThreadStage, i;
    
    /* Kernel */
    mwSize Hsizex, Hsizey;
    const mwSize *Hdims;
    
    if(nrhs!=3)
        mexErrMsgTxt("Three inputs are required:\n\t[Fx,Fy]=ksplat2mexmt(Bx,By,H);");
    else if(nlhs!=2)
        mexErrMsgTxt("Two outputs are required:\n\t[Fx,Fy]=ksplat2mexmt(Bx,By,H);");
    
    /* Input pointers */
    Bx=mxGetPr(prhs[0]);
    By=mxGetPr(prhs[1]);
    H=mxGetPr(prhs[2]);
    
    /* Kernel dims */
    Hdims = mxGetDimensions(prhs[2]);
    Hsizey = Hdims[0]; Hsizex = Hdims[1];
    
    /* Deformation field dims */
    Bdims = mxGetDimensions(prhs[0]);
    ncoords=(size_t) mxGetNumberOfElements(prhs[0]);
    Bsizey = Bdims[0]; Bsizex = Bdims[1];
    
    /* Allocate for output deformation fields (Fx,Fy) */
    plhs[0] = mxCreateNumericArray(2, Bdims, mxDOUBLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericArray(2, Bdims, mxDOUBLE_CLASS, mxREAL);
    Fx = mxGetPr(plhs[0]);
    Fy = mxGetPr(plhs[1]);
    /* The following way avoids zero initialization */
//     plhs[0]=mxCreateDoubleMatrix(0,0,mxREAL); //mxCreateDoubleMatrix(0,0,mxDOUBLE_CLASS,mxREAL);
//     plhs[1]=mxCreateDoubleMatrix(0,0,mxREAL);
//     mxSetDimensions(plhs[0], Bdims, 2);
//     mxSetDimensions(plhs[1], Bdims, 2);
//     Fx =(double *) mxMalloc(ncoords*sizeof(double));
//     Fy =(double *) mxMalloc(ncoords*sizeof(double));
//     mxSetPr(plhs[0], Fx);
//     mxSetPr(plhs[1], Fy);
    
    totalweight = (double *) calloc(ncoords, sizeof(double));
    
    // Do it!
    // Threading
    size_t *ThreadID;
    ksplat_threadargs **ThreadArgs;
    ksplat_threadargs *ThreadArgsi;
    
    /* Handles to the worker threads */
	#ifdef _WIN32
		HANDLE *ThreadList; 
    #else
		pthread_t *ThreadList;
	#endif
    
    size_t Nthreads;
    size_t maxthreads=(size_t) getNumCores();
    Nthreads=(maxthreads>THREADLIMIT ? THREADLIMIT : maxthreads); // hard thread limit
    //if (Nthreads>threadlim) Nthreads=threadlim; // input thread limit
    
    /* Reserve room for handles of threads in ThreadList */
    #ifdef _WIN32
		ThreadList = (HANDLE *) malloc(Nthreads*sizeof(HANDLE));
    #else
		ThreadList = (pthread_t *) malloc(Nthreads*sizeof(pthread_t));
	#endif
	
	ThreadID = (size_t *) malloc(Nthreads*sizeof(size_t));
    ThreadArgs = (ksplat_threadargs **) malloc(Nthreads*sizeof(ksplat_threadargs *));
    for (ThreadStage=0; ThreadStage<2; ThreadStage++) {
        // Find kNNs for each point by traversing kd tree
        for (i=0; i<Nthreads; i++) {
            
            ThreadID[i]=i;
            
            ThreadArgsi = (ksplat_threadargs *) malloc(sizeof(ksplat_threadargs));
            ThreadArgsi->Bx=Bx;
            ThreadArgsi->By=By;
            ThreadArgsi->H=H;
            ThreadArgsi->Fx=Fx;
            ThreadArgsi->Fy=Fy;
            ThreadArgsi->Bsizex=Bsizex;
            ThreadArgsi->Bsizey=Bsizey;
            ThreadArgsi->Hsizex=Hsizex;
            ThreadArgsi->Hsizey=Hsizey;
            ThreadArgsi->ThreadID=ThreadID[i];
            ThreadArgsi->ThreadStage=ThreadStage;
            ThreadArgsi->Nthreads=Nthreads;
            ThreadArgsi->totalweight=totalweight;
            
            /* Start thread  */
            ThreadArgs[i]=ThreadArgsi; // now we can overwrite ThreadArgsi pointer for the next thread
            
            #ifdef _WIN32
                    ThreadList[i] = (HANDLE)_beginthreadex( NULL, 0, &ksplat, ThreadArgs[i] , 0, NULL );
            #else
                    pthread_create((pthread_t*)&ThreadList[i], NULL, &ksplat, ThreadArgs[i]);
            #endif
        }
        
        #ifdef _WIN32
                for (i=0; i<Nthreads; i++) WaitForSingleObject(ThreadList[i], INFINITE);
                for (i=0; i<Nthreads; i++) CloseHandle( ThreadList[i] );
        #else
                for (i=0; i<Nthreads; i++) pthread_join(ThreadList[i], NULL);
        #endif
        
        for (i=0; i<Nthreads; i++) free(ThreadArgs[i]);  //  Frees the memory allocated to each ThreadArgsi
    }
    
    free(ThreadID);
    free(ThreadArgs);
    free(ThreadList);
    
    /* divide by total weight of any and all pixels mapped to each location */
    for (i=0; i<ncoords; i++) {
        //Fx[i]/=(totalweight[i]+0.00000001);
        //Fy[i]/=(totalweight[i]+0.00000001);
        if (totalweight[i]>0.0000001) {
            Fx[i]/=totalweight[i];
            Fy[i]/=totalweight[i];
        } else { // when totalweight is small, so are Fx and Fy
            Fx[i]=0;
            Fy[i]=0;
        }
    }
    
    free(totalweight);
    
}
