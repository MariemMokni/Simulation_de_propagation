
#include <iostream>
#include <chrono>
using namespace std;

#define N 1000
#define ITER 100

int grid[N][N];
int newGrid[N][N];

int main() {

    auto start = chrono::high_resolution_clock::now();

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }

    grid[N/2][N/2] = 1;

    for(int t = 0; t < ITER; t++) {

        for(int i = 1; i < N - 1; i++) {
            for(int j = 1; j < N - 1; j++) {

                if(grid[i][j] == 0) {

                    if(grid[i-1][j] == 1 ||
                       grid[i+1][j] == 1 ||
                       grid[i][j-1] == 1 ||
                       grid[i][j+1] == 1) {

                        newGrid[i][j] = 1;
                    } else {
                        newGrid[i][j] = 0;
                    }

                } else {
                    newGrid[i][j] = grid[i][j];
                }
            }
        }

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                grid[i][j] = newGrid[i][j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();

    cout << "Temps d'execution : "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    return 0;
}
