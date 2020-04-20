#include "mex.h"
#include "general_math.h"
#include "matrix.h"

typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned short uint16;

using namespace genmath;

inline void graycomtx(const uint16 *block, uint8 *is_valid, double *comtx, 
    int dist, int graylevels, int rows, int cols,int slices) 
{
   int i,j,k,l,m,sl;
	
   for(i = 0; i < graylevels*graylevels; i++)
      comtx[i] = 0;

   for(sl = 0; sl < slices; sl++) 
	   for(j = 0; j < cols; j++)
	      for(i = 0; i < rows; i++)
			   if(block[i+j*rows+sl*rows*cols] >= graylevels)
			      mexErrMsgTxt("Graylevels of image fall outside acceptable range.");

   for(sl = 0; sl < slices; sl++)
	   for(j = 0; j < cols; j++)
	      for(i = 0; i < rows; i++)
			{
	        if(is_valid[i+j*rows+sl*rows*cols] == 0)
	            continue;

			uint16 center_value = block[i+j*rows+sl*rows*cols];

			int start_row = genmath::max((int) 0, i-dist);
			int start_col = genmath::max((int) 0, j-dist);
			int start_sl = genmath::max((int) 0, sl-dist);
			int end_row = genmath::min((int) rows-1, i+dist);
			int end_col = genmath::min((int) cols-1, j+dist);
			int end_sl = genmath::min((int) slices-1, sl+dist);
			for(m = start_sl;m <= end_sl; m++)
			   for(l = start_col; l <= end_col; l++)
				   for(k = start_row; k <= end_row; k++)
					{
					comtx[center_value + block[k+l*rows+m*rows*cols]*graylevels] += is_valid[k+l*rows+m*rows*cols];
					//comtx[center_value + block[k+l*rows+m*rows*cols*graylevels] += 1;
					}
			}
			
    /* Subtract off points corresponded with themselves. */
	for(sl = 0; sl < slices; sl++)
		for(j = 0; j < cols; j++)
			for(i = 0; i < rows; i++)
			{
			uint16 center_value = block[i+j*rows+sl*rows*cols];
			if(is_valid[i+j*rows+sl*rows*cols] != 0)
				comtx[center_value + center_value*graylevels] -= is_valid[i+j*rows+sl*rows*cols];
			}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nrhs != 3 && nrhs != 4)
		mexErrMsgTxt("graycomtx_3d(block,graylevels,dist,is_valid)");

    if(!mxIsClass(prhs[0],"uint16"))
		mexErrMsgTxt("BLOCK must be UINT16.");
    if(!mxIsClass(prhs[1],"double"))
		mexErrMsgTxt("GRAYLEVELS must be DOUBLE.");
	
    int rows, cols, slices;
    uint16 *block = (uint16 *) mxGetData(prhs[0]);
   
    int d = (int) mxGetNumberOfDimensions(prhs[0]);
    if(d == 3)
		{
		rows = (int) ((mxGetDimensions(prhs[0]))[0]);
		cols = (int) ((mxGetDimensions(prhs[0]))[1]);
		slices = (int) ((mxGetDimensions(prhs[0]))[2]);
		}
	else
		{
		/* mexWarnMsgTxt("Input block is 2D. Setting Z dimension to 1"); */
		rows = (int) ((mxGetDimensions(prhs[0]))[0]);
		cols = (int) ((mxGetDimensions(prhs[0]))[1]);
		slices = 1;
		}
 
    uint16 dist = (uint16) *mxGetPr(prhs[2]);

	int graylevels = (int) *mxGetPr(prhs[1]);
	if(graylevels < 0 || graylevels > 65535)
		mexErrMsgTxt("GRAYLEVELS must be between 0 and 2^16-1.");

	uint8 *is_valid;
	if(nrhs == 4)
   {
      if(((mxGetDimensions(prhs[3]))[0]) != rows || ((mxGetDimensions(prhs[3]))[1]) != cols)
         mexErrMsgTxt("IS_VALID and BLOCK must be same size.");
		 
      if(mxGetElementSize(prhs[3]) != 1)
      if(!mxIsClass(prhs[3],"logical"))
         mexErrMsgTxt("IS_VALID must be LOGICAL.");
	   is_valid = (uint8 *) mxGetData(prhs[3]);
   }
   else
   {
      is_valid = new uint8[rows*cols*slices];
      for(int i = 0; i < rows*cols*slices; i++)
         is_valid[i] = 1;
   }
	 
   int dims[2];
   dims[0] = graylevels;
   dims[1] = graylevels;
   plhs[0] = mxCreateNumericArray(2,dims,mxDOUBLE_CLASS,mxREAL);
   double *comtx = (double *) mxGetData(plhs[0]);

   graycomtx(block,is_valid,comtx,dist,graylevels,rows,cols,slices);
}
	
    

