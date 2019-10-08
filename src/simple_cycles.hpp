// Copyright (c) 2019 herenvarno
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef __SIMPLE_CYCLES_HPP__
#define __SIMPLE_CYCLES_HPP__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/property_map/property_map.hpp>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

namespace bglex {

/**
 * Find the simple cycles in a graph
 */
template <typename G>
vector<vector<typename G::vertex_descriptor>> simple_cycles(G g_) {

  using namespace boost;
  typedef typename G::vertex_descriptor Vertex;
  typedef typename G::edge_descriptor Edge;

  vector<vector<Vertex>> cycles;

  // Define a aux function _unlock()
  auto _unblock = [](Vertex thisnode, set<Vertex> &blocked,
                     std::unordered_map<Vertex, set<Vertex>> &B) {
    std::stack<Vertex> stack;
    stack.push(thisnode);
    while (!stack.empty()) {
      Vertex node = stack.top();
      stack.pop();
      if (blocked.find(node) != blocked.end()) {
        blocked.erase(node);
        for (auto &n : B[node]) {
          stack.push(n);
        }
        B[node].clear();
      }
    }
  };

  // Find strong connected components with at least two vertices in it.
  auto all_nodes = vertices(g_);
  auto subG = g_.create_subgraph(all_nodes.begin(), all_nodes.end());
  vector<int> component(num_vertices(subG));
  int num = strong_components(
      subG, make_iterator_property_map(component.begin(),
                                       get(boost::vertex_index, subG)));

  stack<vector<Vertex>> sccs;
  for (int i = 0; i < num; i++) {
    vector<Vertex> temp;
    for (int j = 0; j < component.size(); j++) {
      if (component[j] == i) {
        temp.push_back(j);
      }
    }
    if (temp.size() > 1) {
      sccs.push(temp);
    }
  }

  // Record self-loops and remove them
  vector<Vertex> self_loops;
  for (auto vd : vertices(subG)) {
    Edge ed;
    bool exist = false;
    tie(ed, exist) = edge(vd, vd, subG);
    if (exist) {
      self_loops.push_back(vd);
    }
  }
  for (auto vd : self_loops) {
    remove_edge(vd, vd, subG);
  }

  // main loop
  while (!sccs.empty()) {
    auto scc = sccs.top();
    sccs.pop();
    auto sccG = subG.create_subgraph(scc.begin(), scc.end());
    auto startnode = scc[0];
    vector<Vertex> path = {startnode};
    set<Vertex> blocked;
    set<Vertex> closed;
    blocked.insert(startnode);
    std::unordered_map<Vertex, set<Vertex>> B;
    std::queue<Vertex> q;
    q.push(startnode);
    while (!q.empty()) {
      Vertex thisnode = q.front();
      typename G::out_edge_iterator oei, oei_end;
      for (tie(oei, oei_end) = out_edges(thisnode, sccG); oei != oei_end;
           oei++) {
        Vertex nextnode = target(*oei, sccG);
        if (nextnode == startnode) {
          cycles.push_back(path);
          for (auto p : path) {
            closed.insert(p);
          }
          break;
        } else if (blocked.find(nextnode) != blocked.end()) {
          path.push_back(nextnode);
          q.push(nextnode);
          closed.erase(nextnode);
          blocked.insert(nextnode);
          continue;
        }
      }
      if (closed.find(thisnode) != closed.end()) {
        _unblock(thisnode, blocked, B);
      } else {
        for (tie(oei, oei_end) = out_edges(thisnode, sccG); oei != oei_end;
             oei++) {
          Vertex nextnode = target(*oei, sccG);
          if (B[nextnode].find(thisnode) == B[nextnode].end()) {
            B[nextnode].insert(thisnode);
          }
        }
      }
      q.pop();
      path.pop_back();
    }
    auto H = subG.create_subgraph(scc.begin(), scc.end());
    vector<int> c(num_vertices(H));
    int n = strong_components(
        H, make_iterator_property_map(c.begin(), get(boost::vertex_index, H)));

    for (int i = 0; i < n; i++) {
      vector<Vertex> temp;
      for (int j = 0; j < c.size(); j++) {
        if (c[j] == i) {
          temp.push_back(j);
        }
      }
      if (temp.size() > 1) {
        sccs.push(temp);
      }
    }
  }
  return cycles;
}

} // namespace bglex

#endif
