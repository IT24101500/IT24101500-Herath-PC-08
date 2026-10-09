#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    const long long N = 10000000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Start the clock at the same moment on every process
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // Work out this rank's chunk
    long long chunk = N / size;
    long long start = rank * chunk + 1;
    long long end   = (rank + 1) * chunk;
    if (rank == size - 1) end = N;   // last rank takes any remainder

    // Add up my chunk
    long long partial = 0;
    for (long long i = start; i <= end; i++) {
        partial += i;
    }

    if (rank != 0) {
        // Workers send their partial sum to rank 0
        MPI_Send(&partial, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        // Rank 0 collects partial sums from everyone else
        long long total = partial;
        long long incoming;

        for (int src = 1; src < size; src++) {
            MPI_Recv(&incoming, 1, MPI_LONG_LONG, src, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += incoming;
        }

        double t1 = MPI_Wtime();
        printf("np=%d total=%lld time=%f\n", size, total, t1 - t0);
    }

    MPI_Finalize();
    return 0;
}
