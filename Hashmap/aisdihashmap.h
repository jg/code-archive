/** 
@file aisdihashmap.h

AISDIHashMap and related functions interface.
      
@author
Pawel Cichocki

@date  
last revision
- 2006.03.24 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2006 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#ifndef ASDHASH
#define ASDHASH

#include <utility>
#include <list>
#include <string>

using namespace std;

// Yeah, so i'm lazy, sue me
#define T template<class K, class V, unsigned hashFunc(const K&), int compFunc(const K&,const K&)> 
#define M AISDIHashMap<K, V, hashFunc, compFunc>
#define TABSIZE 65536
#define pr(x) cout << x << endl;
#define prit(it) cout << "map[" << it.first << "] = " << it.second << endl;
/// Default keys' comparing function for AISDIHashMap - it uses
/// operator== by default.
/// @returns 0 if keys are equal, non-zero otherwise.
/// @param key1 First key to compare. 
/// @param key2 Second key to compare. 
template <class Key>   
inline int _compFunc(const Key& key1,const Key& key2)
{
   return (key1==key2);
};

/// A map with a similar interface to std::map.
template<class K, class V,
         unsigned hashFunc(const K&),
         int compFunc(const K&,const K&)=&_compFunc<K> >
class AISDIHashMap
{
  public:
    typedef K key_type;
    typedef V value_type;
    typedef unsigned size_type;
    typedef pair<K, V> hash_list_node;
    typedef list< pair<K, V> > hash_list;
    typedef typename hash_list::iterator hash_list_iterator;

    list< pair<K, V> > tab[TABSIZE]; 
    int size_, first_, last_;

    AISDIHashMap();
    ~AISDIHashMap();

    /// Copying constructor.
    explicit AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a);

    /// const_iterator.
    class const_iterator{
      private:
        int index;
        hash_list_iterator it;
        AISDIHashMap *map;
      public: 
        // const_iterator() { }
        const_iterator(int i, hash_list_iterator j, AISDIHashMap *m) : index(i), it(j), map(m) {} 
        const_iterator& operator++();
        const_iterator& operator++(int);
        const_iterator& operator--();
        const_iterator& operator--(int);
        V& operator*() const;
    };
    /// iterator.
    class iterator{
      public: 
        int index;
        hash_list_iterator it;
        AISDIHashMap *map;
      public: 
        iterator() {} 
        iterator(int i, hash_list_iterator j, AISDIHashMap *m) : index(i), it(j), map(m) {} 
        iterator& operator++();
        iterator& operator++(int);
        iterator& operator--();
        iterator& operator--(int);
        bool operator!=(const iterator &it) const; 
        bool operator==(const iterator &it) const; 
        pair<key_type, value_type> *operator->();
        V& operator*();
    };


   /// Returns an iterator addressing the first element in the map.
   inline iterator begin();
   // TODO: uncomment!
   // inline const_iterator begin();

   /// Returns an iterator that addresses the location succeeding the last element in a map.
   inline iterator end();
   inline const_iterator end() const;
   
   /// Inserts an element into the map.
   /// @returns A pair whose bool component is true if an insertion was
   ///          made and false if the map already contained an element
   ///          associated with that key, and whose iterator component coresponds to
   ///          the address where a new element was inserted or where the element
   ///          was already located.
   inline std::pair<iterator, bool> insert(const std::pair<K, V>& entry);

   /// Returns an iterator addressing the location of the entry in the map
   /// that has a key equivalent to the specified one or the location succeeding the
   /// last element in the map if there is no match for the key.
   iterator find(const K& k);
   const_iterator find(const K& k) const;
 
   /// Inserts an element into a map with a specified key value
   /// if one with such a key value does not exist.
   /// @returns Reference to the value component of the element defined by the key.
   V& operator[](const K& k);   

   /// Tests if a map is empty.
   bool empty( );

   /// Returns the number of elements in the map.
   size_type size() ;

   /// Returns the number of elements in a map whose key matches a parameter-specified key.
   size_type count(const K& _Key) const;

   /// Removes an element from the map.
   /// @returns The iterator that designates the first element remaining beyond any elements removed.
   iterator erase(iterator i);
   
   /// Removes a range of elements from the map.
   /// The range is defined by the key values of the first and last iterators
   /// first is the first element removed and last is the element just beyond the last elemnt removed.
   /// @returns The iterator that designates the first element remaining beyond any elements removed.
   iterator erase(iterator first, iterator last);
   
   /// Removes an element from the map.
   /// @returns The number of elements that have been removed from the map.
   ///          Since this is not a multimap itshould be 1 or 0.
   size_type erase(const K& key);

   /// Erases all the elements of a map.
   void clear( );


};


// ASD MAP IMPLEMENTATION
//------------------------------------------------------------


T M::AISDIHashMap() { /*{{{*/
  size_ = 0;
}/*}}}*/
T M::~AISDIHashMap() { /*{{{*/
}/*}}}*/
T M::AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a) {/*{{{*/
}/*}}}*/
T typename M::iterator M::begin() {/*{{{*/
  int i;
  for ( i = 0; i < TABSIZE; i++ ) {
    if ( !tab[i].empty() ) 
      return iterator(i, tab[i].begin(), this);
  }
  return end();
}/*}}}*/
T typename M::iterator M::end() {/*{{{*/
  // return end() of last non empty list
  int i;
  for ( i = TABSIZE-1; i > 0; --i ) {
    if ( !tab[i].empty() ) 
      return iterator(i, tab[i].end(), this);  
  }
  return iterator(TABSIZE-1, tab[TABSIZE-1].end(), this);
}/*}}}*/
T typename M::iterator M::find(const K& k) {/*{{{*/
  int index = hashFunc(k);
  if ( tab[index].empty() )  {
    return end();
  } else {
    iterator it(index, tab[index].begin(), this);
    return it;
  }
}/*}}}*/
T typename M::const_iterator M::find(const K& k) const {/*{{{*/
  int index = hashFunc(k);
  if ( tab[index].empty() ) 
    return end();
  else
    const_iterator it(index, tab[index], this);
}/*}}}*/
T typename M::size_type M::size() { /*{{{*/
  return size_;
}/*}}}*/
T typename M::size_type M::count(const K& key) const {/*{{{*/
  int index = hashFunc(key);
  return tab[index].size();
}/*}}}*/
T typename M::iterator M::erase(iterator i) {/*{{{*/
  if ( i != end() ) {
    iterator next = i;
    next++;

    i.map->tab[i.index].erase(i.it);
    size_--;
    return next;
  } else  {
    return i;
  }
}/*}}}*/
T typename M::iterator M::erase(iterator first, iterator last) {/*{{{*/
  iterator i = first;
  while ( first != last ) first = erase(first);
  return last;
}/*}}}*/
T typename M::size_type M::erase(const K& key) {/*{{{*/
  return (erase(find(key)) != end());
}/*}}}*/
T void M::clear() {/*{{{*/
  erase(begin(), end());
}/*}}}*/
T std::pair<typename M::iterator, bool> M::insert(const std::pair<K, V>& entry) {/*{{{*/
  int index = hashFunc(entry.first);
  
  // insert new value, return reference
  size_++;
  tab[index].push_front(entry);
  iterator it(index, tab[index].begin(), this);

  return pair<iterator, bool>(it, true);
}/*}}}*/
T bool M::empty() {/*{{{*/
  return size() == 0;
}/*}}}*/
T typename M::value_type& M::operator[](const typename M::key_type& k) {/*{{{*/
  int index = hashFunc(k);

  // return value if key present
  for ( hash_list_iterator it = tab[index].begin(); it != tab[index].end(); it++ ) 
    if ( it->first == k ) return it->second;

  typename M::value_type v;
  insert(M::hash_list_node(k, v));

  return tab[index].front().second;
}/*}}}*/

// ITERATOR IMPLEMENTATION
//------------------------------------------------------------

T bool M::iterator::operator!=(const iterator &it) const {/*{{{*/
  if ( (index == it.index) && 
       (this->it == it.it) && 
       ( this->map == it.map ) 
     ) 
    return false;
  else
    return true;
}/*}}}*/
T bool M::iterator::operator==(const iterator &it) const {/*{{{*/
  return !( *this != it );
}/*}}}*/
T typename M::iterator& M::iterator::operator++() {/*{{{*/
  // Return if already at the end
  if ( (*this) == map->end() ) return *this;

    ++it;

    // Special case: we have reached the end of map
    if ( *this == map->end() ) return *this;

    // Special case: we're at the end of the list
    // We need to move index to the next nonempty list
    // And set it to its begin()
    if ( it == map->tab[index].end() ) {
      // AT THE END of current hash list, increase index if possible
      while ( map->tab[++index].empty() && (index < TABSIZE) ) ;
      it = map->tab[index].begin();
    }

  return *this;
}/*}}}*/
T typename M::iterator& M::iterator::operator++(int) {/*{{{*/
  iterator *temp = new iterator(this->index, this->it, this->map);
  ++(*this);
  return *temp;

}/*}}}*/
T typename M::iterator& M::iterator::operator--() {/*{{{*/
  if ( (*this) != map->begin() ) {
    if ( it == map->tab[index].begin() ) {
      // AT THE BEGINNING of current hash list, decrease index if possible
        while ( map->tab[--index].empty() && (index > 0) ) ;
        it = map->tab[index].end();
    } 
    it--;
  }
}/*}}}*/
T typename M::iterator& M::iterator::operator--(int) {/*{{{*/
  iterator *temp = new iterator(this->index, this->it, this->map);
  --(*this);
  return *temp;
}/*}}}*/
T typename M::value_type& M::iterator::operator*() {/*{{{*/
  return it->second;
}/*}}}*/
T pair<typename M::key_type, typename M::value_type>* M::iterator::operator->() {/*{{{*/
  return &(*it);
}/*}}}*/

static const size_t InitialFNV = 2166136261U;
static const size_t FNVMultiple = 16777619;

unsigned hashF(const string& s) {/*{{{*/
 size_t hash = InitialFNV;
 for(size_t i = 0; i < s.length(); i++)
 {
   hash = hash ^ (s[i]);       /* xor  the low 8 bits */
   hash = hash * FNVMultiple;  /* multiply by the magic number */
 }
 return hash%(TABSIZE-1);


}/*}}}*/

#endif
