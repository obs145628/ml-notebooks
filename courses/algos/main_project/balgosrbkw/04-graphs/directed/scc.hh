#pragma once

#include "digraph.hh"
#include <set>

/// Compute the Strongly Connected Components of the Graph G
/// 2 Vertices v and w are strongly connected if there is path from v to w, and
/// from w to v
/// SCC divide the graph into subsets of vertices, called components
/// These are maximal subsets of vertices where every vertex is strongly
/// connected to every other vexter of the subset, such that each vertex belongs
/// to only one subset.
class SCC {

public:
  SCC(const Digraph &g);

  /// Returns true if vertices `v` and `w` are strongly connected
  bool connected(std::size_t v, std::size_t w) const;

  /// Returns the number of strongly connected components
  std::size_t count() const;

  /// Returns the component identifier of vertex `v`, in [0, #vertices - 1]
  std::size_t id(std::size_t v) const;

  /// Get all vertices in component on index `idx`
  std::set<std::size_t> component(std::size_t idx) const;

private:
  std::vector<std::size_t> _ids;
  std::size_t _count;

  void kosaraju_run(const Digraph &g);
  void kosaraju_dfs(const Digraph &g, std::vector<bool> &marked, std::size_t v);
};
