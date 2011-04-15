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
void init_fifo() {/*{{{*/
  int* count = (int*)shm+FIFO_1_CO;
  int* front = (int*)shm+FIFO_1_FO;
  *count = 0;
  *front = 0;
  count = (int*)shm+FIFO_2_CO;
  front = (int*)shm+FIFO_2_FO;
  *count = 0;
  *front = 0;

  int *ptr = (int*)shm;
  for ( int i = 0; i < (FIFO_1_SIZE + FIFO_2_SIZE); ++i, ptr++ ) 
    *(ptr) = 0;
}/*}}}*/
int max(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 ) {
    int* count = (int*)shm + FIFO_1_CO;
    int* front = (int*)shm + FIFO_1_FO;

    int max = 0;
    for ( int i = 0; i < *count; i++ ) {
      int index = (*front + i) % FIFO_1_SIZE;
      int val = ((int*)shm)[FIFO_1_OFFSET + index];
      if ( val > max ) max = val;
    }
    return max;
  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm+FIFO_2_CO;
    int* front = (int*)shm+FIFO_2_FO;
    int max = 0;

    for ( int i = 0; i < *count; i++ ) {
      int index = (*front + i) % FIFO_2_SIZE;
      int val = ((int*)shm)[FIFO_2_OFFSET + index];
      if ( val > max ) max = val;
    }
    return max;
  }
}/*}}}*/
int even_count(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 ) {
    int* count = (int*)shm + FIFO_1_CO;
    int* front = (int*)shm + FIFO_1_FO;
    int even = 0;
    for ( int i = 0; i < *count; i++ ) {
      int index = (*front + i) % FIFO_1_SIZE;
      int val = ((int*)shm)[FIFO_1_OFFSET + index];
      if ( val % 2 == 0 ) even++;
    }
    return even;
  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm+FIFO_2_CO;
    int* front = (int*)shm+FIFO_2_FO;
    int even = 0;
    for ( int i = 0; i < *count; i++ ) {
      int index = (*front + i) % FIFO_2_SIZE;
      int val = ((int*)shm)[FIFO_2_OFFSET + index];
      if ( val % 2 == 0 ) even++;
    }
    return even;
  }
}/*}}}*/
int odd_count(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 ) {
    int* count = (int*)shm + FIFO_1_CO;
    int* front = (int*)shm + FIFO_1_FO;
    int odd = 0;
    for ( int i = 0; i < *count; i++ ) {
      int index = (*front + i) % FIFO_1_SIZE;
      int val = ((int*)shm)[FIFO_1_OFFSET + index];
      if ( val % 2 != 0 ) odd++;
    }
    return odd;
  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm+FIFO_2_CO;
    int* front = (int*)shm+FIFO_2_FO;
    int odd = 0;
    for ( int i = 0; i < *count; i++ ) {
      int index = (*front + i) % FIFO_2_SIZE;
      int val = ((int*)shm)[FIFO_2_OFFSET + index];
      if ( val % 2 != 0 ) odd++;
    }
    return odd;
  }
}/*}}}*/
void put(int fifo, int val) {/*{{{*/
  if ( fifo == FIFO_1 ) { 
    int* count = (int*)shm + FIFO_1_CO;
    int* front = (int*)shm + FIFO_1_FO;
    int* p = (int*)shm;

    if ( *count < FIFO_1_SIZE ) {
      int index = (*count + *front) % FIFO_1_SIZE;
      p[FIFO_1_OFFSET + index] = val;
      // Increase count value
      ++*count;
    } else 
      printf("FIFO_1 FULL!\n");

  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm + FIFO_2_CO;
    int* front = (int*)shm + FIFO_2_FO;
    int* p = (int*)shm;

    if ( *count < FIFO_2_SIZE ) {
      int index = (*count + *front) % FIFO_2_SIZE;
      p[FIFO_2_OFFSET + index] = val;
      // Increase count
      ++*count;
    } else 
      printf("FIFO_2 FULL!\n");
  }

  V(a2);
  V(b1_1);
  V(b1_2);
  V(b2_1);
  V(b2_2);

}/*}}}*/
int get(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 ) { 
    int* count = (int*)shm+FIFO_1_CO;
    int* front = (int*)shm+FIFO_1_FO;
    int* p = (int*)shm;
    int val;

    if ( *count > 0 ) {
      int index = *front % FIFO_1_SIZE;
      val = p[FIFO_1_OFFSET + index];
      p[FIFO_1_OFFSET + index] = 0;
      // Decrease element count
      --*count;
      // Update front
      ++*front;

      V(a1);
      V(a3);

      return val;
    } else 
      printf("FIFO_1 EMPTY!\n");

  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm+FIFO_2_CO;
    int* front = (int*)shm+FIFO_2_FO;
    int* p = (int*)shm;
    int val;
    if ( *count > 0 ) {
      int index = *front % FIFO_2_SIZE;
      val = p[FIFO_2_OFFSET + index];
      p[FIFO_2_OFFSET + index] = 0;
      // Decrease element count
      --*count;
      // Update front
      ++*front;

      V(a1);
      V(a3);

      return val;
    } else 
      printf("FIFO_2 EMPTY!\n");
  }
}/*}}}*/
int count(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 )  
    return *((int*)shm + FIFO_1_CO);
  else if ( fifo == FIFO_2 ) 
    return *((int*)shm + FIFO_2_CO);
}/*}}}*/
int front(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 )  
    return *(int*)shm + FIFO_1_FO;
  else if ( fifo == FIFO_2 ) 
    return *(int*)shm + FIFO_2_FO;
}/*}}}*/
int top(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 ) {
    int* count = (int*)shm + FIFO_1_CO;
    int* front = (int*)shm + FIFO_1_FO;
    return (int)shm[FIFO_1_OFFSET + ((*front + *count) % FIFO_1_SIZE)];
  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm+FIFO_2_CO;
    int* front = (int*)shm+FIFO_2_FO;
    return (int)shm[FIFO_2_OFFSET + ((*front + *count) % FIFO_2_SIZE)];
  }
}/*}}}*/
void print_fifo(int fifo) {/*{{{*/
  if ( fifo == FIFO_1 ) {
    int* count = (int*)shm + FIFO_1_CO;
    int* front = (int*)shm + FIFO_1_FO;
    int* p = (int*)shm+FIFO_1_OFFSET;

    printf("FIFO_1 (%d, %d, max: %d, oc: %d, ec: %d): \n", 
      *front % FIFO_1_SIZE, *count,
      max(FIFO_1), 
      odd_count(FIFO_1),
      even_count(FIFO_1));
    for ( int i = 0; i < FIFO_1_SIZE; ++i ) {
      if ( (i == *front % FIFO_1_SIZE) && (i == (*front + *count - 1) % FIFO_1_SIZE) )
        printf("<%d>  ", *p);
      else if ( i == (*front % FIFO_1_SIZE) ) 
        printf("<%d  ", *p);
      else if ( i == (*front + *count - 1) % FIFO_1_SIZE ) 
        printf("%d>  ", *p);
      else
        printf("%d  ", *p);

      p++;
    }
    printf("\n");

  } else if ( fifo == FIFO_2 ) {
    int* count = (int*)shm+FIFO_2_CO;
    int* front = (int*)shm+FIFO_2_FO;
    int* p = (int*)shm+FIFO_2_OFFSET;

    printf("FIFO_2 (%d, %d, max: %d, oc: %d, ec: %d): \n", 
      *front % FIFO_2_SIZE, *count,
      max(FIFO_2), 
      odd_count(FIFO_2),
      even_count(FIFO_2));

    for ( int i = 0; i < FIFO_2_SIZE; ++i ) {
      if ( (i == *front % FIFO_2_SIZE) && (i == (*front + *count - 1) % FIFO_2_SIZE) )
        printf("<%d>  ", *p);
      else if ( i == *front % FIFO_2_SIZE) 
        printf("<%d  ", *p);
      else if ( i == (*front + *count - 1) % FIFO_2_SIZE ) 
        printf("%d>  ", *p);
      else
        printf("%d  ", *p);
      p++;
    }
    printf("\n");
  }
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

    if ( count(FIFO_1) < 30 ) {
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

    if ( count(FIFO_2) < 20 ) {
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

    if ( (count(FIFO_1) >= 6) && (top(FIFO_1) % 2 == 0) ) {
      P(mutex);
        get(FIFO_1);
      V(mutex);
    } else 
      P(b1_1);

    if ( (count(FIFO_2) >= 6) && (top(FIFO_2) % 2 == 0) ) {
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

    if ( (count(FIFO_1) >= 7) && (top(FIFO_1) % 2 != 0) ) {
      P(mutex);
        get(FIFO_1);
      V(mutex);
    } else
      P(b2_1);


    if ( (count(FIFO_2) >= 7) && (top(FIFO_2) % 2 != 0) ) {
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
