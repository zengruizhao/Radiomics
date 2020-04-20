#include "mex.h"
#include <math.h>
#include <algorithm>
//#include <sstream>
//#include <string>
/* Histogram2
 *Jon Chappelow
 */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    double *I1, *I2, *N, *H, I1min, I1max, I2min, I2max;
    mwSize hndims=2;
    mwSize hdims[2];
    size_t i, hloc, npixI1, npixI2;
    //std::ostringstream sstream;
    //std::string *errstring;
    //const char *errstring;
    
    if (nlhs>1)
        mexErrMsgTxt("Wrong number of output parameters, usage:  Histogram = histogram2(Image1, Image2, N)");
    if (nrhs!=3)
        mexErrMsgTxt("Wrong number of input parameters, usage:  Histogram = histogram2(Image1, Image2, N)");
    
    if (!mxIsDouble(prhs[0]) || !mxIsDouble(prhs[1]) || !mxIsDouble(prhs[2]))
        mexErrMsgTxt("Inputs not doubles.");
    
    npixI1=mxGetNumberOfElements(prhs[0]);
    npixI2=mxGetNumberOfElements(prhs[1]);
    if (npixI1 != npixI2) mexErrMsgTxt("Input parameters X, Y, Z must have the same size");
    
    I1=mxGetPr(prhs[0]);
    I2=mxGetPr(prhs[1]);
    N=mxGetPr(prhs[2]);
    
    I1min=*std::min_element(I1,I1+npixI1);
    I1max=*std::max_element(I1,I1+npixI1);
    I2min=*std::min_element(I2,I2+npixI2);
    I2max=*std::max_element(I2,I2+npixI2);
    
    /*
    mexPrintf("Highest value in I1: %g\n",I1max);
    mexPrintf("Highest value in I2: %g\n",I2max);
    mexPrintf("Lowest value in I1: %g\n",I1min);
    mexPrintf("Lowest value in I2: %g\n",I2min);
    */
    
    if (I1min<0 || I2min<0 || I1max>*N-1 || I2max>*N-1) {
        //sstream << "Data out of range for N of " << *N << std::endl;
        //const char *errstring = sstream.str().c_str();
        mexPrintf("ERROR: Data out of range for N of %g.\n",*N);
        mexErrMsgTxt("Rescale your data.\n");
    }
    
    hdims[0]=(mwSize) *N; // dereference to get double and cast to mwSize
    hdims[1]=(mwSize) *N;
    
    plhs[0]=mxCreateNumericArray(hndims, hdims, mxDOUBLE_CLASS, mxREAL);
    H=mxGetPr(plhs[0]);
    
    for (i=0;i<npixI1;i++) {
        hloc=I1[i]+*N * I2[i]; // derefernce N to get double
        H[hloc]++;
    }
}
