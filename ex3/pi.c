#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    const long long N = 10000000;   // total number of throws

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Start the clock at the same moment on every process
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // My share of the throws (last rank takes any remainder)
    long long my_n = N / size;
    if (rank == size - 1) my_n = N - (N / size) * (size - 1);

    // Different seed per rank so each rank throws different darts
    unsigned int seed = 12345 + rank;

    long long hits = 0;
    for (long long i = 0; i < my_n; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) hits++;
    }

    if (rank != 0) {
        // Workers send their hit count to rank 0
        MPI_Send(&hits, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total_hits = hits;
        long long incoming;

        for (int src = 1; src < size; src++) {
            MPI_Recv(&incoming, 1, MPI_LONG_LONG, src, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_hits += incoming;
        }

        double pi = 4.0 * (double)total_hits / (double)N;
        double t1 = MPI_Wtime();
        printf("np=%d pi=%.6f time=%f\n", size, pi, t1 - t0);
    }

    MPI_Finalize();
    return 0;
}
