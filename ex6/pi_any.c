#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    const long long N = 10000000;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long my_n = N / size;
    if (rank == size - 1) my_n = N - (N / size) * (size - 1);

    unsigned int seed = 12345 + rank;

    long long hits = 0;
    for (long long i = 0; i < my_n; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) hits++;
    }

    if (rank != 0) {
        MPI_Send(&hits, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total_hits = hits;
        long long incoming;
        MPI_Status status;
        int *order = malloc(size * sizeof(int));

        for (int i = 0; i < size - 1; i++) {
            // Accept whichever worker finishes first
            MPI_Recv(&incoming, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0,
                     MPI_COMM_WORLD, &status);
            order[i] = status.MPI_SOURCE;
            total_hits += incoming;
        }

        double pi = 4.0 * (double)total_hits / (double)N;
        double t1 = MPI_Wtime();
        printf("np=%d pi=%.6f time=%f order=", size, pi, t1 - t0);
        for (int i = 0; i < size - 1; i++) printf("%d ", order[i]);
        printf("\n");
        free(order);
    }

    MPI_Finalize();
    return 0;
}
