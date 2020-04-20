#include <stdio.h>
#include <math.h>
#include "mex.h"
#include "knnkdtree.h"

/* knnkdtreemex - Create a kNN graph efficiently using kd-trees.
 * Jon Chappelow
 */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  double gamma;
  unsigned int k;
  double *len, *X, *Graph, *dists;
  //size_t npts, ndims;
  mwSize npts, ndims;
  const mwSize *Xdims;

  /* Check for proper number of arguments. */
  if (nrhs != 3) {
    mexErrMsgTxt("Three input arguments needed:\n\r[len, kNNG, dists] = knnkdtreemex(X, k, gamma)");
  }
  if (nlhs > 3) {
    mexErrMsgTxt("Three output arguments needed:\n\t[len, kNNG, dists] = knnkdtreemex(X, k, gamma)");
  }
  
  if (!mxIsDouble(prhs[0]) || !mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]))
        mexErrMsgTxt("Inputs must be double precision.");

  /* Inputs */
  X = mxGetPr(prhs[0]);
  //npts = mxGetScalar(prhs[1]);
  //ndims = mxGetScalar(prhs[2]);
  //npts = mxGetM(prhs[0]);
  //ndims = mxGetN(prhs[0]);
  Xdims=mxGetDimensions(prhs[0]);
  k = (unsigned int) mxGetScalar(prhs[1]); // prhs[3]
  //k = k + 1;  /* Because first neighbor is the point itself */
  gamma = mxGetScalar(prhs[2]); // prhs[4]
  
  if (k > Xdims[0]) 
    mexErrMsgTxt("Number of NN cant be larger than total number of points.");
  
  /* Outputs */
  plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
  len = mxGetPr(plhs[0]);
  //plhs[1] = mxCreateDoubleMatrix((mwSize) npts*(mwSize) k, 1, mxREAL);
  //plhs[1] = mxCreateDoubleMatrix(Xdims[0]*(mwSize) k, 1, mxREAL);
  plhs[1] = mxCreateDoubleMatrix((mwSize) k, Xdims[0], mxREAL);
  Graph = mxGetPr(plhs[1]);
  //plhs[2] = mxCreateDoubleMatrix((mwSize) npts*(mwSize) k, 1, mxREAL);
  //plhs[2] = mxCreateDoubleMatrix(Xdims[0]*(mwSize) k, 1, mxREAL);
  plhs[2] = mxCreateDoubleMatrix((mwSize) k, Xdims[0], mxREAL);
  dists = mxGetPr(plhs[2]);
  
  //mexPrintf("npts: %d, ndims: %d, k: %d\n", Xdims[0], Xdims[1],k);
  
  npts=Xdims[0]; ndims=Xdims[1];
  knnkdtree(X, npts, ndims, k, len, Graph, dists, gamma);
  //knnkdtree(X, Xdims[0], Xdims[1], k, len, Graph, dists, gamma);

}
