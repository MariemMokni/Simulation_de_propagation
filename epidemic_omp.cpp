
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

#define N 1000
#define ITER 100

int grid[N][N];
int newGrid[N][N];

int main() {

    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            grid[i][j] = 0;
            newGrid[i][j] = 0;
        }
    }

    grid[N/2][N/2] = 1;

    #pragma omp parallel  //Divise cette boucle entre plusieurs threads
    {

        for(int t = 0; t < ITER; t++) {

            #pragma omp for collapse(2) schedule(static) //les deux boucles sont fusionnées
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

                        newGrid[i][j] = 1;
                    }
                }
            }

            #pragma omp for collapse(2) schedule(static)
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    grid[i][j] = newGrid[i][j];
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();

    cout << "Temps OpenMP optimisé : "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    return 0;
}
