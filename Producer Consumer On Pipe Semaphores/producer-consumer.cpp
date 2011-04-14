#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "semaphores.cpp"


#define N 10 /* no. of elements in buffer */
#define SHM_KEY 123
#define SHM_SIZE 64 /* 32 for elements, 32 for counter (int) */
#define SLEEP_INTERVAL 1
#define COUNTER_OFFSET 16

/* exclusion semaphore */
semaphore mutex;
/* number of empty_places places in buffer */
semaphore empty_places;
/* number of items in buffer */
semaphore number_of_items;
// int counter = 0; // number of items in buffer
char *shm, *s;

int producer() {
  while (1) {
    P(empty_places);
    P(mutex);

    int *counter = (int*)shm+COUNTER_OFFSET;
    shm[++(*counter)] = 'x';

    // Print out buffer state
    printf("P>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    for (int i = 0; i < 16; ++i ) printf("%c", *(shm+i)); printf("\n"); fflush(stdout);
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    printf("\n\n");

    V(mutex);
    V(number_of_items);

    sleep(SLEEP_INTERVAL);
  }
}
int consumer() {
  while (1) {
    P(number_of_items);
    P(mutex);

    int *counter = (int*)shm+16;
    shm[--(*counter)] = '.';

    // Print out buffer state
    printf("C>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    for(int i = 0; i < 16; ++i ) printf("%c", *(shm+i)); printf("\n");
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"); printf("\n\n"); fflush(stdout);

    V(mutex);
    V(empty_places);

    sleep(2*SLEEP_INTERVAL);
  }
}

int main(int argc, char** argv) {
  int shmid;
  mutex = create_semaphore(1);
  empty_places = create_semaphore(N);
  number_of_items = create_semaphore(0);
  
  int pid = fork();
  if ( pid == 0 ) { // Child process is consumer

    // Get buffer
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
      printf("shmget child error\n");
      exit(1);
    }
    // Attach segment to data space
    if ((shm = (char*)shmat(shmid, 0, 0)) == (void*) -1) {
      printf("shmat child error\n");
      exit(1);
    }

    consumer();
  } else { // Parent is producer

    // Create shm buffer
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
      printf("shmget error\n");
      exit(1);
    }

    // Attach segment to data space
    if ((shm = (char*)shmat(shmid, 0, 0)) == (void*) -1) {
      printf("shmat error\n");
      exit(1);
    }

    // init counter 
    int *counter = (int*)shm+COUNTER_OFFSET;
    *counter = 0;

    // init memory 
    for(int i = 0; i < 16; ++i ) *(shm+i) = '.';

    producer(); 
  }
}

