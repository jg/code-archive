/* Test pierwszo¶ci Millera-Rabina
Idea bazuje na pomy¶le testowania pierwszo¶ci przy pomocy ma³ego twierdzenia Fermata ( przy pomocy którego mo¿na skonstruowaæ tzw test pierwszo¶ci Fermata ) :

a^(n-1) == 1 (mod n)  (1)

(1) jest spe³niane przez wszystkie liczby pierwsze, lecz niestety tak¿e przez niektóre liczby z³o¿one (istnieje nawet ca³a klasa liczb z³o¿onych które spe³niaj± (1) dla ka¿dej podstawy a, s± to tzw liczby Carmicheal'a). 
Je¶li dana liczba spe³nia (1) mówimy ¿e jest liczb± pseudopierwsz± przy podstawie a. 
Z drugiej strony, liczby nie spe³niaj±ce (1) s± na pewno liczbami z³o¿onymi. 

Test Millera Rabina pozwala na zmiejszenie prawdopodobieñstwa trafienia na liczbê z³o¿on± spe³niaj±c± (1) przez :

1) Sprawdzanie czy zachodzi (1) dla wielu podstaw
2) Sprawdzanie czy przypadkiem nie istnieje nietrywialny pierwiastek z 1 (mod n) co oznacza ¿e dana liczba jest z³o¿on±

Nietrywialny pierwiastek z 1 (mod n) to takie x != 1 (mod n), x != -1 (mod n), ¿e x^2 == 1 (mod n). 
Np 6 jest nietrywialnym pierwiastkiem kwadratowym modulo 35 z jedno¶ci bo 6^2 == 1 (mod 35) i 6 != +/- 1 (mod 35)
Mo¿na pokazaæ ¿e nietrywialny pierwiastek z 1 (mod n) istnieje tylko wtedy gdy n jest liczb± z³o¿on± :

Przeciwnie, gdy n jest liczb± pierwsz± :
x^2 == 1 (mod n) i x != 1 (mod n) i x != -1 (mod n)
Wtedy z w³asno¶ci kongruencji
n|(x^2-1)
n|(x-1)(x+1)
n|(x-1) lub n|(x+1)
x == 1 (mod n) lub x == -1 (mod n)
(zreszt± nie mo¿e mieæ wiêcej pierwiastków ni¿ te dwa ju¿ znalezione bo to s± pierwiastki wielomianu st. 2)

KNOWN BUGS:
modExp nie dzia³a dla podstaw >= 65536 = 2^16 bo 2^16 * 2^16 % n  
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
	/* Wynik liczony jest sprytnie aby zaoszczêdziæ na mno¿eniach tzn 
	 * patrz±c na binarn± reprezentacjê wyk³adnika przemna¿amy rezultat przez bazê o odpowiedniej wadze (2^i) modulo n  tylko wtedy gdy odpowiedni bit wyk³adnika jest '1' 
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
	/* Zwraca 'true' gdy n jest liczb± z³o¿on± (je¶li istnieje nietrywialny pierwiastek z 1 modulo n to n na pewno jest z³o¿ona) 'false' je¶li n jest liczb± pseudopierwsz± przy podstawie b*/
	ulint y;
	ulint t,u; ulint x; ulint i; ulint tmp;

	/* je¶li parzysta to z³o¿ona */
	if ( !(n & 1) ) 
		return true;

	/* Rozk³adamy n - 1 na takie u,t ¿e n - 1 = u*2^t (¿eby móc pó¼niej podnosiæ u*2^i do kwadratu i sprawdzaæ czy przypadkiem nie istnieje nietrywialny pierwiastek z jedynki (modulo n) co oznacza³oby ¿e n jest z³o¿one */
	u = n - 1; t = 0 ;
	while ( !(u & 1) ) {
		u = u / 2;
		t++;
	}

	/* 'x' jest b^u (mod n), y w pêtli jest wykorzystywane jako 'poprzednia' warto¶æ 'x' (do sprawdzenia czy dany pierwiastek z jedynki jest 'trywialny')*/
	y = x = modExp(b, u, n);
	for ( i = 0; i <= t; i++ ) {
		x = (x * x) % n;

		/* Sprawdzamy istnienie nietrywialnego pierwiastka (ró¿nego od 1,-1) z jedynki */	
		if ( x == 1 && ( y != 1 && y != n-1 ) )
			return true;

		y = x;
	}
	/* Tw. Fermata nie spe³nione -> l z³o¿ona */
	if ( x != 1 )
		return true;

	return false;
}/*}}}*/
ulint test(ulint n, ulint k) {/*{{{*/
	/* Zwraca 'true' je¶li n jest liczb± pseudopierwsz± w stosunku do 'k' losowo wygenerowanych podstaw z zakresu 1..n-1*/

	if ( n == 2 ) return true;
	/* je¶li parzysta to z³o¿ona */
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
