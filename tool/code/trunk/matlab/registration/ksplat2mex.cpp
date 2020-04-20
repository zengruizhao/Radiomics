#include "mex.h"
#include "math.h"
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

void ksplat(double* Bx, double* By, double* H, double* Fx, double* Fy,
        mwSize Bsizex, mwSize Bsizey, mwSize Hsizex, mwSize Hsizey) {
    
    double *totalweight;
    int khw, khh;
    double dx, dy;
    int index, i, t, iHx, iHy, ncoords;
    int x0, x1, y0, y1;
    double fracs[4], pct;
    double xhighfrac, yhighfrac;
    double Tlocalx, Tlocaly;
    
    /* X,Y coordinates of current pixel */
    int x, y, tx, ty, tx0, tx1, ty0, ty1;
    //int tx01[2], ty01[2], ix, iy;
    
    /* Measure total weight of any and all pixels mapped to each pixel */
    ncoords=Bsizex*Bsizey;
    totalweight = (double *) calloc(ncoords, sizeof(double));
    
    /* Gaussian kernel half-width/height */
    khw=(int) floor((float) Hsizex/2);
    khh=(int) floor((float) Hsizey/2);
    
    /*  Loop through all image pixel coordinates (of destination) -- find their source via Bx,By (the fractional destination) */
    for (x=0; x<Bsizex; x++) {
        for (y=0; y<Bsizey; y++) {
            index=sub2ind2d(y, x, Bsizey);
            dx=Bx[index];
            dy=By[index];
            //if ((fabs(dx)<0.05) && (fabs(dy)<0.05)) continue;
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
            for (iHx=0; iHx<Hsizex; iHx++) {
                tx0=x0+iHx; tx1=x1+iHx;
                for (iHy=0; iHy<Hsizey; iHy++) {
                    /* Process all 4 neighbors of current kernel pixel */
                    if (H[sub2ind2d(iHy, iHx, Hsizey)]<0.0001) continue;
                    
                    //tx0=x0+iHx; tx1=x1+iHx;
                    ty0=y0+iHy; ty1=y1+iHy;
                    for(t=0; t<4; t++) {
                        
                        if(t==0) {tx=tx0; ty=ty0;}
                        else if(t==1) {tx=tx0; ty=ty1;}
                        else if(t==2) {tx=tx1; ty=ty0;}
                        else {tx=tx1; ty=ty1;}
                        
                        if ((tx>=0)&&(ty>=0)&&(tx<Bsizex)&&(ty<Bsizey)) {
                            pct=fracs[t] * H[sub2ind2d(iHy, iHx, Hsizey)];
                            if (pct<0.0001) continue;
                            index=sub2ind2d(ty, tx, Bsizey);
                            Fx[index]-=dx*pct;
                            Fy[index]-=dy*pct;
                            totalweight[index]+=pct;
                        }
                    }
                    
                    /* Alternate loop */
                    /*
                    tx01[0]=x0+iHx; tx01[1]=x1+iHx;
                    ty01[0]=y0+iHy; ty01[1]=y1+iHy;
                    for (ix=0, t=0; ix<2; ix++) {
                        for (iy=0; iy<2; iy++, t++) {
                            if((tx01[ix]>=0)&&(ty01[iy]>=0)&&(tx01[ix]<Bsizex)&&(ty01[iy]<Bsizey)) {
                                pct=fracs[t]*H[sub2ind2d(iHy, iHx, Hsizey)];
                                index=sub2ind2d(ty01[iy], tx01[ix], Bsizey);
                                Fx[index]-=dx*pct;
                                Fy[index]-=dy*pct;
                                totalweight[index]+=pct;
                            }
                        }
                    }
                     */
                }
            }
        }
    }
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


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    /* MATLAB Input and output variables */
    double *Fx, *Fy, *Bx, *By, *H;
    
    /* Deformation field */
    mwSize Bsizex, Bsizey;
    const mwSize *Bdims;
    size_t ncoords;
    
    /* Kernel */
    mwSize Hsizex, Hsizey;
    const mwSize *Hdims;
    
    if(nrhs!=3)
        mexErrMsgTxt("Three inputs are required:\n\t[Fx,Fy]=ksplat2mex(Bx,By,H);");
    else if(nlhs!=2)
        mexErrMsgTxt("Two outputs are required:\n\t[Fx,Fy]=ksplat2mex(Bx,By,H);");
    
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
    
    // Do it!
    ksplat(Bx,By,H,Fx,Fy,Bsizex,Bsizey,Hsizex,Hsizey);
    
}
