#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 100  // Tamanho do vetor. Pode ser ajustado conforme necessário.

int main(int argc, char** argv) {
    int rank, size;
    int vector[VECTOR_SIZE];
    int portion_size;
    int start_idx, end_idx;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {  // Master process
        // Inicializa o vetor
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i + 1;
        }

        // Envia as porções do vetor para cada escravo
        for (int i = 1; i < size; i++) {
            start_idx = (VECTOR_SIZE / size) * i;
            end_idx = (i == size - 1) ? VECTOR_SIZE : start_idx + (VECTOR_SIZE / size);
            portion_size = end_idx - start_idx;

            MPI_Send(&vector[start_idx], portion_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {  // Slave processes
        start_idx = (VECTOR_SIZE / size) * rank;
        end_idx = (rank == size - 1) ? VECTOR_SIZE : start_idx + (VECTOR_SIZE / size);
        portion_size = end_idx - start_idx;
        int recv_buffer[portion_size];

        MPI_Recv(recv_buffer, portion_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Slave %d received vector portion: ", rank);
        for (int i = 0; i < portion_size; i++) {
            printf("%d ", recv_buffer[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
