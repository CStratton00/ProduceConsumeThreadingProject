/*
Collin Stratton
CST-315
Topic 7 Project 7: Code Errors and the Butterfly Effect
Dr. Ricardo Citro

For this project, the goal was to demonstrate proficiency with threads using
a producer and consumer program. 
*/

// include libraries to display to the terminal, user threads, and stall
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// define the function bufferSize to store 500
#define bufferSize 500

// create a struct "shared" that stores values across the threads
struct {
  pthread_mutex_t mutex;                  // pthread mutex variable to lock and unlock data
  int buff[bufferSize];                   // buff array size with bufferSize used to create the size of the array
  int producedCount;                      // stores the amount produced
  int consumedCount;                      // stores teh amount consumed
} shared = { PTHREAD_MUTEX_INITIALIZER }; // initializes shared as a pthread

// produce function that takes in an argument that points to the inputted value from the terminal
void *produce(void *arg) {
  while(1) {                                  // run until return null
    pthread_mutex_lock(&shared.mutex);        // lock the mutex so only produce can access it
    if(shared.producedCount < bufferSize) {   // check to make sure there is still buffer available
      shared.producedCount++;                 // increase the production count
      *((int *)arg) += 1;                     // increase the inputted argument by 1
      pthread_mutex_unlock(&shared.mutex);    // unlock the mutex for access
    } else {
      pthread_mutex_unlock(&shared.mutex);    // else unlock the mutex
      return NULL;                            // return null to break out of the produce function
    }
  }
}

// consume function that takes in an argument that points to the inputted value from the terminal
void *consume(void *arg) {
  while(1) {                                          // run until return null
    pthread_mutex_lock(&shared.mutex);                // lock the mutex so only consume can access it
    if(shared.consumedCount < shared.producedCount) { // check if the number consumed is less than produced
      shared.consumedCount++;                         // increase the consumption count
      *((int *)arg) += 1;                             // increased the inputed argument by 1
      pthread_mutex_unlock(&shared.mutex);            // unlock the mutex for access
    } else {
      pthread_mutex_unlock(&shared.mutex);            // else unlock the mutex
      return NULL;                                    // return null to break out of the produce function
    }
  }
}

// main body of code
int main() {
  // create variables to store the total number of production and consumer threads
  int prodThreads = 0, consThreads = 0; 

  // prompt the user to input the number of produce and consumer threads to use
  printf("\nEnter the no. of Producer threads: ");
  scanf("%d", &prodThreads);
  printf("\nEnter the no. of Consumers threads: ");
  scanf("%d", &consThreads);

  // create producer and consumer arrays based on the user inputs
  int producerArr[prodThreads], consumerArr[consThreads];

  // create the number of threads as requested by the user and make them cocurrent 
  pthread_t producer[prodThreads], consumer[consThreads];
  pthread_setconcurrency(prodThreads + consThreads);

  // loop through the number of producer and consumer threads and run their functions
  for(int i = 0; i < prodThreads; i++) {
     producerArr[i] = 0;
     pthread_create(&producer[i], NULL, produce, &producerArr[i]);
  }
  for(int i = 0; i < consThreads; i++) {
    consumerArr[i] = 0;
    pthread_create(&consumer[i], NULL, consume, &consumerArr[i]);
  }

  // output the number produced by each production thread
  for(int i = 0; i < prodThreads; i++) {
    // pthread_join waits for the thread to terminates so producerArr displays a correct value
    pthread_join(producer[i], NULL);
    printf("\nThe Producer %d Produced: %d Items", i, producerArr[i]);
    sleep(100);
  }

  printf("\n"); 
  // output the number consumed by each consumption thread
  for(int i = 0; i < consThreads; i++) {
    // pthread_join waits for the thread to terminates so consumerArr displays a correct value
    pthread_join(consumer[i], NULL);
    printf("\nThe Consumer %d Consumed: %d Items", i, consumerArr[i]);
    sleep(100);
  }
  return 0;
}
