#include "../src/list.h"
#include <gtest/gtest.h>


TEST(List, listInitAndPush) { 
  List<int> list;
  list.pushBack(12);
  list.pushBack(23);
  EXPECT_EQ(list.popBack(), 23);
  EXPECT_EQ(list.popBack(), 12);
}


