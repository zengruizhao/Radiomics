#include<stdlib.h>
#include "mex.h"

#ifdef _MSC_VER  // because microsoft visual studio is annoying like that
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#else
#include<stdint.h>
#endif

#define FALSE 0
#define TRUE 1

static uint32_t n_edges;

inline void sub2ind(int rc, int &index, int r, int c)
{
	index = c*rc + r;
}

// basic recursive implementation of hosoya
/*unsigned long long hosoya_index (int first_edge) {
	unsigned long long result = 1;
	for (int i=first_edge; i<n_edges; i++) {
		unsigned int v0 = edgeinfos[i].vertices[0];
		unsigned int v1 = edgeinfos[i].vertices[1];
		if ((!used_up[v0]) && (!used_up[v1])) {
			used_up[v0] = true;
			used_up[v1] = true;
			result += hosoya_index (i+1);
			used_up[v0] = false;
			used_up[v1] = false;
		}
	}
	return result;
}*/

// unweighted recursive implementation of hosoya index (i.e. the original hosoya index)
double hosoya_index (int first_edge, double edges[], bool used_up[]) {
	int idx1, idx2;
	double result = 1;
	for (int i=first_edge; i<n_edges; i++) {
		sub2ind(n_edges, idx1, i, 0);
		sub2ind(n_edges, idx2, i, 1);
		uint32_t v0 = (uint32_t) edges[idx1];
		uint32_t v1 = (uint32_t) edges[idx2];
//		mexPrintf("Debug: Edge %i, vertices [%i %i], idx [%i %i]\n",i, (int) v0, (int) v1, (int) idx1, (int) idx2);
		if ((!used_up[v0-1]) && (!used_up[v1-1])) {
			used_up[v0-1] = true;
			used_up[v1-1] = true;
			result += hosoya_index (i+1, edges, used_up);
			used_up[v0-1] = false;
			used_up[v1-1] = false;
		}
	}
	return result;
}

// hosoya index weighted by total matching weight (i.e. sum of weights of all edges in matching)
double hosoya_index_weighted (int first_edge, double matching_weight, double edges[], double edgeweights[], bool used_up[]) {
	int idx1, idx2;
	double result = matching_weight; // result should be the weight for the matching
	for (int i=first_edge; i<n_edges; i++) {
		sub2ind(n_edges, idx1, i, 0);
		sub2ind(n_edges, idx2, i, 1);
		uint32_t v0 = (uint32_t) edges[idx1];
		uint32_t v1 = (uint32_t) edges[idx2];
		if ((!used_up[v0-1]) && (!used_up[v1-1])) {
			used_up[v0-1] = true;
			used_up[v1-1] = true;
			matching_weight += edgeweights[i];
//			mexPrintf("Debug: Edge %i, weight %g\n",(int) i, (double) edgeweights[i]);
			result += hosoya_index_weighted (i+1, matching_weight, edges, edgeweights, used_up);
			used_up[v0-1] = false;
			used_up[v1-1] = false;
			matching_weight -= edgeweights[i];
		}
	}
//	mexPrintf("Debug: matching weight: %g\n", (double) result);
	return result;
}

// same as hosoya_index_weighted, but with matching weight normalized by number of edges in matching
double hosoya_index_weighted_normalized (int first_edge, double matching_weight, int n_edges_in_matching, double edges[], double edgeweights[], bool used_up[]) {
	int idx1, idx2;
	double temp;
	double result = matching_weight; // result should be the weight for the matching
	for (int i=first_edge; i<n_edges; i++) {
		sub2ind(n_edges, idx1, i, 0);
		sub2ind(n_edges, idx2, i, 1);
		uint32_t v0 = (uint32_t) edges[idx1];
		uint32_t v1 = (uint32_t) edges[idx2];
		if ((!used_up[v0-1]) && (!used_up[v1-1])) {
			used_up[v0-1] = true;
			used_up[v1-1] = true;
			n_edges_in_matching++;
			matching_weight = (matching_weight * (n_edges_in_matching-1) + edgeweights[i]) / n_edges_in_matching; // add weight of new edge in matching while accounting for averaging
//			mexPrintf("Debug: UP  : Edge %i, weight %g, matching_weight: %g, n_edges_in_matching: %i\n",(int) i, (double) edgeweights[i], (double) matching_weight, (int) n_edges_in_matching);			

			result += hosoya_index_weighted_normalized (i+1, matching_weight, n_edges_in_matching, edges, edgeweights, used_up);
//			mexPrintf("Debug: DOWN: Edge %i, weight %g, matching_weight: %g, n_edges_in_matching: %i\n",(int) i, (double) edgeweights[i], (double) matching_weight, (int) n_edges_in_matching);
			used_up[v0-1] = false;
			used_up[v1-1] = false;
			n_edges_in_matching--;
			if(n_edges_in_matching > 0)
				matching_weight = (matching_weight * (n_edges_in_matching+1) - edgeweights[i]) / n_edges_in_matching; // remove weight of newest edge in matching while accounting for averaging 
			else
				matching_weight = 0;
		}
	}
//	mexPrintf("Debug: DOWN: Result: %g\n",(double) result);
	return result;
}



void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	// check inputs and outputs
	bool doNormalize = false; // change to true if 3rd argument exists/is true
	if(nrhs < 1 || nrhs > 3)
		mexErrMsgTxt("Check input arguments!  Calling format is 'hosoya_c( edges [, weights] [, normalize ] )'");
	if(mxGetN(prhs[0]) != 2)
		mexErrMsgTxt("Edges must have 2 columns [v1 v2].");
	if(nrhs > 1) {
		if(mxGetM(prhs[0]) != mxGetM(prhs[1]))
			mexErrMsgTxt("Must have same number of rows in edges and weights!");
		if(mxGetN(prhs[1]) != 1)
			mexErrMsgTxt("Weights must have 1 column.");
	}
	if(nrhs > 2 && !mxIsEmpty(prhs[2])) {
		doNormalize = mxGetScalar(prhs[2]) == TRUE ? true:false;
//		mexPrintf("Debug: Normalize flag set to %s.\n", doNormalize ? "true":"false");
	}

	// declare variables
	double result, *edges, *edgeweights;
	bool *used_up;	
	uint32_t n_vertices;

	// retrieve inputs
	n_edges = mxGetM(prhs[0]);
	edges = (double *)mxGetData(prhs[0]);
	if(nrhs > 1) {
		edgeweights = (double *)mxGetData(prhs[1]);
//		mexPrintf("Debug: Allocated edgeweights correctly.\n");
	}

	// find number of vertices
	n_vertices = 0;
	for(int i=0; i < n_edges*2; i++) {
//		mexPrintf("%i ",(int) edges[i]);
		if(edges[i] > n_vertices)
			n_vertices = edges[i];
	}
//	mexPrintf("\n");

	// set up the "used_up" vector
	used_up = new bool[n_vertices];
	for(int i=0; i<n_vertices; i++)
		used_up[i] = false;

//	mexPrintf("Debug: Setup complete. n_vertices=%i, n_edges=%i\n",(int) n_vertices, (int) n_edges);
	
	// call appropriate hosoya index code
	if(nrhs == 1)
		result = hosoya_index (0, edges, used_up);
	else {
		if(!doNormalize)
			result = hosoya_index_weighted(0, 0, edges, edgeweights, used_up);
		else
			result = hosoya_index_weighted_normalized(0, 0, 0, edges, edgeweights, used_up);
//		result += 1;  // NOTE: uncomment this line to include the null set; is it really necessary?
	}
	
	// outputs
	plhs[0] = mxCreateDoubleScalar((double) result);	
	
	// clean up
	delete[] used_up;
}
