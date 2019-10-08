#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "simple_cycles.hpp"
#include <boost/test/unit_test.hpp>

using namespace bglex;

BOOST_AUTO_TEST_CASE(test1) {
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
      Graph;
  Graph G(4);
  boost::add_edge(0, 1, G);
  boost::add_edge(1, 2, G);
  boost::add_edge(2, 3, G);
  boost::add_edge(3, 1, G);
  vector<vector<Graph::vertex_descriptor>> cycles;
  cycles = bglex::simple_cycles<Graph>(G);
  for (auto v : cycles) {
    for (auto x : v) {
      cout << x << " -> ";
    }
    cout << endl;
  }
}