#include <stdio.h>
#include <string.h>

void *lsearch(void *key, void *base, int (*cmp_fn)(void *v1,void *v2), int n, int element_size) {
	void *p;
	int i=0;

	for (i=0;i<n;i++) {
		p = (char *) base + i*element_size; // (char *) hack, could use (unsigned long *) 
		if ( cmp_fn(key, p) == 0 ) return p; //memcpy would also be viable (except for pointer contents)
	}
	return NULL;
}

int cmp_fn(void *v1, void *v2) {
	int *a = v1;
	int *b = v2;
	if ( *a == *b ) return 0;
	else return 1;
}
int main() {
	int tab[10];
	int i;

	for(i=0;i<10;i++) {
		tab[i]=i;
	}

	for (i=0;i<15;i++){
		printf("Checking for i = %d \n",i);
		int *p = lsearch(&i, tab, cmp_fn, 10, sizeof(int));

		if ( p != NULL )
			printf(" %d\n ",*p);
		else 
			printf(" p is NULL\n");
	}


}
