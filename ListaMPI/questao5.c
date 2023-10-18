#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    char message[50];
    MPI_Status status;

    // Inicializar o ambiente MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {  // Se for o processo master
        for (int i = 1; i < size; i++) {
            MPI_Recv(message, 50, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf("Mestre recebeu a mensagem: '%s' do processo %d.\n", message, status.MPI_SOURCE);
        }
    } else {  // Se for um processo escravo
        snprintf(message, 50, "Oi, Estou vivo!");
        MPI_Send(message, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Finalizar o ambiente MPI
    MPI_Finalize();

    return 0;
}
