#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Thread function
void* print_message(void* arg) {
    printf("Hello from thread! Message: %s\n", (char*)arg);
    return NULL;
}

int main() {
    pthread_t thread; // Thread identifier
    const char* message = "Hello, World!";

    // Create the thread
    if (pthread_create(&thread, NULL, print_message, (void*)message) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    // Wait for the thread to finish
    if (pthread_join(thread, NULL) != 0) {
        perror("Failed to join thread");
        return EXIT_FAILURE;
    }

    printf("Main thread: Thread has finished execution.\n");
    return EXIT_SUCCESS;
}


//gcc -pthread -o hello_pthread hello_pthread.c