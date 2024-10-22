#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* myTurn(void* arg) {
    int* num = (int*)arg; // a pointer to the original num
    
    for(int i = 0; i < 4; ++i) {
        sleep(1);
        printf("My Turn i=%d\n", i);
        (*num)++; // increment the original num
    }
    return num; // return the pointer to the original num
}

void* yourTurn(void * arg) {
    // allocate and initialize memory for the result
    int* iptr = (int*) malloc(sizeof(int));
    if (iptr == NULL) {
        fprintf(stderr, "Memory allocation failed in yourTurn\n");
        return NULL; // Return NULL on memory allocation failure
    }
    *iptr = 5;
    
    for(int i = 0; i < 3; ++i) {
        sleep(1);
        printf("Your Turn i=%d\n", i);
        (*iptr)++;
    }
    return iptr; 
}

int main() {
    pthread_t myThread, yourThread;
    int num = 5; // create an int variable for arg of thread
    
    if (pthread_create(&myThread, NULL, myTurn, (void*)&num)) { // pass the address of num as arg
        fprintf(stderr, "Error creating myThread\n");
        return 1; // Exit on error
    }
    
    if (pthread_create(&yourThread, NULL, yourTurn, NULL)) { // there is no arg
        fprintf(stderr, "Error creating yourThread\n");
        return 1; // Exit on error
    }
        
    
    int* myResult; // pointer to store the result from myTurn
    int* yourResult; // pointer to store the result from yourTurn
    
    // wait until all threads are finished before exiting
    if (pthread_join(myThread, (void**)&myResult)) {
        fprintf(stderr, "Error joining myThread\n");
        return 2; // Exit on error
    }
    
    if (pthread_join(yourThread, (void**)&yourResult)) {
        fprintf(stderr, "Error joining yourThread\n");
        return 2; // Exit on error
    }
    
    // Check if myResult is not NULL before dereferencing
    if (myResult != NULL)
        printf("mR=%d\n",*myResult);
    else
        fprintf(stderr, "myTurn returned NULL\n");

    // Check if yourResult is not NULL before dereferencing
    if (yourResult != NULL) 
        printf("yR = %d\n", *yourResult);
    else 
        fprintf(stderr, "yourTurn returned NULL\n");
        
    free(yourResult); // free dynamically allocated memory

    return 0;
}
