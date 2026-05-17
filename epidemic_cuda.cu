
#include <iostream>
#include <chrono>

using namespace std;

#define N 1000
#define ITER 100

__global__ void update(int *grid, int *newGrid) {

    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if(i > 0 && i < N-1 && j > 0 && j < N-1) {

        int idx = i * N + j;

        if(grid[idx] == 0) {

            int up    = grid[(i-1)*N + j];
            int down  = grid[(i+1)*N + j];
            int left  = grid[i*N + (j-1)];
            int right = grid[i*N + (j+1)];

            if(up == 1 || down == 1 || left == 1 || right == 1)
                newGrid[idx] = 1;
            else
                newGrid[idx] = 0;

        } else {
            newGrid[idx] = grid[idx];
        }
    }
}

int main() {

    int size = N * N * sizeof(int);

    int *h_grid = new int[N*N];
    int *h_newGrid = new int[N*N];

    for(int i=0;i<N*N;i++) h_grid[i] = 0;

    h_grid[(N/2)*N + (N/2)] = 1;

    int *d_grid, *d_newGrid;

    cudaMalloc(&d_grid, size);
    cudaMalloc(&d_newGrid, size);

    cudaMemcpy(d_grid, h_grid, size, cudaMemcpyHostToDevice);

    dim3 threads(16,16);
    dim3 blocks(N/16, N/16);

    auto start = chrono::high_resolution_clock::now();

    for(int t=0;t<ITER;t++) {

        update<<<blocks, threads>>>(d_grid, d_newGrid);

        cudaDeviceSynchronize();

        int *tmp = d_grid;
        d_grid = d_newGrid;
        d_newGrid = tmp;
    }

    auto end = chrono::high_resolution_clock::now();

    cudaMemcpy(h_grid, d_grid, size, cudaMemcpyDeviceToHost);

    cout << "Simulation CUDA terminée" << endl;
    cout << "Temps CUDA : "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    cudaFree(d_grid);
    cudaFree(d_newGrid);

    delete[] h_grid;
    delete[] h_newGrid;

    return 0;
}
