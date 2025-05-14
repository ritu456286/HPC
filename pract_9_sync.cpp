#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 4;  // Each process will receive N elements
    std::vector<int> data(N);

    if (rank == 0) {
        // Initialize data in root process
        std::vector<int> full_data(N * size);
        for (int i = 0; i < N * size; ++i) {
            full_data[i] = i + 1;
        }

        std::cout << "[Root] Broadcasting and scattering data..." << std::endl;

        // Scatter data to all processes
        MPI_Scatter(full_data.data(), N, MPI_INT, data.data(), N, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        // Receive scattered data
        MPI_Scatter(nullptr, N, MPI_INT, data.data(), N, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Barrier for synchronization
    MPI_Barrier(MPI_COMM_WORLD);
    std::cout << "[Process " << rank << "] Received data:";
    for (int i : data) std::cout << " " << i;
    std::cout << std::endl;

    // Compute local sum
    int local_sum = std::accumulate(data.begin(), data.end(), 0);

    // Reduce local sums into total sum at root
    int total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "[Root] Total sum from all processes: " << total_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
