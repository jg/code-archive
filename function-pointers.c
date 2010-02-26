#include <stdio.h>

int *foo() {
	return (int*)324;
}
/* Some tricky pointers-to-function declarations */
int main() {
	/* What does the following do? (and why did i have to comment it out)*/
	(*(void(*)())0)();

	float *g(), (*h)(); /* What's the difference? */

	float (*i)();
	
	/* Are these the same? */
	*foo();

	(*foo)();

	(*foo());


}
