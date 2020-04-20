#include<float.h>
#include<math.h>
#include<algorithm>
#include "mex.h"
#include "blas1_d.c"

#define PI 3.14159265
#define FALSE 0
#define TRUE 1

// custom functor to define strict weak ordering of indices for std::sort
class ltIdx { 
	double * _x;
	
public:
	ltIdx(double *x) : _x(x) {}
	bool operator() (int i, int j) { return _x[i] < _x[j]; }
};

inline void sub2ind(int rc, int &index, int r, int c)
{
	index = c*rc + r;
}

inline void matSubtract(double mat1[], double mat2[], int numElements, double matOut[])
{
	int i;
	for(i=0; i < numElements; i++)
		matOut[i] = mat1[i]-mat2[i];
}

inline double L2dist(double vec1[], double vec2[], int numDims, double work[])
{
	matSubtract(vec1,vec2,numDims,work);
	return dnrm2( 2, work, 1 );
}

inline bool direcTest(double distij, double distjk, double distik, double directionConstraint)
{
	return (distij*distij+distjk*distjk-distik*distik)/(distij*distjk) < directionConstraint;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// inputs
	if(nrhs != 5)
		mexErrMsgTxt("Four inputs only!  Calling format is 'callaghan_c( centroids , neighbors , Tr, Ta, res )'");
	if(mxGetN(prhs[0]) != 2 || mxGetN(prhs[1]) != 2)
		mexErrMsgTxt("Inputs must have 2 columns [X Y] each.");
	//if(mxGetM(prhs[0]) != 1)
	//	mexErrMsgTxt("We can only compare 1 centroid at a time right now.");
	double *XY_center = (double *)mxGetData(prhs[0]);
	double *XY_neighbors = (double *)mxGetData(prhs[1]);
	int M_center = mxGetM(prhs[0]);
	int M_neighbors = mxGetM(prhs[1]);
	double Tr = (double)mxGetScalar(prhs[2]);
	double Ta = (double)mxGetScalar(prhs[3]);
	double res = (double)mxGetScalar(prhs[4]);

	//outputs
	plhs[0] = mxCreateLogicalMatrix(M_neighbors, M_center);
	plhs[1] = mxCreateDoubleMatrix(M_neighbors, M_center, mxREAL);
//	plhs[2] = mxCreateNumericMatrix(M_neighbors,M_center,mxINT32_CLASS, mxREAL);

	// declare variables
	bool doOnce;
	int i,j, k, ii, idx1, idx2, idx_colStart;
	double distanceConstraint;
	double directionConstraint = 2*cos(Ta*PI/180);
	mxLogical mxLogicalTrue = (mxLogical) TRUE;
	mxLogical mxLogicalFalse = (mxLogical) FALSE;
	
	double *W = (double *)mxGetData(plhs[1]);
	int *W_ind_sort = new int[M_neighbors*M_center];
	mxLogical *isValid = (mxLogical *)mxGetData(plhs[0]);
	double *vec1 = new double[2];
	double *vec2 = new double[2];
	double *work = new double[2];
	// initializations
	for(i=0; i<M_neighbors; i++) {
		for(j=0; j<M_center; j++) {
			sub2ind(M_neighbors,idx1,i,j);
			W_ind_sort[idx1] = i;
			isValid[idx1] = mxLogicalTrue;
		}
	}

	
	// ALGORITHM STARTS HERE

	for(ii=0; ii<M_center; ii++) {
		// things that stay constant for this entire column
		sub2ind(M_neighbors,idx_colStart,0,ii); // index referencing start of column in all [M_neighbors x M_center] arrays... will need this several times later
		sub2ind(M_center,idx1,ii,0);  // 1st coordinate of current center
		sub2ind(M_center,idx2,ii,1);  // 2nd coordinate of current center
		vec1[0] = XY_center[idx1];
		vec1[1] = XY_center[idx2];
		
		// distance between lumen and all neighbors
		for(i=0; i<M_neighbors; i++) {
			sub2ind(M_neighbors,idx1,i,0);
			sub2ind(M_neighbors,idx2,i,1);
			vec2[0] = XY_neighbors[idx1];
			vec2[1] = XY_neighbors[idx2];
						
			sub2ind(M_neighbors,idx1,i,ii);
			W[idx1] = L2dist(vec1,vec2,2,work);
		}
		
		// sort W from low to high and return sorted index.  NOTE: only W column for current center
		std::stable_sort(W_ind_sort, W_ind_sort+M_neighbors, ltIdx(W+idx_colStart));
		
		// define distance constraint based on smallest non-zero element in W and apply it
		doOnce = true;
		for(i=0; i<M_neighbors; i++) {
			if(doOnce && W[W_ind_sort[i]+idx_colStart] == 0.0)
				isValid[W_ind_sort[i]+idx_colStart] = mxLogicalFalse;
			else if(doOnce) {
				distanceConstraint = W[W_ind_sort[i]+idx_colStart] * Tr / res;
				doOnce = false;
			} else if(W[W_ind_sort[i]+idx_colStart] > distanceConstraint)
				isValid[W_ind_sort[i]+idx_colStart] = mxLogicalFalse;
		}
			
		// define & apply directional constraint
		for(j=0; j<M_neighbors; j++) {
			if(isValid[W_ind_sort[j]+idx_colStart] == mxLogicalFalse)
				continue;
			for(k=j+1; k<M_neighbors; k++) {
				if(isValid[W_ind_sort[k]+idx_colStart] == mxLogicalFalse)
					continue;
				
				sub2ind(M_neighbors,idx1,W_ind_sort[j],0);
				sub2ind(M_neighbors,idx2,W_ind_sort[j],1);
				vec1[0] = XY_neighbors[idx1];
				vec1[1] = XY_neighbors[idx2];
				sub2ind(M_neighbors,idx1,W_ind_sort[k],0);
				sub2ind(M_neighbors,idx2,W_ind_sort[k],1);
				vec2[0] = XY_neighbors[idx1];
				vec2[1] = XY_neighbors[idx2];
				
				if( direcTest(W[W_ind_sort[j]+idx_colStart], L2dist(vec1,vec2,2,work), W[W_ind_sort[k]+idx_colStart], directionConstraint) )
					isValid[W_ind_sort[k]+idx_colStart] = mxLogicalFalse;
			}
		}
	}
	
	delete []W_ind_sort;
	delete []vec1;
	delete []vec2;
	delete []work;
}
