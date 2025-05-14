#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4  // Size of the matrix and array

void sum_array(int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    printf("Sum of array: %d\n", sum);
}

void matrix_multiply(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array[N] = {1, 2, 3, 4};
    int A[N][N] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    int B[N][N] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    int C[N][N];

    if (rank == 0) {
        printf("Initial array and matrices initialized.\n");
    }

    // Summing up the array in parallel
    int local_sum = 0;
    int chunk_size = N / size;
    int *local_array = (int *)malloc(chunk_size * sizeof(int));

    MPI_Scatter(array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < chunk_size; i++) {
        local_sum += local_array[i];
    }

    int total_sum;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum of array: %d\n", total_sum);
    }

    // Matrix multiplication in parallel
    int local_A[N/size][N];
    int local_C[N/size][N];

    MPI_Scatter(A, N*N/size, MPI_INT, local_A, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N/size; i++) {
        for (int j = 0; j < N; j++) {
            local_C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                local_C[i][j] += local_A[i][k] * B[k][j];
            }
        }
    }

    MPI_Gather(local_C, N*N/size, MPI_INT, C, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Resultant matrix after multiplication:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}


// g++ pract_6_MPI_parallel.cpp -o mpi_parallel.exe -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi

// mpiexec -n 4 ./mpi_parallel.exe