#include <stdio.h>

int foo() {
}
int main() {
	/* Will foo execute? */
	int a,b;
	b = 4;
	if ( a=b == 4 ) {
		foo();
	}
}
