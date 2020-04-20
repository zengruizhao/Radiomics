#include "mex.h"
#include "pqheap.h"
#include <math.h>
#include "maxNumCompThreads.h"

/*   undef needed for LCC compiler  */
#undef EXTERN_C
#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <pthread.h>
#endif

#ifndef min
#define min(X, Y)  (X < Y ? X : Y)
#endif

#define LEAFSIZE 50

/* kd Node struct */
typedef struct optkdnode {
  int leaf;  /* true if the node is a leaf node */
  size_t discrim; /* discriminator of node */
  double cutval; 
  struct optkdnode *loson, *hison;
  size_t lopt,hipt;
} optkdNode;

/* ThreadArgs structure for queryallpts thread function */
struct queryallpts_threadargs {
    double **points;
    size_t ndims;
    size_t npts;
    unsigned int kns;
    optkdNode *root;
    size_t *perm;
    double gamma;
    size_t ThreadID;
    size_t Nthreads;
    double *Graph;
    double *dists;
    double len;
};

/* ThreadArgs structure for BuildkdTree thread function */
struct BuildkdTree_threadargs {
    double **points;
    size_t l;
    size_t u;
    size_t ndims;
    size_t *perm;
    optkdNode *p;
};

/* functions */
static void knnkdtree (double *, size_t, size_t, unsigned int, double * , double *, double *, double);
static void knnkdtree_threaded (double *, size_t, size_t, unsigned int, double * , double *, double *, double, size_t);

#ifdef _WIN32
unsigned __stdcall queryallpts_threadfunc(void *ThreadArgsV); //queryallpts_threadargs *ThreadArgsV
#else
void *queryallpts_threadfunc(void *ThreadArgsV); // C++ needs void*
#endif

optkdNode *BuildkdTree(double **, size_t, size_t, size_t, size_t *);
    size_t dimmaxrange(size_t, size_t, size_t, double **, size_t *);
    void Selection(double **, size_t, size_t, size_t, size_t, size_t *);
optkdNode *BuildkdTree_threaded(double **, size_t, size_t, size_t, size_t *);
#ifdef _WIN32
unsigned __stdcall BuildkdTree_threadfunc(void *ThreadArgsV); //queryallpts_threadargs *ThreadArgsV
#else
void *BuildkdTree_threadfunc(void *ThreadArgsV); // C++ needs void*
#endif
    
size_t *kdOptNNQuery(double **, size_t, double *, unsigned int, optkdNode *, size_t, double *, size_t *);
    void rnnEuclidean(optkdNode *, double *, double **, size_t, unsigned int, size_t *, size_t, double *, size_t *);

void KillOptTree(optkdNode *);

/* GLOBALS */
size_t THREADLIMIT=8;

/* knnkdtree -- single threaded */
void knnkdtree (double *X, size_t npts, size_t ndims, unsigned int kns, double *len, double *Graph, double *dists, double gamma) {
    double **Points;
    double *querpoint;
    double *nndist;
    size_t *perm;
    optkdNode *OptkdRoot;
    size_t *Found;
    size_t i, j, k;
    
    *len = 0.0;
    
    querpoint=(double *) malloc((mwSize)ndims*sizeof(double));
    
    Points = (double **) malloc((mwSize)npts*sizeof(double *));
    for (k = 0; k < npts; k++)
        Points[k] = (double *) malloc((mwSize)ndims*sizeof(double));
    
    /*
    for (k = 0, i = 0; k < npts; k++)
        for (j = 0; j < ndims; j++, i++)
            Points[k][j] = X[i];
    */
    for (j = 0, i = 0; j < ndims; j++)
        for (k = 0; k < npts; k++, i++)
            Points[k][j] = X[i];
    
    // Build kd tree, root node contains all other nodes, hence the tree
    perm = (size_t *) malloc(npts*sizeof(size_t));
    for (j=0; j < npts; j++) perm[j]=j;
    OptkdRoot=BuildkdTree(Points, 0, npts-1, ndims, perm);
    
    nndist = (double *) malloc((kns+1)*sizeof(double));
    for (j=0; j < kns+1; j++) nndist[j] = HUGEDOUBLE;
    
    // Find kNNs for each point by traversing kd tree
    for (i = 0, k = 0; i < npts; i++) {
        // Grab query point i (each dimension j)
        for (j = 0; j < ndims; j++)
            querpoint[j]=Points[i][j];
        
        // Find the kNNs for the query point
        Found = kdOptNNQuery(Points, ndims, querpoint, kns, OptkdRoot, i, nndist, perm);
        
        for (j = kns; j >= 1; j--, k++) /*Found[0] is a dummy. */ {
            dists[k]=sqrt(nndist[j]);
            *len += (gamma>1 ? pow(dists[k], gamma) : dists[k]);
            Graph[k] = (double) Found[j] + 1;
        }
    }
    
    KillOptTree(OptkdRoot);
    OptkdRoot=NULL;
    free(Found);
    for (k=0;k<npts;k++) free(Points[k]);
    free(Points);
    free(querpoint);
    free(perm);
    free(nndist);
}

/* knnkdtree_threaded -- multi-threaded*/
void knnkdtree_threaded (double *X, size_t npts, size_t ndims, unsigned int kns, 
        double *len, double *Graph, double *dists, double gamma, size_t threadlim) {
    double **Points;
    size_t *perm;
    optkdNode *OptkdRoot;
    size_t i, j, k;
    
    *len = 0.0;
    
    size_t *ThreadID;
    queryallpts_threadargs **ThreadArgs;
    queryallpts_threadargs *ThreadArgsi;
    
    /* Handles to the worker threads */
	#ifdef _WIN32
		HANDLE *ThreadList; 
    #else
		pthread_t *ThreadList;
	#endif
    
    size_t Nthreads;
    size_t maxthreads=(size_t) getNumCores();
    Nthreads=(maxthreads>THREADLIMIT ? THREADLIMIT : maxthreads); // hard thread limit
    if (Nthreads>threadlim) Nthreads=threadlim; // input thread limit
//     mexPrintf("%d\n",Nthreads);
//     Nthreads=(Nthreads>threadlimit ? threadlimit : Nthreads); // input thread limit
    
    Points = (double **) malloc((size_t)npts*sizeof(double *));
    for (k = 0; k < npts; k++)
        Points[k] = (double *) malloc((size_t)ndims*sizeof(double));
    
    for (j = 0, i = 0; j < ndims; j++)
        for (k = 0; k < npts; k++, i++)
            Points[k][j] = X[i];
    
    // Build kd tree, root node contains all other nodes, hence the tree
    perm = (size_t *) malloc(npts*sizeof(size_t));
    for (j=0; j < npts; j++) perm[j]=j;
    
    if (maxthreads>1 && npts>=150e+3)
        OptkdRoot=BuildkdTree_threaded(Points, 0, npts-1, ndims, perm);
    else
        OptkdRoot=BuildkdTree(Points, 0, npts-1, ndims, perm);
    
    /* Reserve room for handles of threads in ThreadList */
    #ifdef _WIN32
		ThreadList = (HANDLE *) malloc(Nthreads*sizeof(HANDLE));
    #else
		ThreadList = (pthread_t *) malloc(Nthreads*sizeof(pthread_t));
	#endif
	
	ThreadID = (size_t *) malloc(Nthreads*sizeof(size_t));
	ThreadArgs = (queryallpts_threadargs **) malloc(Nthreads*sizeof(queryallpts_threadargs *));
    
    // Find kNNs for each point by traversing kd tree
    for (i=0; i<Nthreads; i++) {
        
        ThreadID[i]=i;
        
        ThreadArgsi = (queryallpts_threadargs *) malloc(sizeof(queryallpts_threadargs));
        ThreadArgsi->points=Points;
        ThreadArgsi->ndims=ndims;
        ThreadArgsi->npts=npts;
        ThreadArgsi->kns=kns;
        ThreadArgsi->root=OptkdRoot;
        ThreadArgsi->perm=perm;
        ThreadArgsi->gamma=gamma;
        ThreadArgsi->ThreadID=ThreadID[i];
        ThreadArgsi->Nthreads=Nthreads;
        ThreadArgsi->Graph=Graph;
        ThreadArgsi->dists=dists;
        ThreadArgsi->len=0;

        /* Start thread  */
        ThreadArgs[i]=ThreadArgsi; // now we can overwrite ThreadArgsi for the next thread
        
        #ifdef _WIN32
            ThreadList[i] = (HANDLE)_beginthreadex( NULL, 0, &queryallpts_threadfunc, ThreadArgs[i] , 0, NULL );
        #else
            pthread_create((pthread_t*)&ThreadList[i], NULL, &queryallpts_threadfunc, ThreadArgs[i]);
        #endif
    }
    
    #ifdef _WIN32
            for (i=0; i<Nthreads; i++) WaitForSingleObject(ThreadList[i], INFINITE);
            for (i=0; i<Nthreads; i++) CloseHandle( ThreadList[i] );
    #else
            for (i=0; i<Nthreads; i++) pthread_join(ThreadList[i], NULL);
    #endif
    
    for (i=0; i<Nthreads; i++) *len+=ThreadArgs[i]->len;
    
	for (i=0; i<Nthreads; i++) free(ThreadArgs[i]);
    
    free(ThreadArgs);
    free(ThreadID);
    free(ThreadList);
    
    KillOptTree(OptkdRoot);
    OptkdRoot=NULL;
    for (k=0;k<npts;k++) free(Points[k]);
    free(Points);
    free(perm);
    
}

/* queryallpts_threadfunc(queryallpts_threadargs *) */
#ifdef _WIN32
unsigned __stdcall queryallpts_threadfunc(void *ThreadArgsV) {
#else
void *queryallpts_threadfunc(void *ThreadArgsV) {
#endif
    size_t i,j,ij,k;
    unsigned int kns;
    double *querpoint, *nndist;
    size_t *optfound;
    size_t ndims, npts, ThreadOffset, kstart, kskip, Nthreads;
    
    queryallpts_threadargs *ThreadArgs=(queryallpts_threadargs *) ThreadArgsV;
    
    ndims=ThreadArgs->ndims;
    npts=ThreadArgs->npts;
    kns=ThreadArgs->kns;
    ThreadOffset=ThreadArgs->ThreadID;
    Nthreads=ThreadArgs->Nthreads;

    querpoint=(double *) malloc((size_t)ThreadArgs->ndims*sizeof(double));
    optfound = (size_t *) malloc((ThreadArgs->kns+1)*sizeof(size_t));
    nndist = (double *) malloc((ThreadArgs->kns+1)*sizeof(double));
    
    kstart=ThreadOffset*kns;
    kskip=(Nthreads-1)*kns;
    for (i=ThreadOffset, k=kstart; i<npts; i=i+Nthreads, k+=kskip) {
        // Grab query point i (each dimension j)
        for (j = 0; j < ndims; j++) querpoint[j]=ThreadArgs->points[i][j];
        
        // Find the kNNs for the query point
        optfound[0]=1;  /* for start */
        
        for (ij=0; ij < kns+1; ij++) {
            nndist[ij] = MAMOTHDOUBLE;
            //optfound[ij]=ij;
        }
        
        // rnnEuclidean sets optfound and nndist for point i
        rnnEuclidean(ThreadArgs->root, querpoint, ThreadArgs->points, ndims, kns, optfound, i, nndist, ThreadArgs->perm);
        
        // Construct the graph and calculate total edge length
        for (j = kns; j > 0; j--, k++) /* optfound[0] is a dummy. */ {
            ThreadArgs->dists[k]=sqrt(nndist[j]);
            ThreadArgs->len += (ThreadArgs->gamma>1 ? pow(ThreadArgs->dists[k], ThreadArgs->gamma) : ThreadArgs->dists[k]);
            ThreadArgs->Graph[k] = (double) optfound[j] + 1;
        }
    }
    
    free(querpoint);
    free(optfound);
    free(nndist);
    
    /* end thread */
    #ifdef _WIN32
	_endthreadex( 0 );
    return 0;
	#else
	pthread_exit(NULL);
	#endif
    
}


/* kdOptNNQuery */
size_t *kdOptNNQuery(double **points, size_t ndims, double *querpoint, unsigned int kns, optkdNode *root, size_t querpointind, double *nndist, size_t *perm) {
    unsigned int j;
    size_t *optfound;
	
    /* set up found array */
    optfound = (size_t *) malloc((kns+1)*sizeof(size_t));
    optfound[0]=1;  /* for now */
    
    /* nndist is a priority queue of the distances of the nearest neighbors found for a single point */
    for (j=0; j < kns+1; j++) nndist[j] = MAMOTHDOUBLE;

    // One point, querpoint, to all other points, points, based on the tree stored that is in the root node's struct, root
    rnnEuclidean(root, querpoint, points, ndims, kns, optfound, querpointind, nndist, perm);
    
    //free(nndist);
    return(optfound);
}


/* BuildkdTree -- recursive */
optkdNode *BuildkdTree(double **points, size_t l, size_t u, size_t ndims, size_t *perm) {
    
    optkdNode *p;
    size_t m;
    
    p = (optkdNode *) malloc(sizeof(optkdNode));
    
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

/* BuildkdTree_threaded -- start two BuildkdTree threads */
optkdNode *BuildkdTree_threaded(double **points, size_t l, size_t u, size_t ndims, size_t *perm) {
    
    optkdNode *p;
    size_t m, i;
    
    p = (optkdNode *) malloc(sizeof(optkdNode));
    
    /* Handles to the worker threads */
	#ifdef _WIN32
		HANDLE *ThreadList = (HANDLE *) malloc(2*sizeof(HANDLE));; 
    #else
		pthread_t *ThreadList = (pthread_t *) malloc(2*sizeof(pthread_t));;
	#endif
    
    /* Assume there are at least two cores if we got called. */
    size_t Nthreads=2;
//     size_t Nthreads;
//     size_t maxthreads=(size_t) getNumCores();
//     Nthreads=(maxthreads>2 ? 2 : maxthreads);
	
	BuildkdTree_threadargs *ThreadArgs1 = (BuildkdTree_threadargs *) malloc(sizeof(BuildkdTree_threadargs));
    BuildkdTree_threadargs *ThreadArgs2 = (BuildkdTree_threadargs *) malloc(sizeof(BuildkdTree_threadargs));
    
    if (u-l+1 <= LEAFSIZE) {
        p->leaf = 1;
        p->lopt = l;
        p->hipt = u;
        p->loson = NULL;
        p->hison = NULL;
    } else {
        p->leaf = 0;
        p->discrim = dimmaxrange(l, u, ndims, points, perm); // dimension with largest data range
        m=(l+u)/2; // middle point index
        Selection(points, l, u, m, p->discrim, perm);
        p->cutval = points[perm[m]][p->discrim];
        
        // Calculate low tree
        ThreadArgs1->points=points;
        ThreadArgs1->l=l;
        ThreadArgs1->u=m;
        ThreadArgs1->ndims=ndims;
        ThreadArgs1->perm=perm;
        ThreadArgs1->p=(optkdNode *) malloc(sizeof(optkdNode));
        #ifdef _WIN32
            ThreadList[0] = (HANDLE)_beginthreadex( NULL, 0, &BuildkdTree_threadfunc, ThreadArgs1 , 0, NULL );
        #else
            pthread_create((pthread_t*)&ThreadList[0], NULL, &BuildkdTree_threadfunc, ThreadArgs1);
        #endif
        
        // If only one core, wait for first thread to finish before starting next
//         if (Nthreads<2) {
//         #ifdef _WIN32
//             WaitForSingleObject(ThreadList[0], INFINITE);
//         #else
//             pthread_join(ThreadList[0], NULL);
//         #endif
//         }
        
        // Calculate high tree
        ThreadArgs2->points=points;
        ThreadArgs2->l=m+1;
        ThreadArgs2->u=u;
        ThreadArgs2->ndims=ndims;
        ThreadArgs2->perm=perm;
        ThreadArgs2->p=(optkdNode *) malloc(sizeof(optkdNode));
        #ifdef _WIN32
            ThreadList[1] = (HANDLE)_beginthreadex( NULL, 0, &BuildkdTree_threadfunc, ThreadArgs2 , 0, NULL );
        #else
            pthread_create((pthread_t*)&ThreadList[1], NULL, &BuildkdTree_threadfunc, ThreadArgs2);
        #endif
        
        #ifdef _WIN32
            for (i=0; i<2; i++) WaitForSingleObject(ThreadList[i], INFINITE);
            for (i=0; i<2; i++) CloseHandle( ThreadList[i] );
        #else
            for (i=0; i<2; i++) pthread_join(ThreadList[i], NULL);
        #endif
        
        p->loson = ThreadArgs1->p;
        p->hison = ThreadArgs2->p;
        
        free(ThreadArgs1);
        free(ThreadArgs2);
        //free(ThreadID);
        free(ThreadList);
    }
    return(p);
}

/* BuildkdTree_threaded -- recursive, threadargs struct input */
#ifdef _WIN32
unsigned __stdcall BuildkdTree_threadfunc(void *ThreadArgsV) {
#else
void *BuildkdTree_threadfunc(void *ThreadArgsV) {
#endif
    size_t m;
    
    BuildkdTree_threadargs *ThreadArgs = (BuildkdTree_threadargs *) ThreadArgsV;
    
    double **points = ThreadArgs->points;
    size_t l = ThreadArgs->l;
	size_t u = ThreadArgs->u;
	size_t ndims = ThreadArgs->ndims;
	size_t *perm = ThreadArgs->perm;
    optkdNode *p = ThreadArgs->p;
    
    if (u-l+1 <= LEAFSIZE) {
        p->leaf = 1;
        p->lopt = l;
        p->hipt = u;
        p->loson = NULL;
        p->hison = NULL;
    } else {
        p->leaf = 0;
        p->discrim = dimmaxrange(l, u, ndims, points, perm); // dimension with largest data range
        m=(l+u)/2; // middle point index
        Selection(points, l, u, m, p->discrim, perm);
        p->cutval = points[perm[m]][p->discrim];
        p->loson = BuildkdTree(points, l, m, ndims, perm);
        p->hison = BuildkdTree(points, m+1, u, ndims, perm);
    }
    
    /* end thread */
    #ifdef _WIN32
	_endthreadex( 0 );
    return 0;
	#else
	pthread_exit(NULL);
	#endif
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


/* rnnEuclidean -- recursive */
void rnnEuclidean(optkdNode *p, double *querpoint, double **points, size_t ndims, 
        unsigned int kns, size_t *optfound, size_t querpointind, double *nndist, size_t *perm) {
    size_t i, j;
    double d, thisdist, val;
    if (p->leaf) {
        for (i=p->lopt; i <= p->hipt; i++) {
            if ((perm[i])!=querpointind) {
                thisdist=0.0;
                // calcualte the Euclidean distance (squared)
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


/* KillOptTree -- free memory used by kd tree */
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

  free(P);
}
