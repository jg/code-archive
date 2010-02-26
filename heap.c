#include <stdio.h>

#define MAXHEAP 1000
#define parent(i) i%2==0?(i-2)/2:(i-1)/2
#define lchild(i) i*2+1
#define rchild(i)	i*2+2
typedef int element;
element tab[MAXHEAP];
int hsize;

void swap ( element *a, element *b ) {/*{{{*/
	element tmp; tmp = *a; *a = *b; *b = tmp;
}/*}}}*/
void downHeap (int k) {/*{{{*/
	int r,l;
	r = rchild(k);
	l = lchild(k);
	while ( tab[k] < tab[r] || tab[k] < tab[l] ) { 
		if ( tab[r] > tab[l] ) {
			swap(&tab[r],&tab[k]);
			k = r;
		}else {
			swap(&tab[l],&tab[k]);
			k = l;
		}
		r = rchild(k);
		l = lchild(k);
	}
	
}/*}}}*/
void upHeap (int n) {/*{{{*/
	while ( tab[n] > tab[parent(n)] && n != 0) {
		swap(&tab[n], &tab[parent(n)]);
		n = parent(n);
	}
}/*}}}*/
void constructPQ() {/*{{{*/
	hsize = 0;
}/*}}}*/
void insertPQ( element el ) {/*{{{*/
	tab[hsize++] = el;
	upHeap(hsize-1);
}/*}}}*/
void removePQ( element *el ) {/*{{{*/
	*el = tab[0];
	tab[0] = tab[hsize--];
	downHeap(0);
}/*}}}*/
void replacePQ() {
}
void deletePQ() {
}
void prHeap() {/*{{{*/
	int i;
	for ( i = 0; i < hsize; i++ )
		printf("%d  ", tab[i]);
	printf("\n");
}/*}}}*/
void heapSort() {
}
void makeHeap(int n) {
	int i;
	for (i=n/2;i>=0;i--) {
		downHeap(i);
	}
}
void pr(int k) {
	int i;
	for(i=0;i<k;i++)
		printf("%d  ", tab[i]);
	printf("\n");
}
int main() {
	/*
	constructPQ();
	*/
	int i;

	for(i=0;i<30;i++) {
		tab[i]=i;
	}
	pr(30);
	makeHeap(30);
	pr(30);
/*
	insertPQ(1);
	insertPQ(2);
	insertPQ(8);
	insertPQ(4);
	insertPQ(5);
	insertPQ(6);
	insertPQ(7);
	insertPQ(8);
	insertPQ(9);
	insertPQ(245);
	prHeap();
	tab[0]=0;
	prHeap();
	downHeap(0);
	prHeap();
	*/
	/*
	element el;
	removePQ(&el);
	printf("%d\n", el);
	prHeap();
	*/
}
