#include "mex.h"
#include <math.h>
#include <typeinfo>
//#include <algorithm>
//#include <sstream>
//#include <string>
/* randomize
 *Jon Chappelow
 */

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    mwSize nelements;
    double *origarray, *shuffled;
    double tmp, nrandsneeded;
    size_t i, j, r, randval;
    bool dyninitarray;
    
    if (nlhs>1)
        mexErrMsgTxt("Wrong number of output parameters, usage:  shuffled = randomizemex(array)");
    if (nrhs!=1)
        mexErrMsgTxt("Wrong number of input parameters, usage:  shuffled = randomizemex(array)");
    
    if (!mxIsDouble(prhs[0])) mexErrMsgTxt("Input not a double.");

    nelements=mxGetNumberOfElements(prhs[0]);
    
    //const double *inarray=mxGetPr(prhs[0]);
    
    if (nelements>1) {
        origarray=mxGetPr(prhs[0]);
        dyninitarray=false;
    } else {
        //nelements=(mwSize) *mxGetPr(prhs[0]);
        nelements=(mwSize) mxGetScalar(prhs[0]);
        //nelements=(mwSize) *inarray;
        origarray=(double*) mxCalloc(nelements,sizeof(double));
        dyninitarray=true;
        
        for (i=0; i<nelements; i++) {
            origarray[i]=i+1;
            //shuffled[i]=i+1;
        }
    }
    
    //mexPrintf("Number of elements in array: %d.\n", nelements);
    
    plhs[0]=mxCreateDoubleMatrix(1,nelements,mxREAL);
    shuffled=mxGetPr(plhs[0]);
    
    for (i=0; i<nelements; i++) {
        shuffled[i]=origarray[i];
    }
    
    //mexPrintf("i is type %s, size %d bytes\n",typeid(i).name(),sizeof(i));
    
    nrandsneeded=1+ceil((double) nelements/(1+(double) RAND_MAX));
    //mexPrintf("Using %1.0f random values.\n", nrandsneeded);
    
    for (i=0;i<nelements-1;i++) {
        randval=0;
        for (r=0; r<nrandsneeded; r++) randval+=rand();
        j=(randval % (nelements - i)) + i;
        //j=((rand()+rand()+rand()) % (nelements - i)) + i;
        //j=floor(((double) rand()/(double) RAND_MAX) * (nelements-1 - i)) + i;
        //random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0)); 
        //mexPrintf("random index: %d.\n", j);
        tmp=shuffled[i];
        shuffled[i]=shuffled[j];
        shuffled[j]=tmp;
    }
     
    //mexPrintf("Soooo.....\n");
    
    if (dyninitarray) mxFree(origarray);
}
