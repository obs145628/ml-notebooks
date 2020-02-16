#pragma once

#include "graph.hh"
#include <vector>

class DFSPath {
public:
  /// Find all DFS paths in `g` starting from source vertex `s`
  DFSPath(const Graph &g, std::size_t s);

  /// Returns if there is a path between `s` and `v`
  bool connected(std::size_t v) const;

  /// Returns path from `s` to `v`, both included in the result
  std::vector<std::size_t> path_to(std::size_t v) const;

private:
  std::size_t _src;
  std::vector<std::size_t> _edges;

  // Recursive call to visit all connected vertices
  void dfs(const Graph &g, std::size_t v);
};
