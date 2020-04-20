#include "mex.h"
#include "general_math.h"
#include <list>

typedef unsigned char uint8; 
using namespace genmath;

struct bound_element
{
	int r;
   int c;   
};

inline void trace_bound(uint8 *bimg, int rows, int cols, int start_r, 
		int start_c, std::list<struct bound_element> &bound)
{
	int i,r,c;
	int rbox[] = {0,-1,-1,-1,0,1,1,1};
	int cbox[] = {-1,-1,0,1,1,1,0,-1};

	if(start_r == -1)
	{
		for(i = 0; i < rows*cols; i++)
			if(bimg[i] == 1)
				break;
	   start_c = i/rows;
	   start_r = i-start_c*rows;
	}

	struct bound_element current;

	current.r = start_r; 
	current.c = start_c; 

	bound.push_back(current);

	int cnt = 4;
	while(1)
	{
		cnt = (cnt + 5)%8; 
		//mexPrintf("%d\n", cnt);
		for(i = 0; i < 8; i++)
		{
			r = current.r + rbox[cnt];
			c = current.c + cbox[cnt];

			if(r < 0 || r >= rows || c < 0 || c >= cols || bimg[r+c*rows]==0)
				cnt = (cnt + 1)%8; 
			else
				break;
		}

		if((r == start_r && c == start_c) || i==8)
			break; 

		//mexPrintf("%d %d\n", r,c);

		current.r = r;
		current.c = c;

		bound.push_back(current);
	}
}



void mexFunction(int nlhs, mxArray **plhs, 
				 int nrhs, const mxArray **prhs)
{
	if(nrhs != 1 && nrhs != 3)
		mexErrMsgTxt("bound = trace_bound_c(bimg,start_r,start_c)");

	if(!mxIsClass(prhs[0],"logical"))
		mexErrMsgTxt("BIMG must be type LOGICAL.");

	uint8 *bimg = (uint8*) mxGetData(prhs[0]);

	int start_r = -1; 
	int start_c = -1;
	if(nrhs > 1)
	{
		start_r = mxGetScalar(prhs[1]) - 1;
		start_c = mxGetScalar(prhs[2]) - 1;
	}
	int rows = mxGetM(prhs[0]);
	int cols = mxGetN(prhs[0]);

   std::list<struct bound_element> bound;
	trace_bound(bimg, rows, cols, start_r, start_c, bound);

   int dims[2];
	dims[0] = bound.size();
	dims[1] = 2;
	plhs[0] = mxCreateNumericArray(2,dims,mxDOUBLE_CLASS,mxREAL);

	double *bnd = (double *) mxGetData(plhs[0]);
	std::list<struct bound_element>::iterator current = bound.begin();
	for(int i = 0; i < dims[0]; i++,current++)
	{
		bnd[i] = current->r + 1; //C to Matlab notation
		bnd[i+dims[0]] = current->c + 1; //C to Matlab notation
	}
		
}
   



