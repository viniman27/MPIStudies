#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define VECTOR_SIZE 100

int main(int argc, char** argv) {
    int rank, size;
    int vector[VECTOR_SIZE];
    int current_print_position = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size > 6) {
        printf("O programa requer no máximo 6 processos (1 master e 5 workers).\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    srand(time(NULL) + rank);  // Semente para gerar números aleatórios baseada no tempo e rank

    if (rank == 0) {  // Master process
        // Inicializa o vetor
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = i;
        }
    }

    while (current_print_position < VECTOR_SIZE) {
        int offset = 1 + rand() % 14;  // Calcula um offset aleatório entre 1 e 14
        
        // Pega o ponto de impressão atual
        MPI_Bcast(&current_print_position, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Se for o worker correto (determinado pelo ponto de impressão), imprime a sua porção
        if (rank == (current_print_position / offset) % (size - 1) + 1) {
            int end_position = current_print_position + offset;
            end_position = (end_position > VECTOR_SIZE) ? VECTOR_SIZE : end_position;

            printf("Worker %d (offset %d) printing positions %d to %d: ", rank, offset, current_print_position, end_position - 1);
            for (int i = current_print_position; i < end_position; i++) {
                printf("%d ", vector[i]);
            }
            printf("\n");

            current_print_position = end_position;
        }

        // O master atualiza o ponto de impressão para todos os processos
        if (rank == 0) {
            MPI_Bcast(&current_print_position, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
