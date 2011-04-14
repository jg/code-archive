#include "../src/block.h"
#include <gtest/gtest.h>


TEST(Block, blockInit) { 
  Block<int> block(4);
  block.pushBack(12);
  block.pushBack(12);
  block.pushBack(12);
  block.pushBack(12);
  block.remove(0);
  block.pushBack(12);
}
TEST(Block, blockFull) {
  Block<int> block(2);
  block.pushBack(12);
  block.pushBack(13);
  block.pushBack(13);
}
TEST(Block, blockEmpty) {
  Block<int> block(2);
  block.popBack();
  block.popBack();
  block.popBack();
}


