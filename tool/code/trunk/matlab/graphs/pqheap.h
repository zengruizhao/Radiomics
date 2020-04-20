#define SMALLDOUBLE  -999999999.0;
#define LARGEDOUBLE   999999999.0;
#define HUGEDOUBLE  99999999999.0;
#define MAMOTHDOUBLE 999999999999999.0

void PQInsert(double, size_t, double *, size_t *);
void PQupheap(double *, size_t *, size_t);
void PQreplace(double, double *, size_t *, size_t);
void PQdownheap(double *, size_t *, size_t, size_t);

/* PQInsert */
void PQInsert(double distance, size_t index, double *DistArr, size_t *FoundArr) {
//PQInsert(       thisdist,   perm[i],          nndist,      optfound);
    FoundArr[0]=FoundArr[0]+1;       // optfound++
    DistArr[FoundArr[0]] = distance; // nndist[optfound]
    FoundArr[FoundArr[0]] = index;   // optfound[optfound[0]] = perm[i]
    PQupheap(DistArr, FoundArr, FoundArr[0]); // PQupheap(nndist, optfound, optfound[0]);
}

/* PQupheap */
void PQupheap(double *DistArr, size_t *FoundArr, size_t k) {
//PQupheap(         nndist,      optfound, optfound[0])
    double v;
    size_t j;
    
    v=DistArr[k]; DistArr[0] = MAMOTHDOUBLE;
    j=FoundArr[k];
    
    while(DistArr[k/2] <= v) {
        DistArr[k] = DistArr[k/2];
        FoundArr[k] = FoundArr[k/2];
        k=k/2;
    }
    DistArr[k] = v;
    FoundArr[k] = j;
}

/* PQreplace */
void PQreplace(double distance, double *DistArr, size_t *FoundArr, size_t index) {
    DistArr[0]=distance;
    PQdownheap(DistArr, FoundArr, 0, index);
}

/* PQdownheap */
void PQdownheap(double *DistArr, size_t *FoundArr, size_t k, size_t index) {
    
    size_t j, N;
    double v;
    
    v=DistArr[k];
    
    N = FoundArr[0];  /* tricky patch to maintain the data structure */
    FoundArr[0]=index;
    
    while (k <= N/2) {
        j=k+k;
        if (j < N && DistArr[j] <DistArr[j+1]) j++;
        if (v>=DistArr[j]) break;
        DistArr[k]=DistArr[j];
        FoundArr[k]=FoundArr[j];
        k=j;
    }
    
    DistArr[k] = v;
    FoundArr[k]= index;
    FoundArr[0]=N;  /* restore data struct */
}
