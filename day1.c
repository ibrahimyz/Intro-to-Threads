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
    
    pthread_create(&myThread, NULL, myTurn, (void*)&num); // pass the address of num as arg
    pthread_create(&yourThread, NULL, yourTurn, NULL); // there is no arg
    
    int* myResult; // pointer to store the result from myTurn
    int* yourResult; // pointer to store the result from yourTurn
    
    // wait until all threads are finished before exiting
    pthread_join(myThread, (void**)&myResult);
    pthread_join(yourThread, (void**)&yourResult);
    
    printf("mR=%d\n",*myResult);
    printf("yR=%d\n",*yourResult);
    
    free(yourResult); // free dynamically allocated memory

    return 0;
}
