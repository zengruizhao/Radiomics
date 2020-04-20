#include "mex.h"
#include "math.h"
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

void ksplat(double* Bx, double* By, double* Bz, double* H,
        double* Fx, double* Fy, double* Fz,
        mwSize Bsizex, mwSize Bsizey, mwSize Bsizez,
        mwSize Hsizex, mwSize Hsizey, mwSize Hsizez) {
    
    double *totalweight;
    int khw, khh, khd;      // kernel half-width/height/depth
    int iHx, iHy, iHz;
    mwIndex index, indexH, i, t;
    mwSize ncoords, ncoordsxy, Hsizexy;
    double dx, dy, dz;
    double fracs[8], pct;
    double xhf, yhf, zhf, xyhf, xzhf, yzhf;
    double Tlocalx, Tlocaly, Tlocalz;
    
    /* X,Y coordinates of current pixel */
    int x0, x1, y0, y1, z0, z1;  // MUST BE SIGNED!
    int x, y, z, tx, ty, tz, tx0, tx1, ty0, ty1, tz0, tz1;   // MUST BE SIGNED!
    
    /* Create array to count number of kernels added  */
    ncoordsxy=Bsizex*Bsizey;
    ncoords=Bsizex*Bsizey*Bsizez;
    totalweight = (double *) calloc(ncoords, sizeof(double));
    
    /* Gaussian kernel half-width/height */
    Hsizexy=Hsizex*Hsizey;
    khw=(mwSize) floor((float) Hsizex/2);
    khh=(mwSize) floor((float) Hsizey/2);
    khd=(mwSize) floor((float) Hsizez/2);
    
    /*  Loop through all image pixel coordinates */
    for (z=0; z<Bsizez; z++) {
        for (x=0; x<Bsizex; x++) {
            for (y=0; y<Bsizey; y++) {
                //index=sub2ind3d(y, x, z, Bsizey, Bsizex);
                index=sub2ind3d_alt(y, x, z, Bsizey, ncoordsxy);
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
                for (iHz=0; iHz<Hsizez; iHz++) {
                    tz0=z0+iHz; tz1=z1+iHz;
                    for (iHx=0; iHx<Hsizex; iHx++) {
                        tx0=x0+iHx; tx1=x1+iHx;
                        for (iHy=0; iHy<Hsizey; iHy++) {
                            /*  Process all 8 neighbors of current kernel voxel */
                            //index=sub2ind3d(iHy, iHx, iHz, Hsizey, Hsizex);
                            indexH=sub2ind3d_alt(iHy, iHx, iHz, Hsizey, Hsizexy);
                            if (H[indexH]<0.0001) continue;
                            
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
                                    pct=fracs[t] * H[indexH];
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

/* The matlab mex function */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    
    /* MATLAB Input and output variables */
    double *Fx, *Fy, *Fz, *Bx, *By, *Bz, *H;
    
    /* Deformation field */
    mwSize Bsizex, Bsizey, Bsizez;
    const mwSize *Bdims;
    size_t ncoords;
    
    /* Kernel */
    mwSize Hsizex, Hsizey, Hsizez;
    const mwSize *Hdims;
    
    if(nrhs!=4)
        mexErrMsgTxt("Three inputs are required:\n\t[Fx,Fy,Fz]=ksplat3mex(Bx,By,Bz,H);");
    else if(nlhs!=3)
        mexErrMsgTxt("Two outputs are required:\n\t[Fx,Fy,Fz]=ksplat3mex(Bx,By,Bz,H);");
    
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
    
    /* Allocate for output deformation fields (Fx,Fy,Fz) */
    plhs[0] = mxCreateNumericArray(3, Bdims, mxDOUBLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericArray(3, Bdims, mxDOUBLE_CLASS, mxREAL);
    plhs[2] = mxCreateNumericArray(3, Bdims, mxDOUBLE_CLASS, mxREAL);
    Fx = mxGetPr(plhs[0]);
    Fy = mxGetPr(plhs[1]);
    Fz = mxGetPr(plhs[2]);
    
    // Do it!
    ksplat(Bx,By,Bz,H,Fx,Fy,Fz,Bsizex,Bsizey,Bsizez,Hsizex,Hsizey,Hsizez);
    
}
