#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size, next, prev, token;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_len;

    // Inicializar o ambiente MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &hostname_len);

    // Determinar o próximo e o anterior no anel
    next = (rank + 1) % size;
    prev = (rank - 1 + size) % size; // Adicionamos "size" para lidar com rank 0

    if (rank == 0) {
        printf("Rank[0/'%s']: enviando token para Rank[1/'%s']\n", hostname, hostname);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank[0/'%s']: recebeu token do Rank[%d/'%s']\n", hostname, prev, hostname);
    } else {
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank[%d/'%s']: recebeu token e enviou para %d\n", rank, hostname, next);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    // Finalizar o ambiente MPI
    MPI_Finalize();

    return 0;
}
