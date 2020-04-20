#include "mex.h"

#define TRUE 1
#define FALSE 0

typedef unsigned char uint8;

/*inline void ind2sub(int rc, int index, int &r, int &c)
{
	c = index/rc;
	r = index - c*rc;
}*/

inline void sub2ind(int rc, int &index, int r, int c)
{
	index = c*rc + r;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	if(nrhs != 2 || nlhs != 1)
		mexErrMsgTxt("lbpim = lbp_c(img,lb_kernel)");
	if(mxGetNumberOfDimensions(prhs[0]) > 2)
		mexErrMsgTxt("First argument 'img' must be 2D (i.e. grayscale image).");
	if(mxGetNumberOfElements(prhs[1]) != 9)
		mexErrMsgTxt("Second argument 'lb_kernel' must contain 9 elements.");
	if(!mxIsClass(prhs[0],"uint8"))
		mexErrMsgTxt("First argument 'img' must be of class 'uint8'.");
	if(!mxIsClass(prhs[1],"double"))
		mexErrMsgTxt("Second argument 'lb_kernel' must be of class 'double'.");
		
	int index, count, cur_pixel;
	uint8 sumNeighbors, *bnd;
	uint8 *img = (uint8 *)mxGetData(prhs[0]);
	double *lb = (double *)mxGetData(prhs[1]);
	int nRow = mxGetM(prhs[0]);
	int nCol = mxGetN(prhs[0]);
	int dims[2];
	
	dims[0] = nRow;
	dims[1] = nCol;
	plhs[0] = mxCreateNumericArray(2,dims,mxUINT8_CLASS,mxREAL);
	bnd = (uint8 *) mxGetData(plhs[0]);
	
	for(int i=0; i < nRow; i++) {
		for(int j=0; j < nCol; j++) {
			sub2ind(nRow,cur_pixel,i,j);
			count = -1;
			sumNeighbors = 0;
			
			for(int c=-1; c <= 1; c++) {
				for(int r=-1; r <= 1; r++) {
					count++;
					if(r == 0 && c == 0)
						continue;
					if(i+r < 0 || i+r > nRow-1 || j+c < 0 || j+c > nCol-1)
						continue;
					sub2ind(nRow,index,i+r,j+c);
					if(img[index] >= img[cur_pixel])
						sumNeighbors += (uint8) lb[count];
				}
			}
			
			bnd[cur_pixel] = sumNeighbors;
		}
	}
}