// BUGS/*{{{*/
// Possible overflow (count/front)
/*}}}*/
// Includes/*{{{*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "semaphores.cpp"
/*}}}*/
// Macrodefinitions/*{{{*/
// gid map
enum Group { A1 = 1001, A2, A3, B1, B2 };

#define A1_SIZE 1
#define A2_SIZE 1
#define A3_SIZE 1
#define B1_SIZE 1
#define B2_SIZE 1
// Process count
#define PCOUNT \
  A1_SIZE + A2_SIZE + A3_SIZE + B1_SIZE + B2_SIZE

#define FIFO_1 0
#define FIFO_2 1
#define FIFO_1_SIZE 40
#define FIFO_2_SIZE 40
#define FIFO_1_BYTECOUNT sizeof(int)*FIFO_1_SIZE
#define FIFO_2_BYTECOUNT sizeof(int)*FIFO_2_SIZE
#define FIFO_1_OFFSET 0
#define FIFO_2_OFFSET FIFO_1_BYTECOUNT

#define SHM_KEY 128
// Space for FIFO elements and counters 
// FIFO1_COUNT is SHM_SIZE+0, FIFO2_
#define SHM_BYTECOUNT (FIFO_1_BYTECOUNT + FIFO_2_BYTECOUNT) + 4*sizeof(int)

#define SUM FIFO_1_BYTECOUNT + FIFO_2_BYTECOUNT
// FIFO counter offset
#define FIFO_1_CO SUM
#define FIFO_1_FO SUM+sizeof(int)
// FIFO front offset
#define FIFO_2_CO SUM+2*sizeof(int)
#define FIFO_2_FO SUM+3*sizeof(int)

#define MSEC 1000
/*}}}*/
// Globals/*{{{*/
int remaining_A1 = A1_SIZE;
int remaining_A2 = A2_SIZE;
int remaining_A3 = A3_SIZE;
int remaining_B1 = B1_SIZE;
int remaining_B2 = B2_SIZE;

char *shm;

semaphore mutex = create_semaphore(1);
semaphore a1 = create_semaphore(1);
semaphore a2 = create_semaphore(1);
semaphore a3 = create_semaphore(1);
semaphore b1_1 = create_semaphore(1);
semaphore b1_2 = create_semaphore(1);
semaphore b2_1 = create_semaphore(1);
semaphore b2_2 = create_semaphore(1);
/*}}}*/
int *count(int q) {/*{{{*/
  // Returns pointer to integer containing queue element count
  if ( q == FIFO_1 ) 
    return (int*)shm + FIFO_1_CO;
  else
    return (int*)shm + FIFO_2_CO;
}/*}}}*/
int *front(int q) {/*{{{*/
  // Returns pointer to integer containing first queue element index
  if ( q == FIFO_1 ) 
    return (int*)shm + FIFO_1_FO;
  else
    return (int*)shm + FIFO_2_FO;
  
}/*}}}*/
int size(int q) {/*{{{*/
  if ( q == FIFO_1 ) 
    return FIFO_1_SIZE;
  else if ( q == FIFO_2 )
    return FIFO_2_SIZE;
}/*}}}*/
int offset(int q) {/*{{{*/
  if ( q == FIFO_1 ) 
    return FIFO_1_OFFSET;
  else if ( q == FIFO_2 )
    return FIFO_2_OFFSET;
}/*}}}*/
void init_fifo() {/*{{{*/
  *count(FIFO_1) = 0;
  *front(FIFO_1) = 0;

  *count(FIFO_2) = 0;
  *front(FIFO_2) = 0;

  int *ptr = (int*)shm;
  for ( int i = 0; i < (FIFO_1_SIZE + FIFO_2_SIZE); ++i, ptr++ ) 
    *(ptr) = 0;
}/*}}}*/
int max(int q) {/*{{{*/
  int max = 0;

  for ( int i = 0; i < *count(q); i++ ) {
    int index = (*front(q) + i) % size(q);
    int val = ((int*)shm)[offset(q) + index];

    if ( val > max ) max = val;
  }
  return max;
}/*}}}*/
int even_count(int q) {/*{{{*/
    int even = 0;
    for ( int i = 0; i < *count(q); i++ ) {
      int index = (*front(q) + i) % size(q);
      int val = ((int*)shm)[offset(q) + index];

      if ( val % 2 == 0 ) even++;
    }
    return even;
}/*}}}*/
int odd_count(int q) {/*{{{*/
  int odd = 0;
  for ( int i = 0; i < *count(q); i++ ) {
    int index = (*front(q) + i) % size(q);
    int val = ((int*)shm)[offset(q) + index];

    if ( val % 2 != 0 ) odd++;
  }
  return odd;
}/*}}}*/
void put(int q, int val) {/*{{{*/
  // Inserts element to FIFO
  int* p = (int*)shm;

  if ( *count(q) < size(q) ) {
    int index = (*count(q) + *front(q)) % size(q);
    p[offset(q) + index] = val;
    ++*count(q);
  } else 
    printf("FIFO_%d FULL!\n", q);


  V(a2);
  V(b1_1);
  V(b1_2);
  V(b2_1);
  V(b2_2);

}/*}}}*/
int get(int q) {/*{{{*/
  // Removes & returns element from FIFO
  int* p = (int*)shm;
  int val;

  if ( *count(q) > 0 ) {
    int index = *front(q) % size(q);
    val = p[offset(q) + index];
    p[offset(q) + index] = 0;
    --*count(q);
    ++*front(q);

    V(a1);
    V(a3);

    return val;
  } else 
    printf("FIFO_%d EMPTY!\n", q);

}/*}}}*/
int top(int q) {/*{{{*/
  return (int)shm[offset(q) + ((*front(q) + *count(q)) % size(q))];
}/*}}}*/
void print_fifo(int fifo) {/*{{{*/
    int* p = (int*)shm+FIFO_1_OFFSET;

    printf("FIFO_1 (%d, %d, max: %d, oc: %d, ec: %d): \n", 
      *front(fifo) % size(fifo), *count(fifo),
      max(fifo), 
      odd_count(fifo),
      even_count(fifo));
    for ( int i = 0; i < size(fifo); ++i ) {
      if ( (i == *front(fifo) % size(fifo)) && (i == (*front(fifo) + *count(fifo) - 1) % size(fifo)) )
        printf("<%d>  ", *p);
      else if ( i == (*front(fifo) % size(fifo)) ) 
        printf("<%d  ", *p);
      else if ( i == (*front(fifo) + *count(fifo) - 1) % size(fifo) ) 
        printf("%d>  ", *p);
      else
        printf("%d  ", *p);

      p++;
    }
    printf("\n");

}/*}}}*/
void shm_at() {/*{{{*/
  // Attaches shared memory block to process
  int shmid;

  // Create shm buffer
  if ((shmid = shmget(SHM_KEY, SHM_BYTECOUNT, IPC_CREAT | 0666)) < 0) {
    printf("shmget error\n");
    exit(1);
  }
  // Attach shm segment to data space
  if ((shm = (char*)shmat(shmid, 0, 0)) == (void*) -1) {
    printf("shmat error\n");
    exit(1);
  }
}
/*}}}*/
void A1_p() {/*{{{*/
  // Procesy A1 generują kolejne liczby parzyste, jeżeli produkowana liczba
  // będzie mniejsza niż największa liczba z obu buforów, i wstawiają
  // wygenerowaną liczbę do tego bufora, w którym jest mniej liczb.

  shm_at();
  
  int number = 2;
  while(1) {
    // printf("While iteration\\n");
    P(mutex);
    int max1 = max(FIFO_1);
    int max2 = max(FIFO_2);
    int max = max1 > max2 ? max1 : max2;
    V(mutex);

    if ( number < max ) {
      P(mutex);
        if ( count(FIFO_1) < count(FIFO_2) ) 
          put(FIFO_1, number);
        else
          put(FIFO_2, number);
      V(mutex);

      number += 2;
    } else  {
      P(a1);
    }


    sleep(1);
  }

}/*}}}*/
void A2_p() {/*{{{*/
  // Procesy A2 generują do bufora 1 kolejne liczby nieparzyste jeżeli w
  // buforze 1 jest mniej niż 30 liczb

  shm_at();

  int number = 1;
  while(1) {
    printf("A2 loop\n");

    if ( *count(FIFO_1) < 30 ) {
      P(mutex);
        put(FIFO_1, number);
      V(mutex);

      number += 2;
    } else {
      P(a2);
    }

    sleep(1);
  }
}/*}}}*/
void A3_p() {/*{{{*/
  // Procesy A3 generują do bufora 2 kolejne liczby nieparzyste jeżeli w
  // buforze 2 jest mniej niż 20 liczb
  shm_at();


  int number = 1;
  while(1) {
    // printf("A3 loop\\n");

    if ( *count(FIFO_2) < 20 ) {
      P(mutex);
        put(FIFO_2, number);
      V(mutex);
      number += 2;
    } else {
      P(a3);
    }


    sleep(1);
  }
}/*}}}*/
void B1_p() {/*{{{*/
  // Procesy B1 zjadają liczby parzyste (z dowolnego bufora), pod warunkiem
  // że dany bufor zawiera co najmniej 6 liczb.

  shm_at();

  while(1) {
    // printf("B1 loop\\n");

    if ( (*count(FIFO_1) >= 6) && (top(FIFO_1) % 2 == 0) ) {
      P(mutex);
        get(FIFO_1);
      V(mutex);
    } else 
      P(b1_1);

    if ( (*count(FIFO_2) >= 6) && (top(FIFO_2) % 2 == 0) ) {
      P(mutex);
        get(FIFO_2);
      V(mutex);
    } else
      P(b1_2);
     
    sleep(1);
  }

}/*}}}*/
void B2_p() {/*{{{*/
  // Procesy B2 zjadają liczby nieparzyste (z dowolnego bufora), pod
  // warunkiem że dany bufor zawiera co najmniej 7 liczb.

  shm_at();

  while(1) {
    // printf("B2\\n");

    if ( (*count(FIFO_1) >= 7) && (top(FIFO_1) % 2 != 0) ) {
      P(mutex);
        get(FIFO_1);
      V(mutex);
    } else
      P(b2_1);


    if ( (*count(FIFO_2) >= 7) && (top(FIFO_2) % 2 != 0) ) {
      P(mutex);
        get(FIFO_2);
      V(mutex);
    } else
      P(b2_2);
     
   

    sleep(1);
  }


}/*}}}*/
void generate() {/*{{{*/

  shm_at();
  
  int number = 1;
  while(1) {
    P(mutex);

    put(FIFO_1, number);
    number += 1;

    V(mutex);
    sleep(1);
  }

}/*}}}*/
void consume() {/*{{{*/
  shm_at();

  int number = 1;
  while(1) {

    P(mutex);
      
    get(FIFO_1);

    V(mutex);

    sleep(1);
  }
}/*}}}*/
void parent() {/*{{{*/
  while (1) {
    print_fifo(FIFO_1);
    printf("\n");
    print_fifo(FIFO_2);
    printf("\n");
    printf("-------------------------------\n");
    printf("\n");

    sleep(1);
  }

}/*}}}*/
void decrease_remaining() {/*{{{*/
  if ( remaining_A1 > 0 ) 
    remaining_A1--;
  else if ( remaining_A2 > 0 )
    remaining_A2--;
  else if ( remaining_A3 > 0 )
    remaining_A3--;
  else if ( remaining_B1 > 0 )
    remaining_B1--;
  else if ( remaining_B2 > 0 )
    remaining_B2--;
}/*}}}*/
void set_gid() {/*{{{*/
  if ( remaining_A1 > 0 ) {
    setgid(A1);
    --remaining_A1;
  } else if ( remaining_A2 > 0 ) {
    setgid(A2);
    --remaining_A2;
  } else if ( remaining_A3 > 0 ) {
    setgid(A3);
    --remaining_A3;
  } else if ( remaining_B1 > 0 ) {
    setgid(B1);
    --remaining_B1;
  } else if ( remaining_B2 > 0 ) {
    setgid(B2);
    --remaining_B2;
  }
}/*}}}*/
int main(int argc, char** argv) {/*{{{*/
  pid_t pid;
  int i;

  // Attach shm segment to process
  shm_at();
  // Init fifos
  init_fifo();
  
  i = 0;
  for ( int i = 0; i < PCOUNT; ++i ) {
    pid = fork();

    if ( pid == 0 ) {
      // Child
      set_gid();
      // printf("Child process gid: %d\\n", getgid());
      int gid = getgid();
      switch (gid) {
        case A1:
          A1_p();
        break;
        case A2:
          A2_p();
          // generate();
        break;
        case A3:
          A3_p();
          // consume();
        break;
        case B1:
          B1_p();
        break;
        case B2:
          B2_p();
        break;
      }
      break;
    } else {
      // Used for proper group id assignment
      decrease_remaining();
    }
  }
  if ( pid != 0 )
    parent();

  /*
  if ( (pid = fork()) == 0 ) {
    // Child
    setgid(A1);

    printf("Child process group: %d\n", getgid());
  } else {
    printf("Parent process group: %d\n", getgid());
    wait();
  }
  */

}/*}}}*/
