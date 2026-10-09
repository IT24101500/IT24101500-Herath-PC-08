#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int x = 30, y;

    if (rank == 1) {
        printf("Rank 1: sending x to rank 3\n");
        fflush(stdout);
        MPI_Ssend(&x, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        printf("Rank 1: send finished\n");
    } else if (rank == 3) {
        printf("Rank 3: waiting for a message from rank 2\n");
        fflush(stdout);
        // WRONG source on purpose: the sender is rank 1, not 2
        MPI_Recv(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
        printf("Rank 3: got y = %d\n", y);
    }

    MPI_Finalize();
    return 0;
}
