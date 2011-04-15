
typedef int* semaphore;

semaphore create_semaphore(int n){
  semaphore p;
  p = (int *) malloc(2*sizeof(int));

  if ( pipe(p) < 0 ) {
    printf("Error!\n");
    exit(-1);
  }

  for ( int i = 0; i < n; ++i )
    write(p[1], ".", 1);


  return p;
}

void P(semaphore s){
  char c; 
  read(s[0], &c, 1); 
}

void V(semaphore s){
   char c;
   write(s[1], &c, 1); 
}

int delete_semaphore(semaphore s){
  close(s[0]);
  close(s[1]);
  free(s);
}
