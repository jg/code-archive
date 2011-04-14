#ifndef RegexInclude
#define RegexInclude

#include <string>
#include <vector>
#include <iostream>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

class Regex {
  public:
    static bool test(string text, regex pattern); /* test whether pattern matches text */
    static bool match(string text, regex pattern, vector<string> &match); /* test whether pattern matches text & fill match struct (holds captured subexpressions */
    static void printMatches(string text, regex pattern);
};

#endif
