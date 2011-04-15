#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

int modExp(int a, int b, int n) {/*{{{*/
	/* Returns a^b (mod n), quick modular exponentiation */
	int base      = a;
	int exponent  = b;
	int result    = 1;
	while ( exponent > 0 ) {
		if ( (exponent & 1) == 1 ) {
			result = (result * base) % n;
		}
		exponent >>= 1;
		base = (base * base) % n;
	}
	return result;
}/*}}}*/

int witness(int b, int n) {/*{{{*/
	/* Returns true if n is proved to be a composite number, false is n is pseudoprime base b */
	int y;
	int t,u; int x; int i; int tmp;

	if ( !(n & 1) ) 
		return true;

	u = n - 1; t = 0 ;
	while ( !(u & 1) ) {
		u = u / 2;
		t++;
	}

	y = x = modExp(b, u, n);

	for ( i = 0; i <= t; i++ ) {
		x = (x * x) % n;
		if ( x == 1 && ( y != 1 && y != n-1 ) )
			return true;
		y = x;

	}
	if ( x != 1 )
		return true;
	return false;
}/*}}}*/
int test(int n, int k) {/*{{{*/
	/* Returns true if n is pseudoprime with respect to 'k' randomly generated bases */
	if ( n == 2 ) return true;
	if ( !( n & 1 ) || n == 1) 
		return false;
	int j;
	srand(time(NULL));

	for ( j = 0 ; j < k; j++ )
	{
		if ( witness ( (rand()%(n-2))+1, n) )
			return false;
	}
	return true;
}/*}}}*/

int main() {
	int i,j;
	j = 0;

	for ( i = 2; i < 910; i++ ) {
		if ( test (i, 100) ) {
			printf("%5d ",i);
			j++;
			if ( j % 10 == 0 )
				printf("\n");
		}
	}
	printf("\n");
}
