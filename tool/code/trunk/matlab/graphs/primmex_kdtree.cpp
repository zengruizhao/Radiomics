#include "mex.h"
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include "knnkdtree.h"
#include <typeinfo>
#include <limits>
//#include <utility>
//#include <sstream>
//#include <string>

/* primmex_kdtree - Generate minimal spanning tree using Prim's method on a
 * kNN graph constructed using efficient kd-trees.
 * Jonathan Chappelow
 */

bool greater (int i,int j) { return (i>j); }

bool psecondgreater( const std::pair<size_t,double>& i, const std::pair<size_t,double>& j ) {
    return j.second < i.second;
}

bool psecondless( const std::pair<size_t,double>& i, const std::pair<size_t,double>& j ) {
    return i.second < j.second;
}

bool pfirstgreater( const std::pair<double,size_t>& i, const std::pair<double,size_t>& j ) {
    return j.first < i.first;
}

bool pfirstless( const std::pair<double,size_t>& i, const std::pair<double,size_t>& j ) {
    return i.first < j.first;
}

double edist(double *X, size_t npts, size_t ndims, size_t Zind, size_t Yind) {
    double dist=0.0, tmp;
    size_t i;
    
    for (i=0; i<ndims; i++) {
        //tmp = X[Zind][i] - X[Yind][i];
        tmp = X[Zind+i*npts] - X[Yind+i*npts];
        dist += tmp*tmp;
    }
    //dist=sqrt(dist);
    
    return dist;
}

size_t prim(double *X, double *Vx, double *Vy, double *E, size_t npts, size_t ndims, unsigned int kns, double *knnlen) {
    size_t *Zinds, Ysize, Zsize, *Yindinds, *iNNs, *perm, *usedneighbors;
    long int *Yinds;
    size_t i, j, nearestY, nearestZ, zi, yi, nzyi, ki, kiofnearestY, max_size_t;
    size_t **kNNgraph;
    double **kNNdists;
    double mindist, *Xi, **Xmat, *nndist; //, dist;
    double knngraphlen[1]={0};
    bool foundneighbor;
    int *remainingNNsinZ;
    optkdNode *kdrootpt;
    //size_t comparisons=0;
    //double Inf=numeric_limits<double>::infinity()
    
    //std::pair<size_t,double> temppair;
    //std::vector<std::pair<size_t,double>> kNNgvect; // must use push_back
    //std::vector<std::pair<size_t,double>> kNNgvect (kns,std::make_pair(0,0.0));
    //std::pair<double,size_t> temppair;
    //std::vector<std::pair<double,size_t>> kNNgvect; // must use push_back
    std::vector<std::pair<double,size_t>> kNNgvect (kns,std::make_pair(0.0,0));
    
    // Zinds array
    Zinds = (size_t *) mxMalloc(npts*sizeof(size_t));
    // Initialize Zinds to all but the first element in Y
    for (zi=0; zi<npts; zi++) Zinds[zi]=zi;
//     Zsize = npts;
    
    //mexPrintf("yi is of type %s (size_t).\n",typeid(yi).name());
    //mexPrintf("Maximum value of size_t is: %u (%d).\n",std::numeric_limits<size_t>::max(),std::numeric_limits<size_t>::max());
    max_size_t=std::numeric_limits<size_t>::max();
    
    /* Calculated kNN graph */
    //kNNgraph = (size_t *) mxMalloc(npts*(size_t) kns*sizeof(size_t));
    //kNNdists = (double *) mxMalloc(npts*(size_t) kns*sizeof(double));
    kNNgraph = (size_t **) mxMalloc(npts*sizeof(size_t *));
    for (yi=0; yi<npts; yi++) kNNgraph[yi]=(size_t *) mxMalloc((size_t) kns*sizeof(size_t));
    kNNdists = (double **) mxMalloc(npts*sizeof(double *));
    for (yi=0; yi<npts; yi++) kNNdists[yi]=(double *) mxMalloc((size_t) kns*sizeof(double));
    //knnkdtree(X, npts, ndims, kns, knngraphlen, kNNgraph, kNNdists,1);
    
    // Put data in Matrix format
    Xmat = (double **) mxMalloc(npts*sizeof(double *));
    for (i = 0; i < npts; i++) Xmat[i] = (double *) mxMalloc(ndims*sizeof(double));
    for (ki = 0, i = 0; ki < ndims; ki++) {
        for (yi = 0; yi < npts; yi++, i++) {
            Xmat[yi][ki] = X[i];
        }
    }
    
    // Allocate for test point
    Xi=(double *) mxMalloc(ndims*sizeof(double));
    //mexPrintf("Done reallocating X array into matrix.\n");

    /* Build kd-tree, root node contains all other nodes, hence the tree */
    //kdrootpt = BuildOptTree(Xmat, npts, ndims, perm);
    /* initialize perm since it is no longer a global in knnkdtree.h */
    perm = (size_t *) mxMalloc(npts*sizeof(size_t));
    for (j=0; j < npts; j++) perm[j]=j;
    kdrootpt=BuildkdTree(Xmat, 0, npts-1, ndims, perm);
//     mexPrintf("Done building kd-tree.\n");
//     mexEvalString("drawnow");
    
    /* initialize nndist since it is no longer a global in knnkdtree.h */
    nndist = (double *) mxMalloc((kns+1)*sizeof(double));
    for (j=0; j < kns+1; j++) nndist[j] = HUGEDOUBLE;
    
    /* Find the NNs of each point using the kd-tree */
    //i=0;
    for (yi = 0; yi < npts; yi++) {
        // Grab test point yi (each dimension j)
        for (j = 0; j < ndims; j++)
            Xi[j]=Xmat[yi][j];
            //Xi[j]=X[j+yi*kns];
        
        // Find the kNNs for the test point yi/Xi (gets iNNs and nndist!)
        iNNs = kdOptNNQuery(Xmat, ndims, Xi, kns, kdrootpt, yi, nndist, perm);
        //mexPrintf("iNNs: %d %d %d %d %d %d %d.\n",iNNs[kns],iNNs[kns-1],iNNs[kns-2],iNNs[kns-3],iNNs[kns-4],iNNs[kns-5],iNNs[kns-6]);
        //mexPrintf("nndist: %f %f %f %f %f %f %f.\n",nndist[kns],nndist[kns-1],nndist[kns-2],nndist[kns-3],nndist[kns-4],nndist[kns-5],nndist[kns-6]);
        
        // iNNs[0] is a dummy.
        for (ki = kns, j=0; ki > 0; ki--, j++) {
            //kNNdists[i]=sqrt(nndist[ki]);
            //kNNdists[yi][j]=sqrt(nndist[ki]);
            //kNNgvect[j].first=sqrt(nndist[ki]);
            
            //kNNgraph[i] = iNNs[ki];//+1;
            //i++;
            //kNNgraph[yi][j]=iNNs[ki];
            //kNNgvect[j].second=iNNs[ki];
            
            kNNgvect[j]=std::make_pair(sqrt(nndist[ki]), iNNs[ki]); //(dist,ind)
        }
        
        //std::sort(kNNgvect.begin(), kNNgvect.end(), psecondless); // if .second is the distance
        std::sort(kNNgvect.begin(), kNNgvect.end());
        
        //mexPrintf("Sorted neighbors of point %d.\n",yi);
        
        for (ki = 0; ki<kns; ki++) {
            kNNdists[yi][ki]=(kNNgvect[ki]).first;
            kNNgraph[yi][ki]=(kNNgvect[ki]).second;
            *knnlen+=kNNdists[yi][ki];
        }
    }
    
    //mxFree(Xmat); mxFree(perm);
    //mxFree(nndist); mxFree(Xi);
    
    //KillOptTree(kdrootpt);
//     mexPrintf("Done calculating kNNdists.\n");
//     mexEvalString("drawnow");
    
    /* Track number of remaining neighbors that are in Z for each point */
    remainingNNsinZ = (int *) mxMalloc(npts*sizeof(int));
    for (i=0; i<npts; i++) remainingNNsinZ[i]=1; // just needs to be >0
    usedneighbors = (size_t *) mxCalloc(npts,sizeof(size_t));
    //for (i=0; i<npts; i++)
    //    for (ki=0; ki<kns; ki++)
    //        if (kNNgraph[ki+i*kns]>1) remainingNNsinZ[i]++;
    
    /* Array for Y inds and non-zero elements of Yinds*/
    //Yinds = new size_t[npts];
    Yinds = (long int *) mxMalloc(npts*sizeof(long int));
    for (yi=0; yi<npts; yi++) Yinds[yi]=-1;
    //Yinds = (size_t *) mxCalloc(npts, sizeof(size_t));
    Yindinds = (size_t *) mxMalloc(npts*sizeof(size_t));
    // Copy first node index of X into Y
    Yinds[0]=0;
    Yindinds[0]=0;
    Ysize=1;
    
    /* Build the MST one edge at a time, npts-1 total edges */
    for (i=0; i<(npts-1); i++) {        
        /* Use kNN graph to find nearest elements -- Zinds shrinks, Yinds grows */
        //mindist=kNNdists[Yinds[Yindinds[Ysize-1]]][usedneighbors[Yinds[Yindinds[Ysize-1]]]] + 0.1;
        mindist=9999999999.0;
		foundneighbor=false;
        /* Check each Y */
        //for (yi=0; yi<Ysize; yi++) {
        for (yi=(Ysize-1); yi!=max_size_t; yi--) /* max_size_t or -1 */ {
            nzyi=Yindinds[yi]; // location of current non-zero Yind
            /* If current Y has neighbors in Z */
            if (remainingNNsinZ[Yinds[nzyi]]>0) {
                remainingNNsinZ[Yinds[nzyi]]=0;
                /* Check each neighbor of Y */
                for (ki=usedneighbors[Yinds[nzyi]]; ki<kns; ki++) {
                    //mexPrintf("\tChecking neighbor %d (%d) of %d, with distance %f.\n",ki,kNNgraph[Yinds[nzyi]][ki],Yinds[nzyi],kNNdists[Yinds[nzyi]][ki]);
                    /* Is neighbor in z? */
                    if (Yinds[kNNgraph[Yinds[nzyi]][ki]]<0) {
                        //comparisons++;
                        if (kNNdists[Yinds[nzyi]][ki]<mindist) {
                            //mexPrintf("\t\tNeighbor %d is now the closest to Y!\n",ki);
                            mindist=kNNdists[Yinds[nzyi]][ki];
                            nearestZ=kNNgraph[Yinds[nzyi]][ki];
                            nearestY=yi;
                            kiofnearestY=ki;
                            foundneighbor=true;
                        }
                        //mexPrintf("\t\tMoving to next neighbor of Y.\n");
                        remainingNNsinZ[Yinds[nzyi]]=1;
                        break; // break regardless because other neighbors are only further
                    } else {
                        usedneighbors[Yinds[nzyi]]=ki+1;
                    }
                }
                // If we get here, then remainingNNsinZ[Yinds[nzyi]] will still be 0
            } //else { mexPrintf("\t\tSkipping this Y since it has no neighbors in Z.\n"); }
        }
        //mexPrintf("yi=%u.\n",yi);
        
        //if ((yi==0) && (ki=(kns-1))) {
        if (!foundneighbor) {
            mexWarnMsgTxt("All points not connected.\n");
            return Ysize-1;
        }
        
        /* Once next edge is found, store it in our output structures */
        E[i]=(mindist);
        nzyi=Yindinds[nearestY];
        usedneighbors[Yinds[nzyi]]=kiofnearestY+1;
        //mexPrintf("Shortest edge from %d (Z) to %d (Y) with distance %f.\n",Zinds[nearestZ],Yinds[nzyi],mindist);
        
        if (Yinds[nzyi]<Zinds[nearestZ]) {
            Vy[i]=Yinds[nzyi]+1; // +1 for MATLAB
            Vx[i]=Zinds[nearestZ]+1;
        } else {
            Vx[i]=Yinds[nzyi]+1;
            Vy[i]=Zinds[nearestZ]+1;
        }
        
        /* Move Zind into Yind */
        Yindinds[i+1]=nearestZ;
        Yinds[nearestZ]=Zinds[nearestZ];
        // Zinds stay the same so we can address them with kNNgraph
        
        //mexPrintf("Adding node %d to Y (element %d of Z).\n",Zinds[nearestZ],nearestZ);
        
        Ysize++;
//         Zsize--;
        
        //mexPrintf("Yinds:");
        //for (yi=0; yi<npts; yi++) mexPrintf(" %2d",Yinds[yi]);
        //mexPrintf(".\n");
        
//         if ((i % 100)==0) {
//         mexPrintf("Nodes left in Z: %d.\n",Zsize);
//         mexPrintf("Nodes now in Y:  %d.\n",Ysize);
//         }
    }
    /* Done building MST */
//     mexPrintf("Done building MST.\n");
//     mexEvalString("drawnow");
    
    //mxFree(Yinds);
    //mxFree(Zinds);
    //delete[] Yinds;
    //delete[] Zinds;
    //mxFree(Y);
    //mxFree(Z);
    
    //mexPrintf("Total number of edges compared: %d.\n",comparisons);
    
    return Ysize-1;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    size_t ndims, npts;
    double *X;
    double *Vx, *Vy, *E, *lastedge, *knnlen;
    //mwSize vsize[2];
    //double gamma=1;
    unsigned int k;
    
    lastedge=(double *) mxCalloc(1,sizeof(double));
    if (nlhs<5) knnlen=(double *) mxCalloc(1,sizeof(double));
    
    if ((nlhs>5) || (nlhs<3))
        mexErrMsgTxt("Wrong number of output parameters, usage:\n\t[Vx,Vy,E] = primmex_kdtree(X,k) OR [Vx,Vy,E,lastedge,knnlen] = primmex_kdtree(X,k)");
    if (nrhs!=2)
        mexErrMsgTxt("Wrong number of input parameters, usage:\n\t[Vx,Vy,E,lastedge,knnlen] = primmex_kdtree(X,k)");
    
    if (!mxIsDouble(prhs[0]))
        mexErrMsgTxt("Input not double.");
    
    X=mxGetPr(prhs[0]);
    
    /* [rows][cols], [npts][ndims] */
    npts=mxGetM(prhs[0]);
    ndims=mxGetN(prhs[0]);
    
    //mexPrintf("npts is of type %s (size_t).\n",typeid(npts).name());
    
    //k=(unsigned int) floor(ndims*3.5)+1;
    k=(unsigned int) mxGetScalar(prhs[1]);
    
    //mexPrintf("Searching with k=%d.\n",k);
    
    //vsize[0]=npts-1;
    //vsize[1]=ndims;
    
    plhs[0]=mxCreateDoubleMatrix(npts-1, 1, mxREAL); //mxCreateNumericArray(2, vsize, mxDOUBLE_CLASS, mxREAL);
    Vx = mxGetPr(plhs[0]);
    plhs[1]=mxCreateDoubleMatrix(npts-1, 1, mxREAL); //mxCreateNumericArray(2, vsize, mxDOUBLE_CLASS, mxREAL);
    Vy = mxGetPr(plhs[1]);
    plhs[2]=mxCreateDoubleMatrix(npts-1, 1, mxREAL);
    E = mxGetPr(plhs[2]);
    if (nlhs>3) {
        plhs[3]=mxCreateDoubleMatrix(1, 1, mxREAL);
        lastedge=mxGetPr(plhs[3]);
        if (nlhs>4) {
            plhs[4]=mxCreateDoubleMatrix(1, 1, mxREAL);
            knnlen=mxGetPr(plhs[4]);
        }
    }
    
    *lastedge=(double) prim(X,Vx,Vy,E,npts,ndims,k,knnlen);
}
