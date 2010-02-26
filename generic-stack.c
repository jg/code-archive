#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct asdhj {
	void *pbottom;
	int stack_length;
	int allocated_length;
	int element_size;
	void (*freefn)(void *v);
} stack;

void stack_new(stack *s, int element_size) {
	//request memory for four objects for a start
	s->element_size = element_size;
	s->pbottom = malloc ( 4 * element_size );

	s->stack_length = 0;
	s->allocated_length = 4;
}

void stack_dispose(stack *s) {
	int i;

	if ( s->freefn != NULL ) {
		for ( i = 0; i < s->stack_length; i++ )
			s->freefn ( (char *) s->pbottom + s->element_size * i );
	}
}
void freefn(void *v) {
	free ( (char *) v );
}

void stack_push(stack *s, void *pelement) {
	void *target; 
	if ( s->stack_length == s->allocated_length ) { //no more memory, allocate some more
		s->allocated_length *= 2;
		s->pbottom = realloc ( s->pbottom, s->allocated_length * s->element_size );
	}
	
	target = (char *) s->pbottom + s->stack_length * s->element_size;
	memcpy ( target, pelement, s->element_size );

	s->stack_length++;
}

void stack_pop(stack *s, void *result) {
	assert ( s->stack_length != 0 );
	void *source;

	source = (char *) s->pbottom + (s->stack_length - 1)*(s->element_size);
	memcpy ( result, source, s->element_size ); 

	s->stack_length--;
}
	

int main() {
	stack s;
	stack *ps;
	ps = &s;
	
	int i,f;

	stack_new(ps, sizeof(int));

/*
	i=5;
	stack_push(ps, &i);
	i=8;
	stack_push(ps, &i);

	stack_pop(ps, &f);
	printf("%d\n",f);
	stack_pop(ps, &f);
	printf("%d\n",f);
*/

	for ( i = 0; i < 10; i++ ) {
		printf(" Pushing %d to the stack\n",i);
		stack_push(ps, &i);
	}
	for ( i = 0; i < 11; i++ ) {
		stack_pop(ps, &f);
		printf ( " Popping %d from the stack \n ", f );
	}
}
