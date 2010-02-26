/*	Generates the happy number sequence (Sloane's A000216)
 */
#include <stdio.h>
#include <math.h>
#include <string.h>



int powd(int x) {
	int i=1;
	int k=10;
	for(i;i<x;i++) {
		k *= 10;
	}
	return k;
}
int dcount(int n) {
	return (log10(n)+1);
}
/* Returns the i-th rightmost digit */
int digit(int n, int i) {
	int k=n;
	int j=0;
	for(j=1;j<i;j++)
		k /= 10;
	return k % 10;
}
int f(int n) {
	int sum = 0;
	int i;
	for(i=1;i<=dcount(n);i++) 
		sum += (digit(n,i)*digit(n,i));
	return sum;
}

/* takes the starting number and the optional 'h' argument for html formatting */
int main(int argc, char **argv) {
	int n, d, g;
	g = atoi(argv[1]);
	d = dcount(n);
	d = 4;
	int l = ((argc > 2) && (strcmp(argv[2], strdup("h"))) == 0);

	if ( l )
		printf("<pre style=\"margin:0px\">%*d : ", d, g);
	else
		printf("%*d : ", d, g);

	n = g;
	while ((n!=4) && (n!=1)) {
		int k = f(n);
		n = k;
		// If second argument specified, bold function values greater than the parameter
		if ( l ){
			if ( k > g ) {
				printf("<b>%d</b> ", n);}
		}
		else
			printf("%d ", n);
	}
	if ( l )
		printf("</pre>\n");
	else
		printf("\n");
}
