
#include "../src/blockList.h"
#include <gtest/gtest.h>


TEST(Block, oneBlockBlockListOverflowTest) { 
  BlockList<int> list(2);
  for ( int i = 0; i < 100; i++ )
    list.push_back(i);
    EXPECT_EQ(list.getSize(), 50);

}


