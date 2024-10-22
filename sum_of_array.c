// Write a thread function
// Create the thread and give the array as arg
// Calculate the sum of values in the array 
// (if arr size is 5, the result should be (0+1+2+3+4)=10)
// Join to the threads and get the exit(result) status

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// Structure to hold the args for the thread function
typedef struct arr_args {
    int* arr; // Pointer to the integer array
    int size; // Size of the array
} arr_args;

// Function to fill the array with values from 0 to size-1
void fill_array(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        *(arr + i) = i;
    }
}

// Thread function to calculate the sum of the array elements
void* sum_array(void* args) {
    arr_args* th_args = (arr_args*)args; // Create a pointer and cast the arr_args type
    int* sum_p = (int*)malloc(sizeof(int)); // Allocate memory for the sum
    
    // Check memory allocation failure for the sum_p
    if (sum_p == NULL) 
        return NULL;
    
    *sum_p = 0; // Initialize sum to 0
    
    for (int i = 0; i < th_args->size; ++i) {
        *sum_p += *(th_args->arr + i); // Sum the elements of the array
    }
    
    return sum_p; // Return the pointer to the sum
}

int main() {
    
    pthread_t arr_t; // Thread variable
    int size = 10; // Size of the array
    int* arr =(int*) malloc(size * sizeof(int)); // Dynamically allocate memory for the array
    
    // // Check memory allocation failure for arr
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed for arr\n");
        return 1; // Exit on allocation failure
    }
    
    fill_array(arr, size); // Call fill_array
    
    arr_args args; // Create an instance of arr_args
    args.arr = arr; // Assign the arr pointer
    args.size = size; // Assign the size of the array

    // Create a thread to calculate the sum of the array
    if (pthread_create(&arr_t, NULL, sum_array, (void*)&args)) {
        fprintf(stderr, "pthread_create failed\n");
        free(arr); // Free allocated memory before exiting
        return 2; // Exit on thread create failure    
    }
    
    int* sum_result; // pointer to store the result from the thread
    
    // Wait for the thread to finish and return the result
    if(pthread_join(arr_t, (void**)&sum_result)) {
        fprintf(stderr, "pthread_join failed\n");
        free(arr); // Free allocated memory before exiting
        return 3; // Exit on thread join failure  
    };
        
    // Check if sum_result is not NULL before dereferencing
    if (sum_result == NULL)
        fprintf(stderr, "sum_array returned NULL\n");
    else
        printf("Thread exit value=%d\n",*sum_result);
    
    free(arr); // for arr
    free(sum_result); // for the sum_p
    
    return 0;
}
