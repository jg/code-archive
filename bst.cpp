#define NULL 0
template<class type>
class bst {
	typedef struct sNode {
		sNode *leftChild,*rightChild,*parent;
		type value;
	}node;
	node *root;	
	bst() {
	}
	void insert(type value) {/*{{{*/
		node *a,*b;
		a = b = root;
		while ( a != NULL ) {
			b = a;
			a = value <= a->value ? a->leftChild : a->rightChild;
		}
		a = new node; a->leftChild = a->rightChild = a->parent = NULL; a->value = value;
		a->parent = b;
		
		/* b was root */
		if ( b == NULL )  
			root = a;
		else {
			if ( value <= b->value )
				b->leftChild = a;
			else
				b->rightChild = a;
		}
	}/*}}}*/
	int find(type key) {/*{{{*/
		node *a = root;
		while ( a != NULL && a->value != key ) 
				a = key < a->value ? a->leftChild : a->rightChild;
		return a == NULL?0:1;
	}/*}}}*/
	type successor(node *node) {
		if ( node->right != NULL )	
			return minimum();
			/*TODO*/
	}
	type maximum() {/*{{{*/
		node *a = root;
		while ( a->rightChild != NULL ) a = a->rightChild;
		return a->value;
	}/*}}}*/
	type minimum() {/*{{{*/
		node *a = root;
		while ( a->leftChild != NULL ) a = a->leftChild;
		return a->value;
	}/*}}}*/
	node* maximum() {/*{{{*/
		node *a = root;
		while ( a->rightChild != NULL ) a = a->rightChild;
		return a;
	}/*}}}*/
	node* minimum() {/*{{{*/
		node *a = root;
		while ( a->leftChild != NULL ) a = a->leftChild;
		return a;
	}/*}}}*/

	void remove() {
	}
}
