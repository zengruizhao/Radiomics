// include a bunch of header files 
//#include "general_math.h"
#include "mex.h"


// include namespace packages
//using namespace std;
//using namespace genmath;

inline void searchthroughedges(float nodes[], float upper[], int ncols, double &count)
{
	count =0;
	for (int i = 0; i < ncols; i++)
		for (int j = 0; i < ncols; j++)
		{
			if (upper[i*j] == 1)
				count++;
		}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	float *nodes; // input 1xN vector
	float *upper; // NxN input matrix of edges
	int ncols;
	double *count;

if(nrhs!=2)
    mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                      "Two inputs required.");

   if(!mxIsClass(prhs[0],"uint16"))
		mexErrMsgTxt("nodes must be DOUBLE.");
   if(!mxIsClass(prhs[1],"double"))
		mexErrMsgTxt("upper must be DOUBLE.");

   
   nodes = (float *)prhs;
   upper = (float *)prhs+1;
   ncols = mxGetN(prhs[0]);

   plhs[0] = mxCreateDoubleMatrix(1,ncols,mxREAL);
   count = mxGetPr(plhs[0]);
   
   /* call routine */
   searchthroughedges(nodes,upper,ncols,*count);

}
/*
inline void countnodesandedges(double network, double upper)

{
	int i,j, neighborhood_size, nodes, n, count, En, kn;

	for (n=1; n <= sizeof(network); n++)
	{
		

	}

}
*/