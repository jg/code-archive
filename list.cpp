#include <cstdlib>

using namespace std;

template <class type> 
class list{
	type *head,*tail;
	int listLength;

	typedef struct cListElement{
		type value;
		cListElement *next;
		cListElement *previous;
	}listElement;


	list() {/*{{{*/
		listElement *a,*b; 
		a = new listElement;
		b = new listElement;
		head = a; 
		tail = b;
		a->next     = b; a->prev = 0;
		b->previous = a; b->next = 0;
		listLength = 0;
	}/*}}}*/
	void pushFront(type value) {/*{{{*/
		listElement *a;
		a = new listElement;

		a->value = value;

		a->next          = head->next;
		a->previous 		 = head;
		head->next       = a;
		head->next->prev = a;

		listLength++;
	}/*}}}*/
	void pushBack(type value) {/*{{{*/
		listElement *a;
		a = new listElement;

		a->value = value;

		a->next 				 = tail;
		a->previous      = tail->previous;
		tail->prev->next = a;
		tail->prev       = a;

		listLength++;
	}/*}}}*/
	int remove(listElement *element, type *c = NULL) {/*{{{*/
		listElement *a,*b;
		type value;

		a = head; while ( a != tail && a != element ) a = a->next; if ( a == tail ) return -1;

		b = a->prev;
		b->next = a->next; a->next->prev = b;

		if ( c != NULL ) *c = a->value;

		delete a;
		return 0;
	}/*}}}*/
	listElement* find(type a) {/*{{{*/
		listElement *b;
		b = head; while  ( b != tail && b->value != a )  b = b->next; 
		if ( b == tail ) return NULL;
		return b;
	}/*}}}*/
	int len() {/*{{{*/
		return listLength;
	}/*}}}*/
	type popFront() {/*{{{*/
		listElement element = head->next;
		type value = element->value;
		head->next = head->next->next;
		head->next->prev = head;

		delete element;
		return value;
	}/*}}}*/
	type popBack() {/*{{{*/
		listElement element = tail->previous;
		type value = element->value;
		tail->previous = tail->previous->previous;
		tail->previous->next = tail;

		delete element;
		return value;
	}/*}}}*/

};


