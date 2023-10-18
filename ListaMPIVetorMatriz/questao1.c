#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 6

int main(int argc, char** argv) {
    int rank, size;
    int vector[VECTOR_SIZE];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != 3) {
        printf("Este programa requer exatamente 3 processos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    if (rank == 0) {  // Master process
        // Inicializa o vetor
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i + 1;
        }
        MPI_Send(vector, VECTOR_SIZE / 2, MPI_INT, 1, 0, MPI_COMM_WORLD);  // Envia primeira metade para o slave 1
        MPI_Send(&vector[VECTOR_SIZE / 2], VECTOR_SIZE / 2, MPI_INT, 2, 0, MPI_COMM_WORLD);  // Envia segunda metade para o slave 2
    } else {  // Slave processes
        MPI_Recv(vector, VECTOR_SIZE / 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Slave %d received vector: ", rank);
        for (int i = 0; i < VECTOR_SIZE / 2; i++) {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
