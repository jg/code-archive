#include <stdlib.h>
#include <string.h>

void swap(void *x, void *y, int n) {
	char buffer[n];
	memcpy(buffer, x, n);
	memcpy(x, y, n);
	memcpy(y, buffer, n);
}
void quickSort(void *tab, int n, int elementSize, int (*cmp)(void *a, void *b)) {
	int pivot, i;
	pivot = rand()%n;
	for ( i = 0; i < 
}

int main() {
	
}
