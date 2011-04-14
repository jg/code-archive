#ifndef BlockInclude
#define BlockInclude

#define pr(x) cout << x << endl << flush;

#include <iostream>
#include <stdexcept>

using namespace std;

template<class T>
class Block {/*{{{*/
  private:
    int block_size_;
    void block_size(int);
    bool *cellFreeP;
  public:
    T *array;
    Block(int block_size); 
    ~Block();

    int get_first_free_cell(); /* returns block_size when fullP */
    int get_last_free_cell(); /* returns -1 when fullP */ 
    int get_first_taken_cell(); /* returns block_size when emptyP */
    int get_last_taken_cell(); /* returns -1 on when emptyP */
    int block_size();
    void push_back(T);
    T pop_back();
    bool full(); /* full & empty predicates */
    bool empty();
    void remove(int);

    class Iterator;
    Iterator begin();
    Iterator end();
};/*}}}*/

template<class T>
 Block<T>::Block(int block_size) : block_size_(block_size){/*{{{*/
  array = new T[block_size];
  cellFreeP = new bool[block_size];
  for ( int i = 0; i < block_size; i++ ) cellFreeP[i] = true; 
}/*}}}*/

template<class T>
 Block<T>::~Block() {/*{{{*/
  delete[] array;
  delete[] cellFreeP;
}/*}}}*/

template<class T>
 void Block<T>::block_size( int block_size ) {/*{{{*/
  block_size_ = block_size;
}/*}}}*/

template<class T>
 int Block<T>::block_size() {/*{{{*/
  return block_size_;
}/*}}}*/

template<class T>
 int Block<T>::get_first_taken_cell() {/*{{{*/
  int i;
  for ( i = 0; i < block_size(); i++ )
    if ( !cellFreeP[i] ) return i;
  return -1;
}/*}}}*/

template<class T>
 int Block<T>::get_last_taken_cell() {/*{{{*/
  int i;
  for ( i = block_size()-1; i >= 0; i-- )
    if ( !cellFreeP[i] ) return i;
  return i;
}/*}}}*/

template<class T>
 int Block<T>::get_first_free_cell() {/*{{{*/
  int i;
  for ( i = 0; i < block_size(); i++ )
    if ( cellFreeP[i] ) return i;
  return i;
}/*}}}*/

template<class T>
 int Block<T>::get_last_free_cell() {/*{{{*/
  int i;
  for ( i = block_size()-1; i >= 0; i-- )
    if ( cellFreeP[i] ) return i;
  return i;
}/*}}}*/

template<class T>
 bool Block<T>::full() {/*{{{*/
  return (get_first_free_cell() == block_size() ? true : false);
}/*}}}*/

template<class T>
 bool Block<T>::empty() {/*{{{*/
  return (get_first_taken_cell() == -1 ? true : false);
}/*}}}*/

template<class T>
 void Block<T>::remove(int index) {/*{{{*/
  assert( (index >= 0) && (index < block_size()) ); /* assert block index in range */
  cellFreeP[index] = true;
}/*}}}*/

template<class T>
 void Block<T>::push_back(T element) {/*{{{*/
  int index = get_first_free_cell();
  assert( index < block_size() ); // block not full assert
  array[index] = element;
  cellFreeP[index] = false;  
}/*}}}*/

template<class T>
 T Block<T>::pop_back() {/*{{{*/
  int index = get_last_taken_cell();
  assert( index != -1 ); // block not empty assert

  T value = array[index];
  cellFreeP[index] = true;  
  return value;
}/*}}}*/

// Block iterator code

template<class T>
 typename Block<T>::Iterator Block<T>::begin() {/*{{{*/
  int index = get_first_taken_cell();
  return Iterator(this, index);
}/*}}}*/

template<class T>
 typename Block<T>::Iterator Block<T>::end() {/*{{{*/
  return Iterator(this, get_last_taken_cell());
}/*}}}*/


template<class T>
 class Block<T>::Iterator {/*{{{*/
  protected:
    int elementIndex;
    Block<T>* blockPtr;
  public:
    Iterator() {}
    Iterator( Block<T>* block, int index );
    // : blockPtr(block), elementIndex(index) {}
    bool operator==( const Iterator& );
    bool operator!=( const Iterator& );
    Iterator& operator=( const Iterator& rhs );
    Iterator& operator++(int);
    T& operator*();
    T* operator->();
};/*}}}*/

template<class T>
 Block<T>::Iterator::Iterator( Block<T>* block, int index ) : blockPtr(block), elementIndex(index) {}

template<class T>
 typename Block<T>::Iterator& Block<T>::Iterator::operator=( const Iterator& rhs ) {/*{{{*/
  blockPtr = rhs.blockPtr;
  elementIndex = rhs.elementIndex;
  return (*this);
}/*}}}*/

template<class T>
 typename Block<T>::Iterator& Block<T>::Iterator::operator++(int) { /*{{{*/
  int i;
  for ( i = elementIndex+1; (i < (blockPtr->block_size())) && (blockPtr->cellFreeP[i]); i++ ) {}  // skip free cells 
  if ( i < (blockPtr->block_size()) ) elementIndex = i; // assign new value only if in range 
  return (*this);
}/*}}}*/

template<class T>
 bool Block<T>::Iterator::operator==( const Iterator& rhs ) {/*{{{*/
  return ( (this->elementIndex == rhs.elementIndex) && (this->blockPtr == rhs.blockPtr) );
}/*}}}*/

template<class T>
 bool Block<T>::Iterator::operator!= ( const Iterator& rhs ) {/*{{{*/
  return !( *this == rhs );
}/*}}}*/

template<class T>
 T& Block<T>::Iterator::operator*() {/*{{{*/
  return blockPtr->array[elementIndex];
}/*}}}*/

#endif
