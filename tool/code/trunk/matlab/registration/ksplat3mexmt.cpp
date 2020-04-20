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
size_t THREADLIMIT=8;

struct ksplat_threadargs{
    double* Bx;
    double* By;
    double* Bz;
    double* H;
    double* Fx;
    double* Fy;
    double* Fz;
    mwSize Bsizex;
    mwSize Bsizey;
    mwSize Bsizez;
    mwSize Hsizex;
    mwSize Hsizey;
    mwSize Hsizez;
    size_t ThreadID;
    size_t ThreadStage;
    size_t Nthreads;
    double* totalweight;
};

/*
 * [Fx,Fy]=ksplat3mex(Bx,By,H);
 *
 * inputs,
 * Bx,By : The backward transformation fields
 * H : The Splatting kernel
 * outputs,
 * Fx,Fy : The forward transformation fields
 *
 * Based on code originally by D.Kroon
 */

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

/* Convert 2D/3D matrix index to 1D index */
__inline mwIndex sub2ind3d(mwIndex y, mwIndex x, mwIndex z, mwIndex nrows, mwIndex ncols) { return y + x*nrows + z*nrows*ncols; }
__inline mwIndex sub2ind3d_alt(mwIndex y, mwIndex x, mwIndex z, mwIndex nrows, mwIndex nrc) { return y + x*nrows + z*nrc; }

#ifdef _WIN32
unsigned __stdcall ksplat(void *ThreadArgsV) {
#else
void *ksplat(void *ThreadArgsV) {
#endif
    int khw, khh, khd;      // kernel half-width/height/depth
    int iHx, iHy, iHz;
    mwIndex index, t;
    mwSize ncoords, ncoordsxy, Hsizexy;
    int ncoordsxyi, iH, colskip, planeskip, Bsizeyi;
    double dx, dy, dz;
    double fracs[8], pct;
    double xhf, yhf, zhf, xyhf, xzhf, yzhf;
    double Tlocalx, Tlocaly, Tlocalz;
    
    /* X,Y coordinates of current pixel */
    int x0, x1, y0, y1, z0, z1;  // MUST BE SIGNED!
    int x, y, z, tx, ty, tz, tx0, tx1, ty0, ty1, tz0, tz1;   // MUST BE SIGNED!
    
    ksplat_threadargs *ThreadArgs=(ksplat_threadargs *) ThreadArgsV;
    
    double *Bx, *By, *Bz, *H, thread_width, block_jump;
    //volatile double *totalweight, *Fx, *Fy;
    double *totalweight, *Fx, *Fy, *Fz;
    size_t Bsizex, Bsizey, Bsizez, Hsizex, Hsizey, Hsizez;
    size_t ThreadID, Nthreads, ThreadStage, ThreadXstart, ThreadXend;
    
    Bx=ThreadArgs->Bx;
    By=ThreadArgs->By;
    Bz=ThreadArgs->Bz;
    H=ThreadArgs->H;
    Fx=ThreadArgs->Fx;
    Fy=ThreadArgs->Fy;
    Fz=ThreadArgs->Fz;
    Bsizex=ThreadArgs->Bsizex;
    Bsizey=ThreadArgs->Bsizey;
    Bsizez=ThreadArgs->Bsizez;
    Hsizex=ThreadArgs->Hsizex;
    Hsizey=ThreadArgs->Hsizey;
    Hsizez=ThreadArgs->Hsizez;
    ThreadID=ThreadArgs->ThreadID;
    ThreadStage=ThreadArgs->ThreadStage; // 0 or 1
    Nthreads=ThreadArgs->Nthreads;
    totalweight=ThreadArgs->totalweight;
    
    Bsizeyi=(int) Bsizey;
// 	Hsizeyi=(int) Hsizey;
    
    thread_width=((double) Bsizex)/((double) Nthreads*2.0);
    block_jump=(double) (2.0*ThreadID+ThreadStage);
    
    ThreadXstart=(size_t) floor(thread_width*block_jump);
    ThreadXend=(size_t) floor(thread_width*(block_jump+1.));
    
    /* Create array to count number of kernels added  */
    ncoordsxy=Bsizex*Bsizey;
    ncoordsxyi=(int) ncoordsxy;
    ncoords=Bsizex*Bsizey*Bsizez;
    
    /* Gaussian kernel half-width/height */
    Hsizexy=Hsizex*Hsizey;
    khw=(mwSize) floor((float) Hsizex/2);
    khh=(mwSize) floor((float) Hsizey/2);
    khd=(mwSize) floor((float) Hsizez/2);
    
    /*  Loop through all image pixel coordinates */
    for (z=0; z<Bsizez; z++) {
        planeskip=z*ncoordsxyi;
        for (x=ThreadXstart; x<ThreadXend; x++) {
            colskip=x*Bsizeyi+planeskip;
            for (y=0; y<Bsizey; y++) {
                //index=sub2ind3d(y, x, z, Bsizey, Bsizex);
                //index=sub2ind3d_alt(y, x, z, Bsizey, ncoordsxy);
                index=y+colskip;
                dx=Bx[index];
                dy=By[index];
                dz=Bz[index];
                //if ((fabs(dx)<0.05) && (fabs(dy)<0.05) && (fabs(dz)<0.05)) continue;
                //if ((fabs(dx)+fabs(dz)+fabs(dz))<0.05) continue;
                Tlocalx=x-dx;
                Tlocaly=y-dy;
                Tlocalz=z-dz;
                
                /* Determine the coordinates of the pixel(s) which will be come the current pixel */
                /* (using linear interpolation) */
                x0=(int) floor(Tlocalx);
                y0=(int) floor(Tlocaly);
                z0=(int) floor(Tlocalz);
                //if ((x0<0) || (y0<0) || (z0<0)) continue;
                x1=x0+1;
                y1=y0+1;
                z1=z0+1;
                
                /* Linear interpolation constants (percentages) */
                xhf=Tlocalx-floor(Tlocalx);
                yhf=Tlocaly-floor(Tlocaly);
                zhf=Tlocalz-floor(Tlocalz);
                xyhf=xhf*yhf; xzhf=xhf*zhf; yzhf=yhf*zhf;
                
                fracs[7]=xyhf*zhf;                  // xyz
                fracs[6]=xyhf-fracs[7];             // xy - xyz
                fracs[5]=xzhf-fracs[7];             // xz - xyz
                fracs[4]=xhf-xyhf-fracs[5];         // x - xy - xz + xyz
                //fracs[4]=xhf-xyhf-xzhf+fracs[7];
                fracs[3]=yzhf-fracs[7];             // yz - xyz
                fracs[2]=yhf-xyhf-fracs[6];         // y - xy - yz + xyz
                //fracs[2]=yhf-xyhf-yzhf+fracs[7];
                fracs[1]=zhf-yzhf-fracs[5];         // z - yz - xz + xyz
                //fracs[1]=zhf-yzhf-xzhf+fracs[7];
                fracs[0]=1-xhf-yhf+xyhf-fracs[1];   // 1-x-y-z+xy+yz+xz-xyz
                //fracs[0]=1+fracs[5]-xhf-yhf-zhf+xyhf+yzhf;
                
//                 fracs[0]=(1-xhf) * (1-yhf) * (1-zhf);
//                 fracs[1]=(1-xhf) * (1-yhf) * zhf;
//                 fracs[2]=(1-xhf) * yhf * (1-zhf);
//                 fracs[3]=(1-xhf) * yhf * zhf;
//                 fracs[4]=xhf * (1-yhf) * (1-zhf);
//                 fracs[5]=xhf * (1-yhf) * zhf;
//                 fracs[6]=xhf * yhf * (1-zhf);
//                 fracs[7]=xhf * yhf * zhf;
                
                /*  Loop through the whole kernel */
                x0-=khw; x1-=khw;
                y0-=khh; y1-=khh;
                z0-=khd; z1-=khd;
                for (iHz=0, iH=0; iHz<Hsizez; iHz++) {
                    tz0=z0+iHz; tz1=z1+iHz;
                    for (iHx=0; iHx<Hsizex; iHx++) {
                        tx0=x0+iHx; tx1=x1+iHx;
                        for (iHy=0; iHy<Hsizey; iHy++, iH++) {
                            /*  Process all 8 neighbors of current kernel voxel */
                            //index=sub2ind3d(iHy, iHx, iHz, Hsizey, Hsizex);
                            //indexH=sub2ind3d_alt(iHy, iHx, iHz, Hsizey, Hsizexy);
                            if (H[iH]<0.0001) continue;
                            
                            ty0=y0+iHy; ty1=y1+iHy;
                            for(t=0; t<8; t++) {
                                if(t==0)      {tx=tx0; ty=ty0; tz=tz0;}
                                else if(t==1) {tx=tx0; ty=ty1; tz=tz0;}
                                else if(t==2) {tx=tx1; ty=ty0; tz=tz0;}
                                else if(t==3) {tx=tx1; ty=ty1; tz=tz0;}
                                else if(t==4) {tx=tx0; ty=ty0; tz=tz1;}
                                else if(t==5) {tx=tx0; ty=ty1; tz=tz1;}
                                else if(t==6) {tx=tx1; ty=ty0; tz=tz1;}
                                else          {tx=tx1; ty=ty1; tz=tz1;}
                                
                                if ((tx>=0)&&(ty>=0)&&(tz>=0)&&(tx<Bsizex)&&(ty<Bsizey)&&(tz<Bsizez)) {
                                    pct=fracs[t] * H[iH];
                                    if (pct<0.0001) continue;
                                    //index=sub2ind3d(ty, tx, tz, Bsizey, Bsizex);
                                    index=sub2ind3d_alt(ty, tx, tz, Bsizey, ncoordsxy);
                                    Fx[index]-=dx*pct;
                                    Fy[index]-=dy*pct;
                                    Fz[index]-=dz*pct;
                                    totalweight[index]+=pct;
                                }
                            }
                        }
                    }
                }
                // end kernel loop (iHz, iHx, iHz)
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

/* The matlab mex function */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    
    /* MATLAB Input and output variables */
    double *Fx, *Fy, *Fz, *Bx, *By, *Bz, *H, *totalweight;
    
    /* Deformation field */
    mwSize Bsizex, Bsizey, Bsizez;
    const mwSize *Bdims;
    size_t ncoords, ThreadStage, i, nmaxthreads;
    
    /* Kernel */
    mwSize Hsizex, Hsizey, Hsizez;
    const mwSize *Hdims;
    
    if (nrhs>5 || nrhs<4)
        mexErrMsgTxt("Three inputs are required:\n\t[Fx,Fy,Fz]=ksplat3mexmt(Bx,By,Bz,H,[nmaxthreads]);");
    if (nlhs!=3)
        mexErrMsgTxt("Two outputs are required:\n\t[Fx,Fy,Fz]=ksplat3mexmt(Bx,By,Bz,H,[nmaxthreads]);");
    
    /* Input pointers */
    Bx=mxGetPr(prhs[0]);
    By=mxGetPr(prhs[1]);
    Bz=mxGetPr(prhs[2]);
    H=mxGetPr(prhs[3]);
    
    /* Kernel dims */
    Hdims = mxGetDimensions(prhs[3]);
    Hsizey = Hdims[0]; Hsizex = Hdims[1]; Hsizez = Hdims[2];
    
    /* Deformation field dims */
    Bdims = mxGetDimensions(prhs[0]);
    ncoords=(size_t) mxGetNumberOfElements(prhs[0]);
    Bsizey = Bdims[0]; Bsizex = Bdims[1]; Bsizez = Bdims[2];
    
    if (nrhs>=5)
        nmaxthreads=(size_t) mxGetScalar(prhs[4]);
	else
        nmaxthreads=1024;
    
    /* Allocate for output deformation fields (Fx,Fy,Fz) */
    plhs[0] = mxCreateNumericArray(3, Bdims, mxDOUBLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericArray(3, Bdims, mxDOUBLE_CLASS, mxREAL);
    plhs[2] = mxCreateNumericArray(3, Bdims, mxDOUBLE_CLASS, mxREAL);
    Fx = mxGetPr(plhs[0]);
    Fy = mxGetPr(plhs[1]);
    Fz = mxGetPr(plhs[2]);
    
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
    if (Nthreads>nmaxthreads) Nthreads=nmaxthreads; // input thread limit
    
    while (((double) floor(Bsizex/(2.*Nthreads))) < ((double) ceil(Hsizex/2.))) --Nthreads;
    if (nrhs==5 && Nthreads<nmaxthreads) mexPrintf("Using %d threads.\n", Nthreads);
    
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
            ThreadArgsi->Bz=Bz;
            ThreadArgsi->H=H;
            ThreadArgsi->Fx=Fx;
            ThreadArgsi->Fy=Fy;
            ThreadArgsi->Fz=Fz;
            ThreadArgsi->Bsizex=Bsizex;
            ThreadArgsi->Bsizey=Bsizey;
            ThreadArgsi->Bsizez=Bsizez;
            ThreadArgsi->Hsizex=Hsizex;
            ThreadArgsi->Hsizey=Hsizey;
            ThreadArgsi->Hsizez=Hsizez;
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
    
    for (i=0; i<ncoords; i++) {
        //Fx[i]/=(totalweight[i]+0.00000001);
        //Fy[i]/=(totalweight[i]+0.00000001);
        //Fz[i]/=(totalweight[i]+0.00000001);
        if (totalweight[i]>0.0000001) {
            Fx[i]/=totalweight[i];
            Fy[i]/=totalweight[i];
            Fz[i]/=totalweight[i];
        } else { // when totalweight is small, so are Fx and Fy
            Fx[i]=0;
            Fy[i]=0;
            Fz[i]=0;
        }
    }
    
    free(totalweight);
    
}
