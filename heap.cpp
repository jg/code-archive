#include <vector>
#include <cstdio>
using namespace std;
template <class type> 
class heap { 
	public:
	typedef struct sHeapElement {
		type value;
	}heapElement;
	int size;
	int vSize;
	vector<type> tab;

	heap() {/*{{{*/
		size = 0;
	}/*}}}*/
	int leftChild(int n) {/*{{{*/
		return 2*n+1;
	}/*}}}*/
	int rightChild(int n) {/*{{{*/
		return 2*n+2;
	}/*}}}*/
	int parent(int n) {/*{{{*/
		return n%2==0?(n-2)/2:(n-1)/2;
	}/*}}}*/
	void insert(type value) {/*{{{*/
		tab.push_back(value);
		size++;
		upHeap(size-1);
	}/*}}}*/
	void put(type value) {/*{{{*/
		tab.push_back(value);
		size++;
	}/*}}}*/
	void swap(int a, int b) {/*{{{*/
		type tmp = tab[a];
		tab[a] = tab[b]; tab[b] = tmp;
	}/*}}}*/
	void upHeap(int n) {/*{{{*/
		while ( n != 0 && tab[n] >= tab[parent(n)] ) {
			swap(n, parent(n));
			n = parent(n);
		}
	}/*}}}*/
	void downHeap(int n) {/*{{{*/
		while ( n <= parent(size-1)  && ( tab[n] < tab[leftChild(n)] || tab[n] < tab[rightChild(n)] ) ) {
			if ( 2*n+2 > size-1 ) {
				if ( tab[n] < tab[leftChild(n)] ) {
				//one leaf case
				swap(n,leftChild(n));
				n = leftChild(n);
				}else {
					break;
				}
			}else {
			int tmp = tab[rightChild(n)] < tab[leftChild(n)]?leftChild(n):rightChild(n);
			swap(n, tmp);
			n = tmp;
			}
		}
	}/*}}}*/
	type removeMax() {/*{{{*/
		if ( size > 0 ) {
			type tmp;
			tmp = tab.at(0);
			swap(0, --size);
			downHeap(0);
			return tmp;
		}else {
			printf("No elements on heap\n");
		}
	}/*}}}*/
	void makeHeap(type *array, int n) {/*{{{*/
		for ( int i = 0; i < n; i++ ) tab[i] = array[i];
		for ( int i = parent(size); i > 0; i-- ) 
			downHeap(i);
	}/*}}}*/
	void sort() {/*{{{*/
		/* sort() destroys the heap property, use makeHeap to restore */
		int n = size;
		for ( int i = 0; i < n; i++ ) {
			pr();
			swap(0, --size);
			downHeap(0);
		}
		size = n;
	}/*}}}*/
	void pr() {/*{{{*/
		for (int i = 0; i < size; i++) printf("%d  ", tab.at(i));
		printf("\n");
	}/*}}}*/
	void inOrderWalk(void (*f)()){
	}
};
