#ifndef ListInclude
#define ListInclude

#include <iostream>
#include <stdexcept>
#include <assert.h>

using namespace std;


template<class T>
 class List { /* Generic singly-linked list */
  private:
    int size_;
    void set_size(int);
  public:
    struct ListNode { /* List helper class, stores pointer + data values */ /*{{{*/
      ListNode *previous, *next;
      T data;
      ListNode(T, ListNode*, ListNode*); /* value, previous, next */
    };/*}}}*/
    /* head/tail are sentinels, class invariant: head (tail) is before (after) every other element in the list */
    ListNode *head, *tail; /* TODO: should be ronly, i know */
    List();
    ~List();
    void push_back(T);
    int get_size();
    T pop_back();
    bool empty();
};

template<class T>
List<T>::ListNode::ListNode(T element,
                            ListNode *previous, 
                            ListNode *next) : data(element), 
                                              previous(previous), 
                                              next(next) { } 
template<class T>
List<T>::List() {/*{{{*/
  head = new ListNode(NULL, NULL, tail);
  tail = new ListNode(NULL, head, NULL);
  set_size(0);
}/*}}}*/

template<class T>
List<T>::~List() {
  delete head;
  delete tail;
}

template<class T>
void List<T>::push_back(T element) {/*{{{*/
  ListNode *tmp = new ListNode(element, tail->previous, tail);
  tail->previous->next = tmp;
  tail->previous = tmp;
  set_size(get_size()+1);
}/*}}}*/

template<class T>
T List<T>::pop_back() {/*{{{*/
  assert( (tail->previous != head) );

  ListNode *tmp = tail->previous;
  T value = tmp->data;
  tail->previous->previous->next = tail;
  tail->previous = tail->previous->previous;
  delete tmp;
  set_size(get_size()-1);
  return value;
}/*}}}*/

template<class T>
bool List<T>::empty() {/*{{{*/
  return ( tail->previous == head );
}/*}}}*/

template<class T>
int List<T>::get_size() {/*{{{*/
  return size_;
}/*}}}*/

template<class T>
void List<T>::set_size( int size ) {/*{{{*/
  size_ = size;
}/*}}}*/

#endif
