#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // current process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // total number of processes

    // Broadcast example
    int data;
    if (rank == 0) {
        data = 42;
        std::cout << "[Broadcast] Process " << rank << " broadcasting data: " << data << std::endl;
    }

    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "[Broadcast] Process " << rank << " received data: " << data << std::endl;

    // Scatter example
    std::vector<int> scatter_data;
    int recv_data;
    if (rank == 0) {
        scatter_data = {10, 20, 30, 40};
    }

    MPI_Scatter(scatter_data.data(), 1, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "[Scatter] Process " << rank << " received: " << recv_data << std::endl;

    // Gather example
    int send_data = rank * 100;
    std::vector<int> gathered_data(size);
    MPI_Gather(&send_data, 1, MPI_INT, gathered_data.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "[Gather] Gathered data at root: ";
        for (int val : gathered_data) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Reduce example (sum)
    int local_val = rank + 1;
    int sum = 0;
    MPI_Reduce(&local_val, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "[Reduce] Sum of all ranks+1: " << sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
