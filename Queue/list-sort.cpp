#include <iostream>
#include <assert.h>
#include <stdlib.h>

using namespace std;

template<class T>
class List {
  struct Node {
    Node *next;
    Node *prev;
    T data;
  };

  public:
  Node *Head, *Tail;
    
  List() {
    Head = Tail = new Node;
    Head->next = Tail;
    Head->prev = NULL;
    Tail->prev = Head;
    Tail->next = NULL;
  }

  void insert(T el) {
    Node *tmp = new Node;
    tmp->data = el;

    //tmp-
    tmp->prev = Tail->prev;
    tmp->next = Tail;
    Tail->prev->next = tmp;
    Tail->prev = tmp;
  }

  void sort() {
    Node *sorted, *toinsert, *tmp;
    // points to the last element of the sorted part of the list
    sorted = Head->next;

    while ( sorted->next != Tail ) {
      toinsert = sorted->next;
      tmp = sorted;
      while ( (toinsert->data < tmp->data) && (tmp != Head) ) {
        // pick toinsert up
        toinsert->prev->next = toinsert->next;
        toinsert->next->prev = toinsert->prev;

        // insert toinsert one position to the left
        toinsert->prev = tmp->prev;
        toinsert->next = tmp;

        toinsert->prev->next = toinsert;
        toinsert->next->prev = toinsert;

        tmp = toinsert->prev;

      }
      // toinsert hasn't moved, it is the new sorted
      if ( sorted->next == toinsert ) sorted = toinsert;
    }
  }

  void pr() {
    Node *tmp = Head->next;
    while ( tmp != Tail ) {
      cout << "  " << tmp->data;
      tmp = tmp->next;
    }
    cout << endl;
  }

  ~List() {
  }
};

int r() {
  return rand()%100;
}

int main() {
  List<int> l;
  for ( int i = 0; i < 10; ++i ) 
    l.insert(r());
  // l.insert(1);
  l.pr();
  l.sort();
  l.pr();
}
