#include <iostream>
#include <assert.h>
#include <stdlib.h>

using namespace std;

template<class T>
class Queue {
  T *tab;
  int size, capacity, first;
  public:

  Queue(int capacity_) {
    first = size = 0;
    capacity = capacity_;
    tab = new T[capacity];
  }

  bool full() {
    return size == capacity;
  }

  bool empty() {
    return size == 0;
  }

  void insert(T el) {
    assert(!full());
    int index = (first+size) % capacity;
    tab[index] = el;
    ++size;
  }

  T get() {
    assert(!empty());
    T ret = tab[first];
    first = (first+1) % capacity;
    --size;
    return ret;
  }

  void pr() { 
    cout << "first: " << first << endl;
    cout << "size: " << size << endl;
    for ( int i = 0; i < capacity; ++i )
      cout << "  " << tab[i];
    cout << endl;
    cout << endl;
  }

  ~Queue() {
    delete tab;
  }
};

#define CAPACITY 1000
int randkey() {
  return rand() % CAPACITY;
}



int main() {
  srand(time(NULL));
  Queue<int> q(10);
}
