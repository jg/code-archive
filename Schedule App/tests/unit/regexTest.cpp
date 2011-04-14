#include "../../src/regex.h"
#include <gtest/gtest.h>


TEST(Regex, test) {
  regex monday("[pP]oniedziałek");
  EXPECT_EQ(Regex::test("poniedziałek", monday), true);
  EXPECT_EQ(Regex::test("Poniedziałek", monday), true);
  EXPECT_EQ(Regex::test("Poniedziałek stuff", monday), false);
  EXPECT_EQ(Regex::test("sthelseentirel", monday), false);

  // regex wednesday("[śŚ]roda"); TODO
  regex wednesday("środa"); 
  EXPECT_EQ(Regex::test("środa", wednesday), true);
}



