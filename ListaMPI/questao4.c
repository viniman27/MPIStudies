#include <stdio.h>
#include <mpi.h>

#define VETOR_SIZE 5  // Tamanho do vetor a ser enviado para cada worker

int main(int argc, char** argv) {
    int rank, size, vetor[VETOR_SIZE], vetorModificado[VETOR_SIZE];

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

        for (int i = 1; i < size; i++) {
            MPI_Recv(vetorModificado, VETOR_SIZE, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Mestre recebeu o vetor modificado do processo %d: ", i);
            for (int j = 0; j < VETOR_SIZE; j++) {
                printf("%d ", vetorModificado[j]);
            }
            printf("\n");
        }
    } else {
        // Se for um processo worker
        MPI_Recv(vetor, VETOR_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Subtrair uma unidade de cada posição do vetor
        for (int i = 0; i < VETOR_SIZE; i++) {
            vetorModificado[i] = vetor[i] - 1;
        }

        // Enviar o vetor modificado de volta para o mestre
        MPI_Send(vetorModificado, VETOR_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    // Finalizar o ambiente MPI
    MPI_Finalize();

    return 0;
}
