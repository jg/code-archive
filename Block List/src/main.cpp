#include <iostream>
#include "list.h"
#include "block.h"
#include "blockList.h"

using namespace std;

#define pr(x) cout << x << endl << flush;


int main() {
  BlockList<int> blockList(2);
  blockList.push_back(1);
  blockList.push_back(2);
  blockList.push_back(3);
  blockList.push_back(4);
  blockList.push_back(5);
  blockList.push_back(6);
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  pr(blockList.pop_back());
  // for ( it = blockList.begin(); it != blockList.end(); it++ ) {
  //   pr(*it);
  // }
  // BlockList<int>::Iterator it;
  // = blockList.begin();
  // it = blockList.begin();
  // pr(*it);
  // pr(*it++);
  // pr(*it++);
  // pr(*it++);
  // pr(*it++);
  // cout << ( it == blockList.end() ) << endl;

  // blockList.push_back(11);

  // Block<int> block(2);

  // block.push_back(2);
  // block.push_back(2);
  // block.push_back(2);

  // cout << blockList.tail->previous->data.fullP();
  // List< Block<int> >::blockList.tail->previous->data.fullP();
  // blockList::List< Block<int> >.tail;
  // blockList.push_back(12);
  // blockList.push_back(13);
  // cout << blockList.pop_back() << endl;;
  // cout << blockList.pop_back() << endl;;

  // List<int> list;
  // list.push_back(12);
  // cout << list.pop_back() << endl;
  // cout << list.pop_back() << endl;
  // return 0;

  // Block<int> block(2);
  // block.push_back(12);
  // block.push_back(13);
  // block.pop_back();
  // block.pop_back();
  // block.pop_back();
  // block.push_back(14);
  // Block<int>::Iterator it;
  // for ( it = block.begin(); it != block.end(); it++ )
  //   cout << *it << endl;
  // cout << *it << endl;
  // pr(*it++);
  // pr(*it++);
  
  // it++;
  // pr(*it);
  // it = block.begin();

}
