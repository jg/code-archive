/* Test pierwszości Millera-Rabina
Idea bazuje na pomyśle testowania pierwszości przy pomocy małego twierdzenia Fermata ( przy pomocy którego można skonstruować tzw test pierwszości Fermata ) :

a^(n-1) == 1 (mod n)  (1)

(1) jest spełniane przez wszystkie liczby pierwsze, lecz niestety także przez niektóre liczby złożone (istnieje nawet cała klasa liczb złożonych które spełniają (1) dla każdej podstawy a, są to tzw liczby Carmicheal'a). 
Jeśli dana liczba spełnia (1) mówimy że jest liczbą pseudopierwszą przy podstawie a. 
Z drugiej strony, liczby nie spełniające (1) są na pewno liczbami złożonymi. 

Test Millera Rabina pozwala na zmiejszenie prawdopodobieństwa trafienia na liczbę złożoną spełniającą (1) przez :

1) Sprawdzanie czy zachodzi (1) dla wielu podstaw
2) Sprawdzanie czy przypadkiem nie istnieje nietrywialny pierwiastek z 1 (mod n) co oznacza że dana liczba jest złożoną

Nietrywialny pierwiastek z 1 (mod n) to takie x != 1 (mod n), x != -1 (mod n), że x^2 == 1 (mod n). 
Np 6 jest nietrywialnym pierwiastkiem kwadratowym modulo 35 z jedności bo 6^2 == 1 (mod 35) i 6 != +/- 1 (mod 35)
Można pokazać że nietrywialny pierwiastek z 1 (mod n) istnieje tylko wtedy gdy n jest liczbą złożoną :

Przeciwnie, gdy n jest liczbą pierwszą :
x^2 == 1 (mod n) i x != 1 (mod n) i x != -1 (mod n)
Wtedy z własności kongruencji
n|(x^2-1)
n|(x-1)(x+1)
n|(x-1) lub n|(x+1)
x == 1 (mod n) lub x == -1 (mod n)
(zresztą nie może mieć więcej pierwiastków niż te dwa już znalezione bo to są pierwiastki wielomianu st. 2)

KNOWN BUGS:
modExp nie działa dla podstaw >= 65536 = 2^16 bo 2^16 * 2^16 % n  
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

typedef unsigned long int ulint;

ulint modExp(ulint a, ulint b, ulint n) {/*{{{*/
	/* Zwraca a^b (mod n) */
	/* Wynik liczony jest sprytnie aby zaoszczędzić na mnożeniach tzn 
	 * patrząc na binarną reprezentację wykładnika przemnażamy rezultat przez bazę o odpowiedniej wadze (2^i) modulo n  tylko wtedy gdy odpowiedni bit wykładnika jest '1' 
	 * (znalezione w skrypcie do WDI) */
	ulint base      = a;
	ulint exponent  = b;
	ulint result    = 1;
	while ( exponent > 0 ) {
		if ( (exponent & 1) == 1 ) {
			result = (result * base) % n;
		}
		exponent >>= 1;
		base = (base * base) % n;
	}
	return result;
}/*}}}*/
ulint witness(ulint b, ulint n) {/*{{{*/
	/* Zwraca 'true' gdy n jest liczbą złożoną (jeśli istnieje nietrywialny pierwiastek z 1 modulo n to n na pewno jest złożona) 'false' jeśli n jest liczbą pseudopierwszą przy podstawie b*/
	ulint y;
	ulint t,u; ulint x; ulint i; ulint tmp;

	/* jeśli parzysta to złożona */
	if ( !(n & 1) ) 
		return true;

	/* Rozkładamy n - 1 na takie u,t że n - 1 = u*2^t (żeby móc później podnosić u*2^i do kwadratu i sprawdzać czy przypadkiem nie istnieje nietrywialny pierwiastek z jedynki (modulo n) co oznaczałoby że n jest złożone */
	u = n - 1; t = 0 ;
	while ( !(u & 1) ) {
		u = u / 2;
		t++;
	}

	/* 'x' jest b^u (mod n), y w pętli jest wykorzystywane jako 'poprzednia' wartość 'x' (do sprawdzenia czy dany pierwiastek z jedynki jest 'trywialny')*/
	y = x = modExp(b, u, n);
	for ( i = 0; i <= t; i++ ) {
		x = (x * x) % n;

		/* Sprawdzamy istnienie nietrywialnego pierwiastka (różnego od 1,-1) z jedynki */	
		if ( x == 1 && ( y != 1 && y != n-1 ) )
			return true;

		y = x;
	}
	/* Tw. Fermata nie spełnione -> l złożona */
	if ( x != 1 )
		return true;

	return false;
}/*}}}*/
ulint test(ulint n, ulint k) {/*{{{*/
	/* Zwraca 'true' jeśli n jest liczbą pseudopierwszą w stosunku do 'k' losowo wygenerowanych podstaw z zakresu 1..n-1*/

	if ( n == 2 ) return true;
	/* jeśli parzysta to złożona */
	if ( !( n & 1 ) || n == 1) 
		return false;
	ulint j;
	srand(time(NULL));

	for ( j = 0 ; j < k; j++ )
	{
		if ( witness ( (rand()%(n-2))+1, n) )
			return false;
	}
	return true;
}/*}}}*/

int main(int argc, char **argv) {
	char *val;
	ulint     c;
	ulint tries = 100;
	ulint tmp;
	while (( c = getopt(argc, argv, "p:r:t:")) != -1 ) {
		switch (c) {
			case 't':
				val   = optarg;
				tries = atoi(val);
//				prulintf("Setting tries to %d\n", tries);
			break;
			case 'p':
//				prulintf("Tries is %d\n", tries)
        tmp = atoi(optarg);
				if ( tmp == 0 ) break;
				if ( test(tmp, tries) ) 
					printf("1\n");  // Pierwsza
					else 
						printf("0\n");// Nie pierwsza
				break;
		}
	}
	/*
	ulint i,j;
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
	*/
}
