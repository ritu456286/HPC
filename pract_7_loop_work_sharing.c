#include <stdio.h>
#include <omp.h>

int main() {
    int i, n = 10;
    int a[10];

    // Set number of threads (optional)
    omp_set_num_threads(4);

    // Parallelize the loop using OpenMP
    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        a[i] = i * i;
        printf("Thread %d processed index %d --> value: %d\n",
               omp_get_thread_num(), i, a[i]);
    }

    return 0;
}

// gcc -fopenmp pract_7_loop_work_sharing.c -o omp_loop_sharing.exe