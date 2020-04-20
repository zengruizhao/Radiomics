#include "mex.h"
#include "pqheap.h"
#include <math.h>

#ifndef min
#define min(X, Y)  (X < Y ? X : Y)
#endif

#define LEAFSIZE 50

//#define TESTTREE(PP)  ((PP) = (optkdNode *) mxMalloc(sizeof(optkdNode)))
//#define NEWTREE(PP)  if (TESTTREE(PP)==NULL) {mexErrMsgTxt("Cannot allocate memory for tree.\n");return;}

/* kd Node struct */
typedef struct optkdnode {
  int leaf;  /* true if the node is a leaf node */
  size_t discrim; /* discriminator of node */
  double cutval; 
  struct optkdnode *loson, *hison;
  size_t lopt,hipt;
} optkdNode;

/* functions */
static void knnkdtree (double *, size_t, size_t, unsigned int, double * , double *, double *, double);

optkdNode *BuildOptTree(double **, size_t, size_t, size_t *);
optkdNode *BuildkdTree(double **, size_t, size_t, size_t, size_t *);
    size_t dimmaxrange(size_t, size_t, size_t, double **, size_t *);
    void Selection(double **, size_t, size_t, size_t, size_t, size_t *);
    
size_t *kdOptNNQuery(double **, size_t, double *, unsigned int, optkdNode *, size_t, double *, size_t *);
    void rnnEuclidean(optkdNode *, double *, double **, size_t, unsigned int, size_t *, size_t, double *, size_t *);

void KillOptTree(optkdNode *);

/* GLOBALS */

/* knnkdtree */
void knnkdtree (double *X, size_t npts, size_t ndims, unsigned int kns, double *len, double *Graph, double *dists, double gamma) {
    double **Points;
    double *querpoint;
    double *nndist;
    size_t *perm;
    optkdNode *OptkdRoot;
    size_t *Found;
    size_t i, j, k;
	//size_t noutvals;
    //mwSize ndims, npts;
    
    //npts=Xdims[0]; ndims=Xdims[1];
	//npts = (int) npts;
	//ndims = (int) ndims;
	//kns = (int) kns;
    *len = 0.0;
    
    querpoint=(double *) mxMalloc(ndims*sizeof(double));
    //querpoint=(double *) malloc((mwSize)ndims*sizeof(double));
    
    Points = (double **) mxMalloc(npts*sizeof(double *));
    //Points = (double **) malloc((mwSize)npts*sizeof(double *));
    for (k = 0; k < npts; k++) {
        Points[k] = (double *) mxMalloc(ndims*sizeof(double));
        //Points[k] = (double *) malloc((mwSize)ndims*sizeof(double));
    }
    
    /*
    for (k = 0, i = 0; k < npts; k++) {
        for (j = 0; j < ndims; j++, i++) {
            Points[k][j] = X[i];
        }
    }
    */
    for (j = 0, i = 0; j < ndims; j++) {
        for (k = 0; k < npts; k++, i++) {
            Points[k][j] = X[i];
        }
    }

    // Build kd tree, root node contains all other nodes, hence the tree
    //OptkdRoot = BuildOptTree(Points, npts, ndims, perm);
    perm = (size_t *) mxMalloc(npts*sizeof(size_t));
    for (j=0; j < npts; j++) perm[j]=j;
    OptkdRoot=BuildkdTree(Points, 0, npts-1, ndims, perm);
    
    //nndist needs to be initiallized if it is not a global
    nndist = (double *) mxMalloc((kns+1)*sizeof(double));
    for (j=0; j < kns+1; j++) nndist[j] = HUGEDOUBLE;
    
    // Find kNNs for each point by traversing kd tree
    for (i = 0, k = 0; i < npts; i++) {
        // Grab query point i (each dimension j)
        for (j = 0; j < ndims; j++)
            querpoint[j]=Points[i][j];
        
        // Find the kNNs for the query point
        Found = kdOptNNQuery(Points, ndims, querpoint, kns, OptkdRoot, i, nndist, perm);
        //mexPrintf("Found: %d %d %d %d %d %d %d.\n",Found[kns],Found[kns-1],Found[kns-2],Found[kns-3],Found[kns-4],Found[kns-5],Found[kns-6]);
        //mexPrintf("nndist: %f %f %f %f %f %f %f.\n",nndist[kns],nndist[kns-1],nndist[kns-2],nndist[kns-3],nndist[kns-4],nndist[kns-5],nndist[kns-6]);
        
        for (j = kns; j >= 1; j--, k++) /*Found[0] is a dummy. */ {
            dists[k]=sqrt(nndist[j]);
            *len += (gamma>1 ? pow(dists[k], gamma) : dists[k]);
            Graph[k] = (double) Found[j] + 1;
        }
    }
    
//     noutvals=(size_t) kns*npts;
//     if (gamma>1) {
//         for (k=0; k<noutvals; k++)
//             *len += pow(dists[k], gamma);
//     } else {
//         for (k=0; k<noutvals; k++)
//             *len += dists[k];
//     }
    
    // mxFree slows things down (?)
    //mxFree(querpoint);
    //for (k=0;k<npts;k++) mxFree(Points[k]);
    //mxFree(Points);
    
    //free(querpoint);
    //for (k=0;k<npts;k++) free(Points[k]);
    //free(Points);
    
    //KillOptTree(OptkdRoot);
    //OptkdRoot=NULL;
    
   /*KillOptTree(OptkdRoot);
    OptkdRoot=NULL;*/
    /*free(Found);*/
    /*for (k=0;k<npts;k++)
    free(Points[k]);
    free(querpoint);*/
}


/* BuildOptTree */
optkdNode *BuildOptTree(double **points, size_t npts, size_t ndims, size_t *perm) {
    size_t j;
    
    /* initialize perm array */
    perm = (size_t *) mxMalloc(npts*sizeof(size_t));
    //perm = (int *) malloc(npts*sizeof(int));
    for (j=0; j < npts; j++) {
        perm[j]=j; // 0:(npts-1)
    }
    
    return(BuildkdTree(points, 0, npts-1, ndims, perm));
}


/* BuildkdTree */
optkdNode *BuildkdTree(double **points, size_t l, size_t u, size_t ndims, size_t *perm) {
    
    optkdNode *p;
    size_t m;
    //mexPrintf("perm[1]: %d.\n",perm[1]);
    
    //NEWTREE(p);
    p = (optkdNode *) mxMalloc(sizeof(optkdNode));
    //if (p==NULL) {mexErrMsgTxt("Cannot allocate memory for tree.\n");return;}
    
    if (u-l+1 <= LEAFSIZE) {
        p->leaf = 1;
        p->lopt = l;
        p->hipt = u;
        p->loson = NULL;
        p->hison = NULL;
    } else {
        p->leaf =0;
        p->discrim = dimmaxrange(l, u, ndims, points, perm); // dimension with largest data range
        m=(l+u)/2; // middle point index
        Selection(points, l, u, m, p->discrim, perm);
        p->cutval = points[perm[m]][p->discrim];
        p->loson = BuildkdTree(points, l, m, ndims, perm);
        p->hison = BuildkdTree(points, m+1, u, ndims, perm);
    }
    return(p);
}


/* dimmaxrange */
size_t dimmaxrange(size_t l, size_t u, size_t ndims, double **points, size_t *perm) {

    size_t i, j, dimmax=0;
    double maxval, minval, maxrange;
    maxval = SMALLDOUBLE;
    minval = LARGEDOUBLE;
    maxrange = SMALLDOUBLE;

    /* find dimension with most range */
    for (i=0; i < ndims; i++) {
        maxval = SMALLDOUBLE;
        minval = LARGEDOUBLE;

        for (j=l; j <= u; j++) {
            //mexPrintf("perm[j]: %d.\n",perm[j]);
            if (maxval < points[perm[j]][i]) {
                maxval = points[perm[j]][i];
            }
            if (minval > points[perm[j]][i]) {
                minval = points[perm[j]][i];
            }
            if (maxrange < fabs(maxval-minval)) {
                maxrange = fabs(maxval-minval);
                dimmax = i;
            }
        }
    }

    return(dimmax);
}


/* Selection */
void Selection(double **points, size_t l, size_t ustart, size_t m, size_t discrim, size_t *perm) {
    //Selection(points, l, u, m, p->discrim);
    double uval;
    size_t i, j, r, tmp;

    r=ustart;

    //while  upperind > lowerind
    while(r>l) {
        uval=points[perm[r]][discrim];
        i=l-1;
        j=r;
        for (;;) {
            while (points[perm[++i]][discrim] < uval);
            while (points[perm[--j]][discrim] > uval && j>l);
            if (i >= j) break;
            tmp=perm[i]; perm[i] = perm[j]; perm[j]=tmp;
        }
        tmp=perm[i]; perm[i] = perm[r]; perm[r]=tmp;

        if (i>=m) r=i-1;
        if (i<=m) l=i+1;
    }
}


/* kdOptNNQuery */
size_t *kdOptNNQuery(double **points, size_t ndims, double *querpoint, unsigned int kns, optkdNode *root, size_t querpointind, double *nndist, size_t *perm) {
    unsigned int j;
    size_t *optfound;
    //mexPrintf("Setting up optfound and nndist.\n");
    /* set up found array */
    optfound = (size_t *) mxMalloc((kns+1)*sizeof(size_t));
    optfound[0]=1;  /* for now */
    
    /* nndist is a priority queue of the distances of the nearest neighbors found for a single point */
    //nndist = (double *) mxMalloc((kns+1)*sizeof(double));
    for (j=0; j < kns+1; j++) nndist[j] = MAMOTHDOUBLE;
    //mexPrintf("Done with that.\n");
    // One point, querpoint, to all other points, points, based on the tree stored that is in the root node's struct, root
    rnnEuclidean(root, querpoint, points, ndims, kns, optfound, querpointind, nndist, perm);
    
    /*for (j=0;j<k;j++)
    printf("%g\n",nndist[j]);
    free(nndist);*/
    return(optfound);
    /*free(optfound);*/
}


/* rnnEuclidean */
void rnnEuclidean(optkdNode *p, double *querpoint, double **points, size_t ndims, 
        unsigned int kns, size_t *optfound, size_t querpointind, double *nndist, size_t *perm) {
    size_t i, j;
    double d, thisdist, val;
    //mexPrintf("nndist: %g, %g, %g, %g, %g.\n",nndist[0],nndist[1],nndist[2],nndist[3],nndist[4]);
    if (p->leaf) {
        for (i=p->lopt; i <= p->hipt; i++) {
            //mexPrintf("perm[i]: %d.\n",perm[i]);
            if ((perm[i])!=querpointind) {
                thisdist=0.0;
                for (j=0; j<ndims; j++) {
                    d=(querpoint[j]-points[perm[i]][j]);
                    thisdist=thisdist+d*d;
                }

                if (optfound[0] < kns) {
                    PQInsert(thisdist, perm[i], nndist, optfound);
                } else {
                    PQreplace(thisdist, nndist, optfound, perm[i]);
                }
            }
        }
    } else {
        // if not a leave, go down the tree (left or right) until we hit a leaf
        val = querpoint[p->discrim] - p->cutval; // go left (low) if the query points is less than val at cut
        if (val < 0) {
            rnnEuclidean(p->loson, querpoint, points, ndims, kns, optfound, querpointind, nndist, perm);
            if (nndist[1] >= val*val) {
                rnnEuclidean(p->hison, querpoint, points, ndims, kns, optfound, querpointind, nndist, perm);
            }
        } else {
            rnnEuclidean(p->hison, querpoint, points, ndims, kns, optfound, querpointind, nndist, perm);
            if (nndist[1] >= val*val) {
                rnnEuclidean(p->loson, querpoint, points, ndims, kns, optfound, querpointind, nndist, perm);
            }
        }
    }
}


void KillOptTree(optkdNode *P) {
  /*if (perm != NULL) {
    free(perm);
  }  /* free permutation array */

  if (P==NULL) {
    return;
  } /* just to be sure */
  if (P->loson != NULL) {
    KillOptTree(P->loson);
  }

  if (P->hison != NULL) {
    KillOptTree(P->hison);
  }

  //free(P);
  mxFree(P);

}
