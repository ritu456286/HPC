#include <iostream>
#include <thread>
#include <vector>

// Function to compute the partial sum of an array segment
void partial_sum(const std::vector<int>& array, int start, int end, int& result) {
    result = 0;
    for (int i = start; i < end; ++i) {
        result += array[i];
    }
}

// Main function
int sum_array_with_two_processors(const std::vector<int>& array) {
    int n = array.size();
    int mid = n / 2;

    // Results for both halves
    int result1 = 0, result2 = 0;

    // Create threads for each half
    std::thread thread1(partial_sum, std::ref(array), 0, mid, std::ref(result1));
    std::thread thread2(partial_sum, std::ref(array), mid, n, std::ref(result2));

    // Wait for both threads to complete
    thread1.join();
    thread2.join();

    // Combine the results from both threads
    return result1 + result2;
}

int main() {
    // Example array
    std::vector<int> array = {1, 2, 3, 4, 5, 6, 7, 8};

    std::cout << "Array: ";
    for (int num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    int total_sum = sum_array_with_two_processors(array);
    std::cout << "Sum of all elements: " << total_sum << std::endl;

    return 0;
}


// g++ -std=c++11 -pthread -o sum_program pract_2_sum_ele.cpp