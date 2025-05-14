#include <iostream>
#include <omp.h>

int main() {
    // Set number of threads (optional)
    omp_set_num_threads(4);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            std::cout << "Section 1 executed by thread " << omp_get_thread_num() << std::endl;
        }

        #pragma omp section
        {
            std::cout << "Section 2 executed by thread " << omp_get_thread_num() << std::endl;
        }

        #pragma omp section
        {
            std::cout << "Section 3 executed by thread " << omp_get_thread_num() << std::endl;
        }

        #pragma omp section
        {
            std::cout << "Section 4 executed by thread " << omp_get_thread_num() << std::endl;
        }
    }

    return 0;
}
