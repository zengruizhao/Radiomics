/* 
 * Px_displaced, Py_displaced, Pz_displaced, Bproducts, localinds --> tlocalx, tlocaly, tlocalz
 */

#include <math.h>
#include "mex.h"

void bprod2tlocal(float* bprods, double* Pxdispl, double* Pydispl, double* Pzdispl, long* tlis,
        double* tlocalx, double* tlocaly, double* tlocalz, size_t npts) {
    
    size_t bpind, tlival;
    long long i, dimskip;
    int m, n, l;
    
//     #pragma omp parallel for shared(tlocalx, tlocaly, tlocalz) private(l, m, n, i, bpind, tlival, dimskip)
    for (l=0; l<4; l++) {
        for (m=0; m<4; m++) {
            for (n=0; n<4; n++) {
                dimskip=npts*(m + n*4 + l*16);
                // for all points in input bprods and tlis
                #pragma omp parallel for shared(tlocalx, tlocaly, tlocalz) private(i, bpind, tlival)
                for (i=0; i<npts; i++) {
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
    
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    double *Pxdispl, *Pydispl, *Pzdispl, *tlocalx, *tlocaly, *tlocalz;
    long *tlis;
    float *bprods;
    
    size_t i, npts, mdimsize, ndimsize, ldimsize, ncps;
    
    mwSize bprodndims, pxpypzndims, tlocaldims[3];
    const mwSize *bproddims, *pxpypzdims;
    
    /* Check for input errors */
    if (nlhs>3 || nrhs!=5)
        mexErrMsgTxt("Wrong number of output parameters, usage:  [tlocalx,tlocaly,tlocalz] = bprodstotlocals3mex(Bproducts,Pxdispl,Pydispl,Pzdispl,tlis)");
    
    if (!mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]) || !mxIsDouble(prhs[3]))
        mexErrMsgTxt("bprodstotlocals3mex: Input arguments must be double.");
    if (!mxIsSingle(prhs[0]))
        mexErrMsgTxt("bprodstotlocals3mex: Bproducts must be single.");
    if (!mxIsClass(prhs[4],"uint32"))
        mexErrMsgTxt("bprodstotlocals3mex: tlis must be uint32.");
    
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
    
    // Output arguments
    tlocaldims[0]=npts; tlocaldims[1]=1;
    plhs[0] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    plhs[2] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    tlocalx = mxGetPr(plhs[0]);
    tlocaly = mxGetPr(plhs[1]);
    tlocalz = mxGetPr(plhs[2]);
    
    /* Do the actual computations in a subroutine */
    bprod2tlocal(bprods,Pxdispl,Pydispl,Pzdispl,tlis,tlocalx,tlocaly,tlocalz,npts);
}
