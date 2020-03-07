#pragma once

#include "digraph.hh"
#include <vector>

/// Class to check with vertices are reachable from a source vertex
/// Use Depth First Search Ordering
class DirectedDFS {

public:
  DirectedDFS(const Digraph &g, std::size_t s);
  DirectedDFS(const DirectedDFS &) = delete;

  /// Returns true if there is a path from `s` to `v`
  bool reachable(std::size_t v) const;

private:
  std::vector<bool> _marked;

  /// Recursive call to visit all reachable vertices
  void dfs(const Digraph &g, std::size_t v);
};
