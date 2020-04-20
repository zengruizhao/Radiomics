/* 
 * Px_displaced, Py_displaced, Bproducts, localinds --> tlocalx, tlocaly
 */

#include <math.h>
#include "mex.h"

void bprod2tlocal(double* bprods, double* Pxdispl, double* Pydispl, double* tlis,
        double* tlocalx, double* tlocaly, size_t npts) {
    
    size_t m, n, i, bpind, tlival;
    
    for (m=0; m<4; m++) {
        for (n=0; n<4; n++) {
            for (i=0; i<npts; i++) {
                bpind=i+m*npts+n*4*npts;
                tlival=(size_t) tlis[bpind]-1;
                tlocalx[i]+=(bprods[bpind]*Pxdispl[tlival]);
                tlocaly[i]+=(bprods[bpind]*Pydispl[tlival]);
            }
        }
    }
    
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] ) {
    double *bprods, *Pxdispl, *Pydispl, *tlis, *tlocalx, *tlocaly;
    
    size_t i, npts, mdimsize, ndimsize, ncps;
    
    mwSize bprodndims, pxpyndims, tlocaldims[2];
    const mwSize *bproddims, *pxpydims;
    
    /* Check for input errors */
    if (nlhs>2 || nrhs!=4)
        mexErrMsgTxt("Wrong number of output parameters, usage:  [tlocalx,tlocaly] = bprodstotlocals2mex(Bproducts,Pxdispl,Pydispl,tlis)");
    
    if (!mxIsDouble(prhs[0]) || !mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]) || !mxIsDouble(prhs[3]))
        mexErrMsgTxt("bprodstotlocals2mex: Input arguments must be double.");
    
    if ((mxGetNumberOfDimensions(prhs[1]) != mxGetNumberOfDimensions(prhs[2])) ||
            (mxGetNumberOfElements(prhs[1]) != mxGetNumberOfElements(prhs[2]))) mexErrMsgTxt("Input parameters Pxdispl and Pydispl must have the same size");
    if ((mxGetNumberOfDimensions(prhs[0]) != mxGetNumberOfDimensions(prhs[3])) ||
            (mxGetNumberOfElements(prhs[0]) != mxGetNumberOfElements(prhs[3]))) mexErrMsgTxt("Input parameters Bproducts and tlis must have the same size");
    
    /* Get the sizes of each input argument */
    bprodndims = mxGetNumberOfDimensions(prhs[0]);
    if (bprodndims!=3) mexErrMsgTxt("Bproducts and tlis must be npts-by-4-by-4.");
    bproddims = mxGetDimensions(prhs[0]);
    npts = bproddims[0];
    mdimsize = bproddims[1]; ndimsize = bproddims[2];
    if ((mdimsize!=4) || (ndimsize!=4)) mexErrMsgTxt("Bproducts and tlis must be npts-by-4-by-4.");
    
    pxpyndims = mxGetNumberOfDimensions(prhs[1]);
    pxpydims = mxGetDimensions(prhs[1]);
    ncps=mxGetNumberOfElements(prhs[1]);
    //if (pxpyndims!=2) 
    
    // Input arguments pointers
    bprods = mxGetPr(prhs[0]);
    Pxdispl = mxGetPr(prhs[1]);
    Pydispl = mxGetPr(prhs[2]);
    tlis = mxGetPr(prhs[3]);
    for (i=0; i<(npts*16); i++)
        if ((tlis[i]>ncps) || (tlis[i]<1)) mexErrMsgTxt("tlis out of range for Pxdispl, Pydispl.");
    
    // Output arguments
    tlocaldims[0]=npts; tlocaldims[1]=1;
    plhs[0] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    plhs[1] = mxCreateNumericArray(2, tlocaldims, mxDOUBLE_CLASS, mxREAL);
    tlocalx = mxGetPr(plhs[0]);
    tlocaly = mxGetPr(plhs[1]);
    
    /* Do the actual computations in a subroutine */
    bprod2tlocal(bprods,Pxdispl,Pydispl,tlis,tlocalx,tlocaly,npts);
}
