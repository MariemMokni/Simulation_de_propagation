#include <iostream>
#include <chrono>
using namespace std;

#define N 1000

int grid[N][N];
int newGrid[N][N];

long long run(int ITER) {
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) grid[i][j]=0;
    grid[N/2][N/2]=1;

    auto start = chrono::high_resolution_clock::now();
    for(int t=0;t<ITER;t++) {
        for(int i=1;i<N-1;i++)
            for(int j=1;j<N-1;j++) {
                if(grid[i][j]==0)
                    newGrid[i][j]=(grid[i-1][j]||grid[i+1][j]||grid[i][j-1]||grid[i][j+1])?1:0;
                else newGrid[i][j]=1;
            }
        for(int i=0;i<N;i++) for(int j=0;j<N;j++) grid[i][j]=newGrid[i][j];
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end-start).count();
}

int main() {
    int iters[] = {50, 200};
    for(int it : iters)
        cout << "ITER=" << it << " -> " << run(it) << " ms" << endl;
    return 0;
}
