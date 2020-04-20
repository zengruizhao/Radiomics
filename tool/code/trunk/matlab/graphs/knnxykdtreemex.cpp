#include <stdio.h>
#include <math.h>
#include "mex.h"
#include "knnkdtree.h"

/* knnxykdtreemex - Create a kNN graph efficiently using kd-trees.
 * Jon Chappelow
 */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    double gamma;
    unsigned int k;
    double *X, *Y, *knnsinx, *diststox;
    //size_t npts, ndims;
    mwSize nptsX, nptsY, ndims, dummyquerind;
    mwSize i, ii, j, ix, iy;
    const mwSize *Xdims, *Ydims;
    
    /* Check for proper number of arguments. */
    if (nrhs != 4) {
        mexErrMsgTxt("Three input arguments needed:\n\r[knnsinx, diststox] = knnxykdtreemex(X, Y, k, gamma)");
    }
    if (nlhs > 2) {
        mexErrMsgTxt("Three output arguments needed:\n\t[knnsinx, diststox] = knnxykdtreemex(X, Y, k, gamma)");
    }
    
    if (!mxIsDouble(prhs[0]) || !mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]) || !mxIsDouble(prhs[3]))
        mexErrMsgTxt("Inputs must be double precision.");
    
    /* Inputs */
    X = mxGetPr(prhs[0]);
    Xdims=mxGetDimensions(prhs[0]);
    
    Y = mxGetPr(prhs[1]);
    Ydims=mxGetDimensions(prhs[1]);
    
    k = (unsigned int) mxGetScalar(prhs[2]);
    //k = k + 1;  /* Because first neighbor is the point itself */
    gamma = mxGetScalar(prhs[3]);
    
    if (k > Xdims[0]) mexErrMsgTxt("Number of NN cant be larger than total number of points in X.");
    if (Xdims[1]!=Ydims[1]) mexErrMsgTxt("Number of dimensions (columns/2nd dim.) must be same for X and Y.");
    
    nptsX=Xdims[0]; ndims=Xdims[1];
    nptsY=Ydims[0];
    
    /* Outputs */
    plhs[0] = mxCreateDoubleMatrix((mwSize) k, nptsY, mxREAL);
    knnsinx = mxGetPr(plhs[0]);
    plhs[1] = mxCreateDoubleMatrix((mwSize) k, nptsY, mxREAL);
    diststox = mxGetPr(plhs[1]);
    
    //mexPrintf("npts: %d, ndims: %d, k: %d\n", Xdims[0], Xdims[1],k);
    
    //knnkdtree(X, npts, ndims, k, len, Graph, dists, gamma);
    //knnkdtree(X, Xdims[0], Xdims[1], k, len, Graph, dists, gamma);
    
    double *Ypt, **Xm, **Ym;
    Ypt=(double *) mxMalloc(ndims*sizeof(double));
    Xm = (double **) mxMalloc(nptsX*sizeof(double *));
    Ym = (double **) mxMalloc(nptsY*sizeof(double *));
    for (ii = 0; ii < nptsX; ii++) Xm[ii] = (double *) mxMalloc(ndims*sizeof(double));
    for (ii = 0; ii < nptsY; ii++) Ym[ii] = (double *) mxMalloc(ndims*sizeof(double));
    
    for (j = 0, ix = 0, iy=0; j < ndims; j++) {
        for (ii = 0; ii < nptsX; ii++, ix++) Xm[ii][j] = X[ix];
        for (ii = 0; ii < nptsY; ii++, iy++) Ym[ii][j] = Y[iy];
    }
    
    //mexPrintf("coppied data into 2D arrays.\n");
    
    optkdNode *kdtreeX;
    size_t *permX, *kneighbindsX;
    
    permX = (size_t *) mxMalloc(nptsX*sizeof(size_t));
    for (j=0; j < nptsX; j++) permX[j]=j;
    
    kdtreeX=BuildkdTree(Xm, 0, nptsX-1, ndims, permX);
    
    //mexPrintf("kd-tree of X built.\n");
    
    double *nndist = (double *) mxMalloc((k+1)*sizeof(double));
    
    // kNNs in X for each point in Y
    for (j=0; j < k+1; j++) nndist[j] = HUGEDOUBLE;
    
    dummyquerind=nptsX+1;
    
    for (i = 0, ii = 0; i < nptsY; i++) {
        // Grab query point i (each dimension j)
        for (j = 0; j < ndims; j++) Ypt[j]=Ym[i][j];
        
        // Find the kNNs for the query point
        kneighbindsX = kdOptNNQuery(Xm, ndims, Ypt, k, kdtreeX, dummyquerind, nndist, permX); // nptsX+1 deals with rnnEuclidean
        
        for (j = k; j >= 1; j--, ii++) /* kneighbindsX[0] is a dummy */ {
            diststox[ii]=sqrt(nndist[j]);
            //*lenY += pow(distsYtoX[ii], gamma);
            knnsinx[ii] = (double) kneighbindsX[j] + 1;
        }
    }
    
    //mexPrintf("kNNs in X for each point in Y calculated.\n");
    
}
