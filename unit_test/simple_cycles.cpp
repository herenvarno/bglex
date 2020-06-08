#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "simple_cycles.hpp"
#include <boost/test/unit_test.hpp>

using namespace bglex;

BOOST_AUTO_TEST_CASE(test1) {
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
      Graph;
  Graph G(6);
  boost::add_edge(0, 1, G);
  boost::add_edge(1, 2, G);
  boost::add_edge(2, 3, G);
  boost::add_edge(3, 1, G);
  boost::add_edge(2, 4, G);
  boost::add_edge(4, 5, G);
  boost::add_edge(5, 3, G);

  vector<vector<int>> reference = {{1, 2, 4, 5, 3}, {1, 2, 3}};

  vector<vector<Graph::vertex_descriptor>> cycles;
  cycles = bglex::simple_cycles<Graph>(G);

  BOOST_CHECK_EQUAL(cycles.size(), reference.size());
  for (auto n = 0; n < cycles.size(); n++) {
    BOOST_CHECK_EQUAL(cycles[n].size(), reference[n].size());
    for (auto m = 0; m < cycles[n].size(); m++) {
      BOOST_CHECK_EQUAL(cycles[n][m], reference[n][m]);
    }
  }
}