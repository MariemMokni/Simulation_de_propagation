
#include <iostream>
#include <mpi.h>
#include <chrono>

using namespace std;

#define N 1000
#define ITER 100

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = N / size;

    int localGrid[rows + 2][N];
    int newGrid[rows + 2][N];

    auto start_time = chrono::high_resolution_clock::now();

    for(int i = 0; i < rows + 2; i++) {
        for(int j = 0; j < N; j++) {
            localGrid[i][j] = 0;
            newGrid[i][j] = 0;
        }
    }

    // infection cellule centrale
    int globalMiddle = N / 2;

    int owner = globalMiddle / rows;

    if(rank == owner) {
        int localMiddle = globalMiddle % rows + 1;
        localGrid[localMiddle][globalMiddle] = 1;
    }

    for(int t = 0; t < ITER; t++) {

        if(rank > 0) {
            MPI_Sendrecv(
                localGrid[1], N, MPI_INT, rank - 1, 0,
                localGrid[0], N, MPI_INT, rank - 1, 1,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
        }

        if(rank < size - 1) {
            MPI_Sendrecv(
                localGrid[rows], N, MPI_INT, rank + 1, 1,
                localGrid[rows + 1], N, MPI_INT, rank + 1, 0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE
            );
        }

        for(int i = 1; i <= rows; i++) {

            for(int j = 1; j < N - 1; j++) {

                if(localGrid[i][j] == 0) {

                    if(localGrid[i-1][j] == 1 ||
                       localGrid[i+1][j] == 1 ||
                       localGrid[i][j-1] == 1 ||
                       localGrid[i][j+1] == 1) {

                        newGrid[i][j] = 1;

                    } else {
                        newGrid[i][j] = 0;
                    }

                } else {
                    newGrid[i][j] = 1;
                }
            }
        }

        for(int i = 1; i <= rows; i++) {
            for(int j = 1; j < N - 1; j++) {
                localGrid[i][j] = newGrid[i][j];
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();

    if(rank == 0) {

        cout << "Simulation MPI Halo terminée" << endl;

        cout << "Temps MPI : "
             << chrono::duration_cast<chrono::milliseconds>(
                    end_time - start_time).count()
             << " ms" << endl;
    }

    MPI_Finalize();

    return 0;
}
