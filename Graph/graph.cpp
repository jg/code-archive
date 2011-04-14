#ifndef GRAPH
#define GRAPH
#include <iostream>
#include <vector>

#define REP(x, n) for (int x = 0; x < (n); ++x)
#define VAR(v, n) __typeof(n) v = (n)
#define FOREACH(i, c) for (VAR(i, (c).begin()); i != (c).end(); ++i)

using namespace std;


// TODO: move definitions outside Graph class

template<typename V,typename E>
class Graph {

  struct Edge : E {/*{{{*/
    int source, dest;
    int rev; // holds position of the inverted edge in g[dest] for const time lookup

    friend ostream& operator<<(ostream &out, const Edge& g) {/*{{{*/
      out << "Hello world";
      return out;
    }/*}}}*/
  };/*}}}*/
  struct Vertex : vector<Edge>, V {/*{{{*/
    void edgeD(Edge &e) {/*{{{*/
      // Adds a directed edge
      g[e.source].push_back(e);
    }/*}}}*/
    void edgeU(Edge &e) {/*{{{*/
      // Adds a undirected edge
      e.rev = g[e.source].size();
      g[e.source].push_back(e);

      e.rev = g[e.dest].size();
      g[e.dest].push_back(e);
    }/*}}}*/
  };/*}}}*/

  vector<Vertex> g; 
  int size_; // number of vertices in graph

  public:

  Graph(int n) : size_(n) {}
  Graph() : size_(0) {}

  int size() { return size_; }
  void fromString() {
  }
  string toString() {/*{{{*/
    ostream os;
    REP(i, g.size()) {
      os << i << " : ";
      FOREACH(it, g[i]) os << *it << "  ";
      os << endl;
    }
  }/*}}}*/

};


#endif 
