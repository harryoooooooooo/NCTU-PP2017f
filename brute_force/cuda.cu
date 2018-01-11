#include<stdio.h>
#include<stdbool.h>

typedef unsigned long long int ull;

__device__ bool getval(int v, ull id, ull ie){
    if (v<0) v=-v;
    if (v<=30) return (id & (1llu<<v)) ? true : false;
    return (ie & (1llu<<(v-31))) ? true : false;
}

__device__ bool test(int n, int* raw, ull id, ull ie){
    bool ret = true;
    for (int i = 0; i < n; i+=3){
        bool tmp = false;
        for (int j = 0; j < 3; j++)
            tmp |= (getval(raw[i+j], id, ie) ^ (raw[i+j] < 0));
        ret &= tmp;
    }
    return ret;
}

__device__ void fillres(int m, bool* res, ull id, ull ie){
    for (int i=1;i<=m;i++)
        res[i] = getval(i, id, ie);
}

__global__ void bf(int n, int m, int* raw, bool* res, int* flag){
    ull myid = blockIdx.x * 1024llu + threadIdx.x;
    ull mxstate = (1llu<<m) - 1;
    if (myid > mxstate) return;
    ull end = 1;
    if (m-30 > 0) end <<= m-30;
    for (ull i = 0; i < end; i ++){
        if (test(n, raw, myid<<1, i)){
            if (!atomicExch(flag, 1))
                fillres(m, res, myid<<1, i);
            return;
        }
        if ((i & 0xff) == (myid & 0xff) && *flag)
            return;
    }
}

int main (){
    int *rawd, *raw, *flag;
    bool *resd, *res;
    int n, m, mflag = 0;
    scanf("%d%d", &n,&m);
    n*=3;
    raw = (int*)malloc(sizeof(int)*n);
    res = (bool*)malloc(m+1);
    for (int i=0;i<n;i++)
        scanf("%d", raw+i);
    cudaMalloc((void**)&rawd, sizeof(int)*n);
    cudaMalloc((void**)&resd, m+1);
    cudaMalloc((void**)&flag, sizeof(int));
    cudaMemcpy(rawd, raw, sizeof(int)*n, cudaMemcpyHostToDevice);
    cudaMemcpy(flag, &mflag, sizeof(int), cudaMemcpyHostToDevice);
    bf<<<1048576,1024>>>(n, m, rawd, resd, flag);
    cudaMemcpy(&mflag, flag, sizeof(int), cudaMemcpyDeviceToHost);
    if (mflag){
        cudaMemcpy(res, resd, m+1, cudaMemcpyDeviceToHost);
        for (int i = 1; i <= m; i++)
            printf("%d := %s\n", i, (res[i]?"true":"false"));
    }
    else printf("No satisfy!\n");
    cudaFree(rawd);
    cudaFree(resd);
    cudaFree(flag);
    free(raw);
    free(res);
}
