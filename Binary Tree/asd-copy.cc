/** 
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.
    
@author
Pawel Cichocki, Michal Nowacki

@date  
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

// COMMENT OUT BEFORE RELEASE
// #include "gtest/gtest.h"

#include <assert.h>
#include <algorithm>

#include <iostream>

#ifdef _SUNOS
#include "/home/common/dyd/aisdi/tree/TreeMap.h" 
#else
#include "TreeMap.h"
#endif

#define MAP TreeMap
#define MAXSIZE 10
#define IT TreeMap::iterator
#define PAIR std::pair<int, string>
#define PRMAP for_each(m.begin(), m.end(), print ); cout << endl;
#define PR(p) print(p)

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p) {
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

using namespace std;
/// A helper class.
class TreeMapDetail //Helper
{
protected:
   friend class TreeMap;

   /// Stupid example of a method that modifies a protected field in 
   /// the TreeMap class. Feel free to remove this method or add new
   /// ones here.
   static void erase(TreeMap* tm, const TreeMap::Key& k)
   {
      tm->root=NULL; // we just modified a protected field in tree map
   }
};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

TreeMap::TreeMap() {/*{{{*/
   root = NULL;
}/*}}}*/
TreeMap::TreeMap( const TreeMap& m ) {/*{{{*/
/// Content of existing TreeMap object is copied into the new object. 
  // Key k;
  // Val v;
  // m.insert(const_cast*p);
   ///@todo Implement this

  // iterator it1 = m.begin();
  // for ( ; it1 != end(); ++it1 ) {
  //   this->insert(*it1);
  // }
    // m.insert(*it1);
  // }
}/*}}}*/
TreeMap::~TreeMap() {/*{{{*/
   clear();
}/*}}}*/
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry) {/*{{{*/

  // cout << endl;
  // PR(entry);
  // cout << endl;
// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//          made and false if the map already contained an element
//          associated with that key, and whose iterator component coresponds to
//          the address where a new element was inserted or where the element
//          was already located.
   iterator i = begin();
   for ( ; i != end(); ++i )
   {
      if ( i->first == entry.first )
        return std::make_pair(i, (bool)false);
   }
    i = unsafe_insert(entry);
    return std::make_pair(i, (bool)false);
}/*}}}*/
TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry) {/*{{{*/
  // PR(entry);
// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.
  TreeNode *z = new TreeNode(entry);
  TreeNode *y = NULL;
  TreeNode *x;
  x = root;

  while ( x != NULL ) {
    y = x;
    if ( (z->data.first) < (x->data.first) )
      x = x->left;
    else
      x = x->right;
  }

  z->parent = y;
  if ( y == NULL ) {
    root = z;
  } else if ( z->data.first < y->data.first ) {
    y->left = z;
  } else {
    y->right = z;
  }

  return iterator(z);
}/*}}}*/
TreeMap::iterator TreeMap::find(const Key& k) {/*{{{*/
// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
  iterator i;
  for(i=begin(); i!=end(); ++i)
    if(i->first==k) return i;

  return end();
}/*}}}*/
TreeMap::const_iterator TreeMap::find(const Key& k) const {/*{{{*/
  cout << "FIND" << endl;
  const_iterator i;
  for(i=begin(); i!=end(); ++i)
    if(i->first==k) return i;

  return end();
}/*}}}*/
TreeMap::Val& TreeMap::operator[](const Key& k) {/*{{{*/
  // cout <<"OPEATOR[]" << endl;
// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
  // Find if element already exists in map
  iterator i = find(k);
  if ( i != end() ) return i->second;

  // Insert element to map, return reference to value
  Val v;
  i = unsafe_insert(std::pair<Key, std::string>(k, v));
  return i->second;
}/*}}}*/
bool TreeMap::empty( ) const {/*{{{*/
// Tests if a map is empty.
   return root==NULL;
}/*}}}*/
TreeMap::size_type TreeMap::size( ) const {/*{{{*/
// Returns the number of elements in the map.
  iterator i;
  int size;
  for ( i = begin(), size = 0; i != end(); ++i, ++size );
  return size;
}/*}}}*/
TreeMap::size_type TreeMap::count(const Key& _Key) const {/*{{{*/
// Returns the number of elements in a map whose key matches a parameter-specified key.
  if ( find(_Key) != end() )
    return 1;
  else
    return 0;
}/*}}}*/
TreeMap::iterator TreeMap::erase(TreeMap::iterator i) {/*{{{*/
  // cout << "ERASE BY ITERATOR" << endl;
// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
  if ( i.node == NULL ) return end();
  TreeNode *z = i.node;
  TreeNode *y = NULL;
  TreeNode *x = NULL;
  iterator ret = ++i;

  if ( (z->left == NULL) || (z->right == NULL) ) 
    y = z;
  else {
    y = (++iterator(z)).node;
    // y = ++z;
    // ret = y;
    // --z;
  }
  if ( y->left != NULL ) 
    x = y->left;
  else
    x = y->right;

  
  if ( x != NULL ) {
    x->parent = y->parent;
  }

  if ( y->parent == NULL )
    root = x;
  else if ( y == y->parent->left )
    y->parent->left = x;
  else
    y->parent->right = x;


  if ( y != z ) {
    z->data = y->data;
    ret = (++iterator(z)).node;
  }

  delete y;

  return ret;
}/*}}}*/
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l) {/*{{{*/
  // cout << "ERASE BY RANGE" << endl;
// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
  while ( f != l ) f = erase(f);
  return erase(l);
}/*}}}*/
TreeMap::size_type TreeMap::erase(const Key& key) {/*{{{*/
  // cout << "ERASE BY KEY " << key <<  endl;
// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
  iterator i = find(key);
  if ( i != end() ) {
    erase(i);
    return 1;
  } else
    return 0;
}/*}}}*/
void TreeMap::clear( ) {/*{{{*/
// Erases all the elements of a map.
  erase(begin(), end());
}/*}}}*/
bool TreeMap::struct_eq(const TreeMap& another) const/*{{{*/
{
  // cout << "struct_eq" << endl;
  // cout << "STUCT_EQ"; 
  iterator it1 = begin();
  iterator it2 = another.begin();
  for ( ; it1 != end(); it1++, it2++ ) {
    if ((it1.node->left == NULL) &&  (it2.node->left != NULL)     || 
        (it1.node->right != NULL) &&  (it2.node->right == NULL)   ||
        (it1.node->parent == NULL) &&  (it2.node->parent != NULL) ||
        (it1.node->parent != NULL) &&  (it2.node->parent == NULL) 
       )
      return false;
  }
  if ( (it1 == end()) && (it2 != end()) ) return false;
  if ( (it1 != end()) && (it2 == end()) ) return false;
  return true;
}/*}}}*/
bool TreeMap::info_eq(const TreeMap& another) const/*{{{*/
{
  // cout << "info_eq" << endl;
  // cout << "INFO_EQ"; 
  iterator it1 = begin();
  iterator it2 = another.begin();

  for ( ; (it1 != end() && it2 != end()) ; it1++, it2++ ) {
    if ( (it1->first != it2->first) || (it1->second != it2->second) ) return false;
  }
  if ( (it1 == end()) && (it2 != end()) ) return false;
  if ( (it1 != end()) && (it2 == end()) ) return false;

 return true;
}/*}}}*/
TreeMap::const_iterator& TreeMap::const_iterator::operator ++() {/*{{{*/
  // cout << "operator++" << endl;
  // cout << node->data.second << endl;
  if ( node == NULL ) { return *this; }

  TreeNode *x = node;
  TreeNode *y = NULL;

  if ( x->right != NULL ) {
    // Return Tree-Minimum(right[x])
    x = x->right;
    while ( x->left != NULL ) x = x->left;

    node = x;

    return *this;
  }
  y = x->parent;
  while ( ( y != NULL ) && ( x == y->right ) ) {
    x = y;
    y = y->parent;
  }
  node = y;
  return *this;
}/*}}}*/
TreeMap::const_iterator TreeMap::const_iterator::operator++(int) {/*{{{*/
  const_iterator it(*this);
  (++(*this));
  return it;
}/*}}}*/
TreeMap::const_iterator& TreeMap::const_iterator::operator--() {/*{{{*/
  cout << "Operator--" << endl;
  if ( node == NULL ) return *this;

  TreeNode *x = node;
  TreeNode *y;

  if ( x->left != NULL ) {
    // Return Tree-Maximum(left[x])
    x = x->left;
    while ( x->right != NULL ) x = x->right;

    node = x;
    return *this;
  }
  y = x->parent;
  while ( ( y != NULL ) && ( x == y->left) ) {
    x = y;
    y = y->parent;
  }
  // return iterator(y);
  node = y;

   ///@todo Implement this
   return *this;
}/*}}}*/
TreeMap::const_iterator TreeMap::const_iterator::operator--(int) {/*{{{*/
   ///@todo Implement this
   const_iterator it(*this);
   (--*this);
   return it;
}/*}}}*/
TreeMap& TreeMap::operator=(const TreeMap& ) {/*{{{*/
/// Assignment operator copy the source elements into this object.
   ///@todo Implement this
   return *this;
}/*}}}*/
TreeMap::iterator TreeMap::begin() { /*{{{*/
/// Returns an iterator addressing the first element in the map
  if ( root == NULL ) return iterator(NULL);
  TreeNode *x = root;
  while ( x->left != NULL ) 
    x = x->left;

  return iterator(x);
}/*}}}*/
TreeMap::const_iterator TreeMap::begin() const { /*{{{*/
  if ( root == NULL ) return iterator(NULL);

  TreeNode *x = root;
  while ( x->left != NULL ) 
    x = x->left;

  return const_iterator(x);
}/*}}}*/
TreeMap::iterator TreeMap::end() { /*{{{*/
/// Returns an iterator that addresses the location succeeding the last element in a map
  return iterator(NULL);
}/*}}}*/
TreeMap::const_iterator TreeMap::end() const { /*{{{*/
/// Returns an iterator that addresses the location succeeding the last element in a map
   ///@todo Implement this
   return const_iterator(NULL);
}/*}}}*/
// Default Tests/*{{{*/
//////////////////////////////////////////////////////////////////////////////


#include <map>
#include <assert.h>
/// The big mean test function ;)
void test() {
  // A typedef used by the test.
  // typedef std::map<int, std::string> TEST_MAP;
  //typedef SmallMap<int, std::string> TEST_MAP;
  typedef TreeMap TEST_MAP;

  std::cout << "Testy uzytkownika" << std::endl;

  // TEST_MAP m, k, l;  

  // k.insert(PAIR(30,"30"));
  // k.insert(PAIR(30,"30"));
  // m.insert(PAIR(30,"30"));
  // m.insert(PAIR(100, "100"));
  // k.insert(PAIR(110, "110"));
  // assert( m.struct_eq(k) == true );
  // assert( k.struct_eq(m) == true );
  // assert( l.struct_eq(l) == true );
  // m[2] = "Merry";
  // m[4] = "Jane";
  // m[8] = "Korwin";
  // m[4] = "Magdalena";

  // m.insert(PAIR(15,"15"));
  // m.insert(PAIR(20, "20"));
  // m.insert(PAIR(18, "18"));
  // m.insert(PAIR(25, "25"));

  // m.insert(PAIR(30,"15"));
  // m.insert(PAIR(15,"15"));
  // m.insert(PAIR(20,"15"));
  // m.insert(PAIR(18,"15"));
  // m.insert(PAIR(25,"15"));
  // m.erase(20);
  // cout << m.root->data.first;
  // cout << m.root->left->right->right->data.first;
  // IT i = m.find(20);
  // i++;
  // cout << i->first;
  // cout << i->first;

  // IT i = m.begin();
  // cout << i->first << endl;
  // i++;
  // cout << i->first << endl;
  // i++;
}
// int main(int argc, char **argv) {
//    test();
// }
/*}}}*/
// TESTY - MOJE/*{{{*/
//////////////////////////////////////////////////////////////////////////////

/*
namespace {
void print(const std::pair<int, std::string>& p) {
   std::cout<<p.first<<", "<<p.second<<std::endl;
}
// DEFINES

string rand_key() {
  static std::string key(10, '0');
  std::string::iterator ch = key.begin();
  for (size_t i = 0; i<sizeof(key)-1; ++i, ++ch) {
    *ch = 'A' + (rand() & 7);
  }
  return key;
}
TEST(ASDMap, unsafe_insert) {
  MAP m;
  ASSERT_EQ(m.size(), 0);
  for ( int i=0; i < MAXSIZE; ++i ) {
    m.unsafe_insert(PAIR(i, rand_key()));
  }
  ASSERT_EQ(m.size(), MAXSIZE);
}
TEST(ASDMap, safe_insert) {
  MAP m;
  ASSERT_EQ(m.size(), 0);
  m.insert(PAIR(1, "Batman"));
  for ( int i=0; i < MAXSIZE; ++i ) {
    m.insert(PAIR(1, rand_key()));
  }
  ASSERT_EQ(m.size(), 1);
}
TEST(ASDMap, find) {
  MAP m;
  ASSERT_EQ(m.size(), 0);
  string key = rand_key();

  m.insert(PAIR(1, "Mary"));
  m.insert(PAIR(2, "Jane"));
  m.insert(PAIR(3, "Watson"));

  IT i = m.find(4);
  bool b = i == m.end();
  ASSERT_EQ(b, true);

  i = m.find(2);
  ASSERT_EQ(i->second, "Jane");
}
TEST(ASDMap, erase_by_iterator) {
  MAP m;

  m.insert(PAIR(1, "Mary"));
  m.insert(PAIR(2, "Jane"));
  m.insert(PAIR(3, "Watson"));
  m.insert(PAIR(4, "Batman"));
  ASSERT_EQ(m.size(), 4);

  // Pathologcal cases, erasing non-existing elements or end()
  IT i = m.find(4384);
  bool b = m.erase(i) == m.end();
  ASSERT_EQ(b, true);
  m.erase(m.end());

  ASSERT_EQ(m.size(), 4);
  // PRMAP

  // Erasing Waston, should get batman 
  ASSERT_EQ(m.erase(m.find(3))->second, "Batman");
  ASSERT_EQ(m.size(), 3);

  // Now erasing Mary, should get Jane
  ASSERT_EQ(m.erase(m.find(1))->second, "Jane");
  ASSERT_EQ(m.size(), 2);

  // l33t Erase two remaining elements
  m.erase(m.erase(m.begin()));
  ASSERT_EQ(m.size(), 0);

  ASSERT_EQ(m.begin() == m.end(), true);

}
TEST(ASDMap, erase_by_range) {
  MAP m;

  m.insert(PAIR(1, "Mary"));
  m.insert(PAIR(2, "Jane"));
  m.insert(PAIR(3, "Watson"));
  m.insert(PAIR(4, "Batman"));
  m.insert(PAIR(5, "Robin"));
  m.insert(PAIR(6, "Joker"));

  IT i = m.erase(m.find(2), m.find(5));
  ASSERT_EQ(m.size(), 2);
  ASSERT_EQ(m.begin()->second, "Mary");
  ASSERT_EQ((++m.begin())->second, "Joker");

}
TEST(ASDMap, clear) {
  MAP m;

  m.insert(PAIR(1, "Mary"));
  m.insert(PAIR(2, "Jane"));
  m.insert(PAIR(3, "Watson"));
  m.insert(PAIR(4, "Batman"));
  m.insert(PAIR(5, "Robin"));
  m.insert(PAIR(6, "Joker"));

  ASSERT_EQ(m.size(), 6);
  m.clear();
  ASSERT_EQ(m.size(), 0);
  ASSERT_EQ(m.begin() == m.end(), true);

}
TEST(ASDMap, array_operator) {
  MAP m;

  m[1] = "Mary";
  m[2] = "Jane";
  m[3] = "Watson";
  m[4] = "Batman";
  m[5] = "Robin";
  m[6] = "Joker";

  m[1] = "Hari";
  ASSERT_EQ(m[1], "Hari");
  m[0] = "stuff";
  ASSERT_EQ(m.size(), 7);

}
}  // namespace
*/
/*
TEST(ASDMap, array_operator ) {
  MAP m;
  m["key"] = "value";
  IT it = m.find("key");
  ASSERT_EQ((*it), "value");
}

TEST(ASDMap, clear) {
  MAP m;

  // Insert some stuff & erase it all
  for ( int i = 0; i < MAXSIZE; i++ ) {
    m.insert(PAIR(rand_key(), rand_key()));
  }
  m.clear();
  ASSERT_EQ(m.size(), 0);

  // Insert more stuff
  for ( int i = 0; i < MAXSIZE; i++ ) {
    m.insert(PAIR(rand_key(), rand_key()));
  }
  ASSERT_EQ(m.size(), MAXSIZE);

  for ( int i = 0; i < MAXSIZE; i++ ) {
    m.insert(PAIR(rand_key(), rand_key()));
  }
  m.clear();
  ASSERT_EQ(m.size(), 0);
}

TEST(ASDMap, test_size_while_erasing_stuff) {
  MAP m;
  for ( int i = 0; i < MAXSIZE; i++ ) m.insert(RANDPAIR);
  IT it = m.begin();
  int size = m.size();
  ASSERT_EQ(m.size(), MAXSIZE);
  for ( int i = 0; i < MAXSIZE; i++ ) {
    it = m.erase(it);
    ASSERT_EQ(m.size(), --size);
  }
  
}



*/
/*}}}*/
//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

   // My tests COMMENT BEFORE RELEASE
  // ::testing::InitGoogleTest(&argc, argv);
  // return RUN_ALL_TESTS();
   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
   // Test2();
   //system("PAUSE");
   // return EXIT_SUCCESS;
/*
*/
