#include <iostream>
#include <omp.h>
#include <chrono>
using namespace std;

#define ITER 100
#define MAXN 2000

int grid[MAXN][MAXN];
int newGrid[MAXN][MAXN];

long long run(int N) {
    #pragma omp parallel for collapse(2)
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) { grid[i][j]=0; newGrid[i][j]=0; }
    grid[N/2][N/2]=1;

    auto start = chrono::high_resolution_clock::now();
    #pragma omp parallel
    for(int t=0;t<ITER;t++) {
        #pragma omp for collapse(2) schedule(static)
        for(int i=1;i<N-1;i++)
            for(int j=1;j<N-1;j++) {
                if(grid[i][j]==0)
                    newGrid[i][j]=(grid[i-1][j]||grid[i+1][j]||grid[i][j-1]||grid[i][j+1])?1:0;
                else newGrid[i][j]=1;
            }
        #pragma omp for collapse(2) schedule(static)
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) grid[i][j]=newGrid[i][j];
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end-start).count();
}

int main() {
    int sizes[] = {500, 2000};
    for(int s : sizes)
        cout << "N=" << s << " -> " << run(s) << " ms" << endl;
    return 0;
}
