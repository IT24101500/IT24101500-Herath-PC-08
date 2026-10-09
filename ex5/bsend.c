#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank;
    MPI_Status status;
    int x[10], y[10];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        for (int r = 0; r < 10; r++) x[r] = 10 * r;

        // Attach a buffer big enough for the message plus MPI's overhead
        int bufsize = 10 * sizeof(int) + MPI_BSEND_OVERHEAD;
        char *buffer = malloc(bufsize);
        MPI_Buffer_attach(buffer, bufsize);

        printf("Rank 1: buffered-sending x to rank 3\n");
        MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);

        // x was already copied into the buffer, so it is safe to change now
        for (int r = 0; r < 10; r++) x[r] = -1;
        printf("Rank 1: overwrote x with -1 after Bsend\n");

        // Detach waits until the buffered message has been delivered
        MPI_Buffer_detach(&buffer, &bufsize);
        free(buffer);
    } else if (rank == 3) {
        MPI_Recv(y, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Rank 3: received y =");
        for (int r = 0; r < 10; r++) printf(" %d", y[r]);
        printf("\n");
    } else {
        printf("Rank %d: just a normal process\n", rank);
    }

    MPI_Finalize();
    return 0;
}
