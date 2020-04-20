/* 
 * Isize,Px,Py,Px -> Bproducts,tlis,Bms,Bnt,Blu,s,t,u,i,j,k
 *
 *mex -v -largeArrayDims COMPFLAGS="$COMPFLAGS /openmp" imbeziers3mex.cpp
 *
 */

#include <algorithm>
#include <math.h>
#include "mex.h"

/* Convert 2D/3D matrix index to 1D index */
__inline mwIndex sub2ind3d(mwIndex y, mwIndex x, mwIndex z, mwIndex nrows, mwIndex ncols) { return y + x*nrows + z*nrows*ncols; }
//__inline mwIndex sub2ind3d_alt(mwIndex y, mwIndex x, mwIndex z, mwIndex nrows, mwIndex nrc) { return y + x*nrows + z*nrc; }

__inline double B0(double t) { return ((pow(1-t,3))/6); };
__inline double B1(double t) { return ((3*t*t*t - 6*t*t + 4)/6); };
__inline double B2(double t) { return ((-3*t*t*t + 3*t*t +3*t + 1)/6); };
__inline double B3(double t) { return ((t*t*t)/6); };

void imbeziers3(double *Isize, double *Px, double *Py, double *Pz,
        float* bprods, long *tlis, double* Bms, double* Bnt, double* Blu,
        double* s, double* t, double* u, long *i, long *j, long *k, const mwSize *griddims) {
    
    size_t dimskip, Bloc1, Bloc2, Bloc3;
    int m, n, l;
    long long ii;
    float xcpstart, ycpstart, zcpstart, spx, spy, spz;
    long x,y,z;
    mwSize npts, lskip, mskip, nskip, ngridrows, ngridrc, nrows, ncols, nplanes;
    
    nrows=(mwSize) Isize[0];
    ncols=(mwSize) Isize[1];
    nplanes=(mwSize) Isize[2];
    npts=nrows*ncols*nplanes;
    //npts=(mwSize) (Isize[0]*Isize[1]*Isize[2]);
    
    ngridrows=(mwSize) griddims[0];
    ngridrc=(mwSize) (griddims[0]*griddims[1]);
    
    xcpstart=(float) Px[0]-1; // -1 to account for x,y,z that must start at 0 below
    ycpstart=(float) Py[0]-1;
    zcpstart=(float) Pz[0]-1;
    
    spx=(float) (Px[sub2ind3d(0,1,0,griddims[0],griddims[1])]-Px[0]);
    spy=(float) (Py[sub2ind3d(1,0,0,griddims[0],griddims[1])]-Py[0]);
    spz=(float) (Pz[sub2ind3d(0,0,1,griddims[0],griddims[1])]-Pz[0]);
    
    double *svals = new double[ncols];
    double *tvals = new double[nrows];
    double *uvals = new double[nplanes];
    long *ivals = new long[ncols];
    long *jvals = new long[nrows];
    long *kvals = new long[nplanes];
    double *Bx0vals = new double[ncols];
    double *Bx1vals = new double[ncols];
    double *Bx2vals = new double[ncols];
    double *Bx3vals = new double[ncols];
    double *By0vals = new double[nrows];
    double *By1vals = new double[nrows];
    double *By2vals = new double[nrows];
    double *By3vals = new double[nrows];
    double *Bz0vals = new double[nplanes];
    double *Bz1vals = new double[nplanes];
    double *Bz2vals = new double[nplanes];
    double *Bz3vals = new double[nplanes];
    
    for (x=0; x<ncols; x++) {
//         isegment = Px[sub2ind3d(0,1,0,griddims[0],griddims[1])]
        svals[x]=(double) (x-xcpstart)/spx;
        ivals[x]=(long) (floor(svals[x])+1.); // segment number, starting at 1
        svals[x]-=floor(svals[x]); // fraction (t or u)
        Bx0vals[x]=B0((double) svals[x]);
        Bx1vals[x]=B1((double) svals[x]);
        Bx2vals[x]=B2((double) svals[x]);
        Bx3vals[x]=B3((double) svals[x]);
    }
    
//     mexPrintf("%d\n",ivals[ncols-1]);
    
    for (y=0; y<nrows; y++) {
        tvals[y]=(double) (y-ycpstart)/spy;
        jvals[y]=(long) (floor(tvals[y])+1.);
        tvals[y]-=floor(tvals[y]);
        By0vals[y]=B0((double) tvals[y]);
        By1vals[y]=B1((double) tvals[y]);
        By2vals[y]=B2((double) tvals[y]);
        By3vals[y]=B3((double) tvals[y]);
    }
    
    for (z=0; z<nplanes; z++) {
        uvals[z]=(double) (z-zcpstart)/spz;
        kvals[z]=(long) (floor(uvals[z])+1.);
        uvals[z]-=floor(uvals[z]);
        Bz0vals[z]=B0((double) uvals[z]);
        Bz1vals[z]=B1((double) uvals[z]);
        Bz2vals[z]=B2((double) uvals[z]);
        Bz3vals[z]=B3((double) uvals[z]);
    }
    
    // reorganize data -- could probably get rid of this if we don't want all outputs
    for (ii=0, z=0; z<nplanes; z++) {
        for (x=0; x<ncols; x++) {
            for (y=0; y<nrows; y++, ii++) {
                s[ii]=svals[x];
                t[ii]=tvals[y];
                u[ii]=uvals[z];
                
                //Bloc0=ii; 
                Bloc1=ii+npts; Bloc2=Bloc1+npts; Bloc3=Bloc2+npts;
                Bnt[ii]=By0vals[y];
                Bnt[Bloc1]=By1vals[y];
                Bnt[Bloc2]=By2vals[y];
                Bnt[Bloc3]=By3vals[y];
                Bms[ii]=Bx0vals[x];
                Bms[Bloc1]=Bx1vals[x];
                Bms[Bloc2]=Bx2vals[x];
                Bms[Bloc3]=Bx3vals[x];
                Blu[ii]=Bz0vals[z];
                Blu[Bloc1]=Bz1vals[z];
                Blu[Bloc2]=Bz2vals[z];
                Blu[Bloc3]=Bz3vals[z];
                
                i[ii]=ivals[x];
                j[ii]=jvals[y];
                k[ii]=kvals[z];
            }
        }
    }
    
    //#pragma omp parallel for shared(bprods, tlis, npts, ngridrows, ngridrc) private(l, m, n, lskip, mskip, nskip, dimskip, bpind)
    for (l=0; l<4; l++) {
        lskip=l*npts;
        for (m=0; m<4; m++) {
            mskip=m*npts;
            for (n=0; n<4; n++) {
                nskip=n*npts;
                dimskip=mskip+4*nskip+16*lskip;
                //dimskip=npts*(m + n*4 + l*16);
                #pragma omp parallel for shared(bprods, tlis) private(ii)
//                 #pragma omp parallel for shared(bprods, tlis, npts, ngridrows, ngridrc, l, m, n, lskip, mskip, nskip, dimskip) private(ii, bpind)
                for (ii=0; ii<npts; ii++) {
                    //bpind=i + m*npts + n*4*npts + l*16*npts;
                    //bpind=i + npts*(m + n*4 + l*16);
//                     bpind=ii + dimskip;
                    bprods[ii + dimskip]=(float) (Bms[ii+mskip]*Bnt[ii+nskip]*Blu[ii+lskip]);
//                     bprodsum[ii]+=(float) (Bms[ii+mskip]*Bnt[ii+nskip]*Blu[ii+lskip]);
                    tlis[ii + dimskip]=(long) ((j[ii]+n-1) + (i[ii]+m-2)*ngridrows + (k[ii]+l-2)*ngridrc); // -1 on j vs. -2 on j + 1 for matlab
                }
            }
        }
    }
    
    delete[] svals;
    delete[] tvals;
    delete[] uvals;
    delete[] ivals;
    delete[] jvals;
    delete[] kvals;
    delete[] Bx0vals;
    delete[] Bx1vals;
    delete[] Bx2vals;
    delete[] Bx3vals;
    delete[] By0vals;
    delete[] By1vals;
    delete[] By2vals;
    delete[] By3vals;
    delete[] Bz0vals;
    delete[] Bz1vals;
    delete[] Bz2vals;
    delete[] Bz3vals;
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    double *Isize, *Px, *Py, *Pz, *Bms, *Bnt, *Blu, *s, *t, *u;
    long *tlis, *i, *j, *k;
    float *bprods;
    
    //size_t i, npts, mdimsize, ndimsize, ldimsize, ncps;
    size_t nIdims;
    
    mwSize npts, bproddims[4], bezdims[2], ijkdims[2], ngriddims, ngridels;
    const mwSize *griddims;
    
    /* Check for input errors */
    if (nlhs>11 || nrhs!=4)
        mexErrMsgTxt("Wrong number of I/O parameters, usage:  [Bprods,tlis,Bms,Bnt,Blu,s,t,u,i,j,k] = imbeziers3mex(Isize,Px,Py,Pz)");
    
    if (!mxIsDouble(prhs[0]) || !mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]) || !mxIsDouble(prhs[3]))
        mexErrMsgTxt("imbeziers3mex: Input arguments must be double.");
    
    if ((mxGetNumberOfDimensions(prhs[1]) != mxGetNumberOfDimensions(prhs[2])) ||
            (mxGetNumberOfElements(prhs[1]) != mxGetNumberOfElements(prhs[2])) ||
            (mxGetNumberOfDimensions(prhs[1]) != mxGetNumberOfDimensions(prhs[3])) ||
            (mxGetNumberOfElements(prhs[1]) != mxGetNumberOfElements(prhs[3]))) mexErrMsgTxt("Input parameters Px, Py and Pz must have the same size");
    
    /* Get the sizes of each input argument */
    nIdims=mxGetNumberOfElements(prhs[0]);
    if (nIdims!=3) mexErrMsgTxt("Image size must be 3D.");
    
    ngriddims = mxGetNumberOfDimensions(prhs[1]);
    if (ngriddims!=3) mexErrMsgTxt("Grids must be 3D.");
    
    griddims = mxGetDimensions(prhs[1]);
    if (griddims[0]<2 || griddims[1]<2 || griddims[2]<2) mexErrMsgTxt("Grids must actually be 3D.");
    
    ngridels = mxGetNumberOfElements(prhs[1]);
    
    // Input arguments pointers
    Isize = mxGetPr(prhs[0]);
    Px = mxGetPr(prhs[1]);
    Py = mxGetPr(prhs[2]);
    Pz = mxGetPr(prhs[3]);
    
    npts=(mwSize) (Isize[0]*Isize[1]*Isize[2]);
    
    double minxgridpt=*std::min_element(Px,Px+ngridels);
    double minygridpt=*std::min_element(Py,Py+ngridels);
    double minzgridpt=*std::min_element(Pz,Pz+ngridels);
    double maxxgridpt=*std::max_element(Px,Px+ngridels);
    double maxygridpt=*std::max_element(Py,Py+ngridels);
    double maxzgridpt=*std::max_element(Pz,Pz+ngridels);
    
    if (minxgridpt>1 || minygridpt>1 || minzgridpt>1 || maxxgridpt<Isize[1] || 
            maxygridpt<Isize[0] || maxzgridpt<Isize[2])
        mexErrMsgTxt("Grids Px, Py and/or Pz do not span Isize.");
    //else mexPrintf("OK grids.\n");
    
    // Output arguments
    bproddims[0]=npts; bproddims[1]=4; bproddims[2]=4; bproddims[3]=4;
    bezdims[0]=npts; bezdims[1]=4;
    ijkdims[0]=npts; ijkdims[1]=1;
    // Bproducts
    plhs[0] = mxCreateNumericArray(4, bproddims, mxSINGLE_CLASS, mxREAL);
    // tlis
    plhs[1] = mxCreateNumericArray(4, bproddims, mxUINT32_CLASS, mxREAL);
    // Bms,Bnt,Blu
    plhs[2] = mxCreateNumericArray(2, bezdims, mxDOUBLE_CLASS, mxREAL);
    plhs[3] = mxCreateNumericArray(2, bezdims, mxDOUBLE_CLASS, mxREAL);
    plhs[4] = mxCreateNumericArray(2, bezdims, mxDOUBLE_CLASS, mxREAL);
    // s,t,u
    plhs[5] = mxCreateNumericArray(2, ijkdims, mxDOUBLE_CLASS, mxREAL);
    plhs[6] = mxCreateNumericArray(2, ijkdims, mxDOUBLE_CLASS, mxREAL);
    plhs[7] = mxCreateNumericArray(2, ijkdims, mxDOUBLE_CLASS, mxREAL);
    // i,j,k
    plhs[8] = mxCreateNumericArray(2, ijkdims, mxUINT32_CLASS, mxREAL);
    plhs[9] = mxCreateNumericArray(2, ijkdims, mxUINT32_CLASS, mxREAL);
    plhs[10] = mxCreateNumericArray(2, ijkdims, mxUINT32_CLASS, mxREAL);
    
    bprods = (float *) mxGetData(plhs[0]);
    tlis = (long *) mxGetData(plhs[1]);
    Bms = mxGetPr(plhs[2]);
    Bnt = mxGetPr(plhs[3]);
    Blu = mxGetPr(plhs[4]);
    s = mxGetPr(plhs[5]);
    t = mxGetPr(plhs[6]);
    u = mxGetPr(plhs[7]);
    i = (long *) mxGetData(plhs[8]);
    j = (long *) mxGetData(plhs[9]);
    k = (long *) mxGetData(plhs[10]);
    
    /* Do the actual computations in a subroutine */
    imbeziers3(Isize,Px,Py,Pz,bprods,tlis,Bms,Bnt,Blu,s,t,u,i,j,k,griddims);
}
