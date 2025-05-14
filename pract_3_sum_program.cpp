#include <iostream>
#include <vector>
#include <thread>
#include <numeric> // for std::accumulate

// Function to compute partial sum
void partial_sum(const std::vector<int>& array, int start, int end, int& result) {
    result = std::accumulate(array.begin() + start, array.begin() + end, 0);
}

// Function to sum array using p processors
int sum_array_with_p_processors(const std::vector<int>& array, int p) {
    int n = array.size();
    std::vector<std::thread> threads;
    std::vector<int> results(p, 0);

    int chunk_size = (n + p - 1) / p; // Ceiling division

    for (int i = 0; i < p; ++i) {
        int start = i * chunk_size;
        int end = std::min(start + chunk_size, n);
        if (start < n) {
            threads.emplace_back(partial_sum, std::cref(array), start, end, std::ref(results[i]));
        }
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    // Sum all partial results
    return std::accumulate(results.begin(), results.end(), 0);
}

int main() {
    std::vector<int> array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int p = 4; // Number of processors/threads

    std::cout << "Array: ";
    for (int num : array) std::cout << num << " ";
    std::cout << "\n";

    int total_sum = sum_array_with_p_processors(array, p);
    std::cout << "Sum of all elements using " << p << " processors: " << total_sum << std::endl;

    return 0;
}

//  g++ -std=c++11 -pthread -o sum_with_threads pract_3_sum_program.cpp