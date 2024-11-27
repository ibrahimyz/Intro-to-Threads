#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 3 // Define the size of 2D Array

// Structure to hold the args for the thread function
typedef struct ThreadArg {
   int* arr;      // Pointer to int 2D array
   int colIndex;  // Column Index for each column
   int sum;       // Sum variable for each column 
} ThreadArg;

// Function to fill the array 
void fillArray(int* arr) {
   for (int i = 0; i < SIZE; ++i) {
      for (int j = 0; j < SIZE; ++j) {
         *((arr + i * SIZE) + j) = (i * SIZE + j) * 2;   // Values are i+j(indicies) * 2
      }
   }
}

// Thread function to sum of each column in the array 
void* sumColumn(void* args) {
   // Check if args is NULL  
   if (args == NULL) {
      perror("NULL argument passed to thread function!");
      pthread_exit(NULL);
   }
  
   ThreadArg* thArgs = (ThreadArg*)args; // Cast to ThreadArg type pointer
   int colIndex = thArgs->colIndex; 

   // Calculate to sum of each column
   for (int i = 0; i < SIZE; ++i){
      thArgs->sum += *((thArgs->arr + i * SIZE) + colIndex); // DEĞİŞECEK
   }
  
   // Print column index and its sum value. (We can return this values instead printing.)
   printf("Column: %d - Sum: %d\n", colIndex, thArgs->sum);
   
   free(thArgs); // Free allocated memory before exiting
   pthread_exit(0);
}

int main() {
   pthread_t t[SIZE]; // Thread array

   int* arr = (int*) malloc(SIZE*SIZE*sizeof(int)); // Allocate memory for the array

   // Check memory allocation failure for arr
   if (arr == NULL) {
      perror("Memory allocation failed!");
      return EXIT_FAILURE;
   }
   
   fillArray(arr); // Call fillArray function
   
    for (int i = 0; i < SIZE; ++i) {
      ThreadArg* args = (ThreadArg*)malloc(sizeof(ThreadArg)); // Allocate unique structure for each thread

      // Check memory allocation failure for args
      if (args == NULL) {
          perror("Memory allocation failed!");
          free(arr); // Free allocated memory before exiting
          return EXIT_FAILURE;
      }

      // Assing values for structure
      args->arr = arr; // Arr pointer hold the address
      args->colIndex = i;
      args->sum = 0;     

      // Create all threads to calculate the sum of each column
      if (pthread_create(t + i, NULL, &sumColumn, (void*)args)) {
          perror("Thread creation failed!");
          free(arr);   // Free memory
          return EXIT_FAILURE; // Create failure
      }
    }

   // Wait for all threads to finish
   for (int i = 0; i < SIZE; ++i){
      if(pthread_join(t[i], NULL)) {
          perror("Join failed!");
          free(arr);   // Free memory
          return EXIT_FAILURE; // Join failure
      }
   }
   
   free(arr); // Free arr memory before main return
   return 0;
}
