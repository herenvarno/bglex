#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include "all_topological_sorts.hpp"
#include <boost/test/unit_test.hpp>

using namespace bglex;

BOOST_AUTO_TEST_CASE(test1) {
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
      Graph;
  Graph G(5);
  boost::add_edge(0, 1, G);
  boost::add_edge(1, 2, G);
  boost::add_edge(1, 4, G);
  boost::add_edge(3, 4, G);

  vector<vector<int>> reference = {
      {0, 1, 2, 3, 4}, {0, 1, 3, 4, 2}, {0, 1, 3, 2, 4}, {0, 3, 1, 2, 4},
      {0, 3, 1, 4, 2}, {3, 0, 1, 2, 4}, {3, 0, 1, 4, 2}};

  std::set<string> reference_str;
  for (auto r : reference) {
    string token = "";
    for (auto v : r) {
      token += to_string(v) + ",";
      reference_str.insert(token);
    }
  }

  vector<vector<Graph::vertex_descriptor>> ts;
  ts = bglex::all_topological_sorts<Graph>(G);

  BOOST_CHECK_EQUAL(ts.size(), reference.size());
  for (auto n = 0; n < ts.size(); n++) {
    BOOST_CHECK_EQUAL(ts[n].size(), 5);
    string token = "";
    for (auto m = 0; m < ts[n].size(); m++) {
      token += to_string(ts[n][m]) + ",";
    }
    BOOST_CHECK(reference_str.find(token) != reference_str.end());
  }
}