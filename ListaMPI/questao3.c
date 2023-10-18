#include <stdio.h>
#include <mpi.h>

#define VETOR_SIZE 5  // Tamanho do vetor a ser enviado para cada worker

int main(int argc, char** argv) {
    int rank, size, vetor[VETOR_SIZE];

    // Inicializar o ambiente MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Se for o processo mestre
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            for (int j = 0; j < VETOR_SIZE; j++) {
                vetor[j] = (i * VETOR_SIZE) + j;
            }
            // Enviar o vetor de inteiros para o processo worker
            MPI_Send(vetor, VETOR_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Se for um processo worker
        MPI_Recv(vetor, VETOR_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processo %d recebeu o vetor: ", rank);
        for (int i = 0; i < VETOR_SIZE; i++) {
            printf("%d ", vetor[i]);
        }
        printf("\n");
    }

    // Finalizar o ambiente MPI
    MPI_Finalize();

    return 0;
}
