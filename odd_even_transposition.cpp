#include <iostream>
#include <pthread.h>
#include <algorithm>
using namespace std;

#define N 8

int a[N] = {2, 1, 4, 9, 5, 3, 6, 10};
int tmp_index;
pthread_mutex_t lock;

int max_threads = (N + 1) / 2;


void* compare(void* arg) {
    int index;

    pthread_mutex_lock(&lock);
    index = tmp_index;
    tmp_index += 2;
    pthread_mutex_unlock(&lock);

    if ((index + 1 < N) && (a[index] > a[index + 1])) {
        swap(a[index], a[index + 1]);
    }

    return NULL; 
}


void oddEvenSort() {
    pthread_t threads[max_threads];
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0)
            tmp_index = 0;  // Odd phase
        else
            tmp_index = 1;  // Even phase

        int thread_count = (i % 2 == 0) ? max_threads : max_threads - 1;

        for (int j = 0; j < thread_count; ++j) {
            pthread_create(&threads[j], NULL, compare, NULL);
        }

        for (int j = 0; j < thread_count; ++j) {
            pthread_join(threads[j], NULL);
        }
    }
}


void printArray() {
    for (int i = 0; i < N; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main() {
    pthread_mutex_init(&lock, NULL);

    cout << "Given array is: ";
    printArray();

    oddEvenSort();

    cout << "\nSorted array is: ";
    printArray();

    pthread_mutex_destroy(&lock);
    return 0;
}
