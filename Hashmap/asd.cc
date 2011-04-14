//
//                     **    UWAGA!   **
//
// Plik asd.cc przeznaczony jest tylko do wpisania wlasnych testow.
// Cala implementacja powinna znajdowac sie w pliku aisdihashmap.h

// #include<iostream>
// #include <list>
#include "gtest/gtest.h"
#include "aisdihashmap.h"


#define prmap(map) \
  for ( IT it = map.begin(); it != map.end(); ++it ) { \
    cout << "tab[" << it.index << "] = " << *it << endl; \
  }
// using namespace std;

typedef AISDIHashMap<string, string, hashF> MAP;
typedef AISDIHashMap<string, string, hashF>::iterator ITERATOR;
typedef ITERATOR IT;
typedef pair<string, string> PAIR;
#define RANDPAIR PAIR(rand_key(), rand_key())

#define MAXSIZE 1024
using namespace std;
namespace {

string rand_key() {
  static std::string key(10, '0');
  std::string::iterator ch = key.begin();
  for (size_t i = 0; i<sizeof(key)-1; ++i, ++ch) {
    *ch = 'A' + (rand() & 7);
  }
  return key;
}


TEST(ASDMap, insert) {
  MAP m;
  ASSERT_EQ(m.size(), 0);
  m.insert(PAIR("key", "value"));
  IT it = m.find("key");
  ASSERT_EQ((*it), "value");
  ASSERT_EQ(m.size(), 1);
}

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



}  // namespace
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

