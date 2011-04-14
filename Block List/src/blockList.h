#ifndef BlockListInclude
#define BlockListInclude

#include "list.h"
#include "block.h"

using namespace std;
// notice, this is a list of pointers to Block<T>
// the interface is that of a normal list, 
// difference lies in implementation
template<class T>
class BlockList : List< Block<T>* > {/*{{{*/ 
  private:
    int block_size_;
    int element_count_;
    void block_size(int);
    void element_count(int);
  public:
    BlockList(int block_size) : block_size_(block_size) {}

    /* Iterators */
    class Iterator;
    Iterator begin();
    Iterator end();
    /* Methods */
    void push_back(T);
    T pop_back();
    int element_count();
    /* Accessors */
    int block_size();

};/*}}}*/

template<class T>
 void BlockList<T>::push_back(T element) {/*{{{*/
  if ( (this->tail->previous == this->head) || (this->tail->previous->data->full()) ) { // if last block full or we have no block at all 
    // cout << "initializing new block with block_size = " << block_size << endl;
    Block<T> *block = new Block<T>(block_size()); // add new block to the list 
    List< Block<T>* >::push_back(block); 
    // cout << "pushed new block to list" << endl;
  }
  this->tail->previous->data->push_back(element); // push the element onto the block 
  // cout << "pushed value to block" << endl;
}/*}}}*/

template<class T>
 T BlockList<T>::pop_back() {/*{{{*/
  assert( (this->tail->previous != this->head) && (!this->tail->previous->data->empty()) ); // if list not empty && block not empty 
  T value;
  value = this->tail->previous->data->pop_back();
  if ( this->tail->previous->data->empty() )  // remove block if empty 
    List< Block<T>* >::pop_back();
  return value;
}/*}}}*/

template<class T>
 void BlockList<T>::block_size(int block_size) {/*{{{*/
  block_size_ = block_size;
}/*}}}*/

template<class T> 
int BlockList<T>::block_size() {/*{{{*/
  return block_size_;
}/*}}}*/

template<class T>
int BlockList<T>::element_count() {/*{{{*/
  return element_count_;
}/*}}}*/

template<class T>
void BlockList<T>::element_count( int element_count ) {/*{{{*/
  element_count_ = element_count;
}/*}}}*/

/* BlockList Iterator */

template<class T>
 typename BlockList<T>::Iterator BlockList<T>::begin() {/*{{{*/
  if ( !this->empty() ) {
    int index = this->head->next->data->get_first_taken_cell();
    return Iterator(this->head->next, index);
  }
}/*}}}*/

template<class T>
 typename BlockList<T>::Iterator BlockList<T>::end() {/*{{{*/
  if ( !this->empty() ) {
    return Iterator(this->tail->previous, this->tail->previous->data->get_last_taken_cell());
  }
}/*}}}*/


template<class T> 
class BlockList<T>::Iterator {/*{{{*/
  private:
  public:
  typename List< Block<T>* >::ListNode *nodePtr;
  int elementIndex;

  Iterator() {}
  Iterator(typename List< Block<T>* >::ListNode *nodePtr, int index) : nodePtr(nodePtr), elementIndex(index) {}
  bool operator==( const Iterator& );
  bool operator!=( const Iterator& );
  Iterator& operator=( const Iterator& rhs );
  Iterator& operator++(int);
  T& operator*();
  T* operator->();
};/*}}}*/

template<class T> 
bool BlockList<T>::Iterator::operator==( const Iterator& rhs) {/*{{{*/
  return ( (nodePtr == rhs.nodePtr) && (elementIndex == rhs.elementIndex) );
}/*}}}*/

template<class T> 
bool BlockList<T>::Iterator::operator!=( const Iterator& rhs) {/*{{{*/
  return !( (*this) == rhs );
}/*}}}*/

template<class T> 
typename BlockList<T>::Iterator& BlockList<T>::Iterator::operator=(const Iterator& rhs) {/*{{{*/
  nodePtr = rhs.nodePtr;
  elementIndex = rhs.elementIndex;
  return (*this);
}/*}}}*/

template<class T> 
typename BlockList<T>::Iterator& BlockList<T>::Iterator::operator++(int) { /* TODO: double get_last_taken_cell call *//*{{{*/
  if ( (nodePtr->next->next != NULL) && (elementIndex == nodePtr->data->get_last_taken_cell()) ) { // if in last element of block, move to the next block in list, unless last block 
    // pr("Moving to the next block");
    nodePtr = nodePtr->next;
    elementIndex = nodePtr->data->get_first_taken_cell();
  }else if ( elementIndex != nodePtr->data->get_last_taken_cell() ){ // not last element 
    int i;
    for ( i = elementIndex+1; i < (nodePtr->data->block_size()) && (nodePtr->data->cellFreeP[i]); i++ ) {} // skip free cells 
    if ( i < (nodePtr->data->block_size()) ) elementIndex = i;
  }else {} // last element, last node 

  return (*this);
}/*}}}*/

template<class T> 
T& BlockList<T>::Iterator::operator*() { /*{{{*/
  return nodePtr->data->array[elementIndex];
}/*}}}*/


#endif
