#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>

#define NUM_THREAD 512
#define NUM_BLOCK 65

// Function to sum an array
__global__ void reduce0(float *g_odata) {
extern __shared__ int sdata[];

// each thread loads one element from global to shared mem
unsigned int tid = threadIdx.x;
unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
sdata[tid] = g_odata[i];
__syncthreads();

// do reduction in shared mem
for (unsigned int s=1; s < blockDim.x; s *= 2) { // step = s x 2
    if (tid % (2*s) == 0) { // only threadIDs divisible by the step participate
        sdata[tid] += sdata[tid + s];
    }
    __syncthreads();
}

// write result for this block to global mem
if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
__global__ void monteCarlo(float *g_odata, int  trials, curandState *states){
    extern __shared__ int sdata[];
//  unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int k, incircle;
    float x, y, z;
    incircle = 0;

    curand_init(1234, i, 0, &states[i]);

    for(k = 0; k < trials; k++){

    x = curand_uniform(&states[i]);
    y = curand_uniform(&states[i]);
    z = sqrt(x*x + y*y);
    if (z <= 1) incircle++;
    else{}
    }
    __syncthreads();
    g_odata[i] = incircle;
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
int main() {

    float* solution = (float*)calloc(100, sizeof(float));
    float *sumDev, sumHost[NUM_BLOCK*NUM_THREAD];
    int trials, total; 
    curandState *devStates;



    trials = 100;
    total = trials*NUM_THREAD*NUM_BLOCK;

    dim3 dimGrid(NUM_BLOCK,1,1); // Grid dimensions
    dim3 dimBlock(NUM_THREAD,1,1); // Block dimensions
    size_t size = NUM_BLOCK*NUM_THREAD*sizeof(float); //Array memory size
    cudaMalloc((void **) &sumDev, size); // Allocate array on device

//    cudaMalloc((void **) &devStates, size*sizeof(curandState));
cudaMalloc((void **) &devStates, (NUM_BLOCK *NUM_THREAD)*sizeof(curandState));
    // Do calculation on device by calling CUDA kernel
    monteCarlo <<<dimGrid, dimBlock, size>>> (sumDev, trials, devStates);
        // call reduction function to sum
    reduce0 <<<dimGrid, dimBlock, size>>> (sumDev);
    // Retrieve result from device and store it in host array
    cudaMemcpy(sumHost, sumDev, size, cudaMemcpyDeviceToHost);

    *solution = 4*(sumHost[0]/total);
    printf("\n%.*f\n", 1000, *solution);
    free (solution);
    //*solution = NULL;
    return 0;
}