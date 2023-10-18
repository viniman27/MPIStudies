#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size, number;

    // Inicializar o ambiente MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Se for o processo mestre
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            // Enviar um número inteiro (i) para cada processo worker
            MPI_Send(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Se for um processo worker
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu o número %d do mestre.\n", rank, number);
    }

    // Finalizar o ambiente MPI
    MPI_Finalize();

    return 0;
}
