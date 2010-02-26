#include <stdio.h>

void swap(void *v1, void *v2, int n) {
	char buffer[n];
	memcpy(buffer, v1, n);
	memcpy(v1, v2, n);
	memcpy(v2, buffer, n);
}

int main() {
	int a,b;
	a = 5;
	b = 7;

	printf("%d %d", a, b);
	swap(&a, &b);
	printf("%d %d", a, b);
}
