#include "regex.h"

  bool Regex::test(string text, regex pattern) {/*{{{*/
    string::const_iterator start, end;
    start = text.begin();
    end = text.end();

    return regex_match(start, end, pattern, boost::match_default); 
  }/*}}}*/
  bool Regex::match(string text, regex pattern, vector<string> &match) {/*{{{*/
    smatch what;
    smatch::iterator it;
    string::const_iterator start, end;
    start = text.begin();
    end = text.end();

    bool found = regex_search(start, end, what, pattern, boost::match_default); 
    for(it = what.begin()+1; it != what.end(); it++) {
      if (*it != "") match.push_back(*it);
    }
    return found;
  }/*}}}*/
  void Regex::printMatches(string text, regex pattern) {/*{{{*/
    vector<string> matches;
    if (match(text, pattern, matches)) 
      copy (matches.begin(), matches.end(), ostream_iterator<string> (cout, "\n"));
  }/*}}}*/

