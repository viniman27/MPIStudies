#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 4  // Dimensão da matriz

void printMatrix(float matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int rank, size;
    float A[N][N], B[N][N], C[N][N];
    float rowA[N], rowB[N], rowC[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != 5) {
        printf("O programa requer exatamente 5 processos (1 master e 4 workers).\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    srand(time(NULL));

    if (rank == 0) {  // Master process
        // Inicializa as matrizes A e B com valores randômicos
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
            }
        }

        // Envia cada linha das matrizes para os workers
        for (int i = 0; i < N; i++) {
            MPI_Send(A[i], N, MPI_FLOAT, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(B[i], N, MPI_FLOAT, i + 1, 0, MPI_COMM_WORLD);
        }
    }

    // Workers recebem suas linhas, fazem a soma e enviam de volta para o master
    if (rank > 0) {
        MPI_Recv(rowA, N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(rowB, N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int j = 0; j < N; j++) {
            rowC[j] = rowA[j] + rowB[j];
        }

        MPI_Send(rowC, N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    // O master coleta os resultados dos workers e monta a matriz C
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            MPI_Recv(C[i], N, MPI_FLOAT, i + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Imprime a matriz resultante C
        printf("Matriz C resultante:\n");
        printMatrix(C);
    }

    MPI_Finalize();
    return 0;
}
