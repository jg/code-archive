#include <stdio.h>

/* Compute the value of c in each case */
int main() {
	int a = 6;
	int b = 7;
	int c;

	c = a---b; /* What is the value of c? */

	a=6;b=7;
	c = a--+--b; /* How about now? */
	printf("%d",c);

	a=6;b=7;
	c = a+++b++;

	a=6;b=7;
	c = b+++a--+b;
	
	a=6;b=7;
	c = -b+++a--+--b;

	a=6;b=7;
	if (a-->0) {
		c = 1;
	}else c=2;

}
