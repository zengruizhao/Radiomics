#include "mex.h"
#include "general_math.h"
#include "matrix.h"

typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned short uint16;

using namespace genmath;

inline void graycomtx(const uint16 *block, const double *opmtx,
   double *comtx, int graylevels, uint8 *is_valid, int rows, int cols, 
   int mrows, int mcols) 
{
   int i,j,k,l;
	int hmrows = mrows/2;
	int hmcols = mcols/2;
	
   for(i = 0; i < graylevels*graylevels; i++)
      comtx[i] = 0;

   for(j = 0; j < cols; j++)
      for(i = 0; i < rows; i++)
		   if(block[i+j*rows] >= graylevels)
		      mexErrMsgTxt("Graylevels of image fall outside acceptable range.");

   for(j = 0; j < cols; j++)
      for(i = 0; i < rows; i++)
		{
         if(is_valid[i+j*rows] == 0)
            continue;

		   uint16 center_value = block[i+j*rows];

			int start_row = genmath::max((int) 0, i-hmrows);
			int start_col = genmath::max((int) 0, j-hmcols);
			int end_row = genmath::min((int) rows-1, i+hmrows);
			int end_col = genmath::min((int) cols-1, j+hmcols);
		   for(l = start_col; l <= end_col; l++)
			   for(k = start_row; k <= end_row; k++)
				{
				   double opmtx_val = opmtx[(i-k+hmrows) + (j-l+hmcols)*mrows];
               comtx[center_value + block[k+l*rows]*graylevels] += 
                  opmtx_val * is_valid[k+l*rows];
               //comtx[center_value + block[k+l*rows]*graylevels] += 1;
				}
		}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   if(nrhs != 3 && nrhs != 4)
		mexErrMsgTxt("comtx = graycomtx_c(block,opmtx,graylevels,is_valid)");

   if(!mxIsClass(prhs[0],"uint16"))
		mexErrMsgTxt("BLOCK must be UINT16.");
   if(!mxIsClass(prhs[1],"double"))
		mexErrMsgTxt("MASK must be DOUBLE.");

   int rows = (int) mxGetM(prhs[0]);
   int cols = (int) mxGetN(prhs[0]);
   int mrows = (int) mxGetM(prhs[1]);
   int mcols = (int) mxGetN(prhs[1]);
	if((mrows/2) * 2 == mrows)
		mexErrMsgTxt("MASK row length must be odd.");
	if((mcols/2) * 2 == mcols)
		mexErrMsgTxt("MASK column length must be odd.");

   uint16 *block = (uint16 *) mxGetData(prhs[0]);
   double *opmtx = (double *) mxGetData(prhs[1]);

	int graylevels = (int) *mxGetPr(prhs[2]);
	if(graylevels < 0 || graylevels > 65535)
		mexErrMsgTxt("GRAYLEVELS must be between 0 and 2^16-1.");

	uint8 *is_valid;
	if(nrhs == 4)
   {
      if(mxGetM(prhs[3]) != rows || mxGetN(prhs[3]) != cols)
         mexErrMsgTxt("IS_VALID and BLOCK must be same size.");
      if(mxGetElementSize(prhs[3]) != 1)
      if(!mxIsClass(prhs[3],"logical"))
         mexErrMsgTxt("IS_VALID must be LOGICAL.");
	   is_valid = (uint8 *) mxGetData(prhs[3]);
   }
   else
   {
      is_valid = new uint8[rows*cols];
      for(int i = 0; i < rows*cols; i++)
         is_valid[i] = 1;
   }
	 
   int dims[2];
   dims[0] = graylevels;
   dims[1] = graylevels;
   plhs[0] = mxCreateNumericArray(2,dims,mxDOUBLE_CLASS,mxREAL);
   double *comtx = (double *) mxGetData(plhs[0]);

   graycomtx(block,opmtx,comtx,graylevels,is_valid,rows,cols,mrows,mcols);

   if(nrhs !=4)
      delete[] is_valid;
}
	
    

