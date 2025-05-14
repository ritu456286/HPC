#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric> // for std::accumulate

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank (process ID)
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes

    const int N = 16; // Total number of elements (you can adjust this)
    std::vector<int> full_array;
    int chunk_size = N / size;

    std::vector<int> sub_array(chunk_size);

    if (rank == 0) {
        // Initialize the array in the root process
        full_array.resize(N);
        for (int i = 0; i < N; ++i) {
            full_array[i] = i + 1;
        }

        std::cout << "Full array: ";
        for (int val : full_array) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Scatter the array to all processes
    MPI_Scatter(full_array.data(), chunk_size, MPI_INT,
                sub_array.data(), chunk_size, MPI_INT,
                0, MPI_COMM_WORLD);

    // Each process computes its local sum
    int local_sum = std::accumulate(sub_array.begin(), sub_array.end(), 0);

    // Reduce all local sums to get the total sum at root (rank 0)
    int total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Total sum using " << size << " processors: " << total_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}

// g++ pract_4_sum_mpi.cpp -o pract_4_sum_mpi.exe -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include" -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi

//  mpiexec -n 4 ./pract_4_sum_mpi.exe