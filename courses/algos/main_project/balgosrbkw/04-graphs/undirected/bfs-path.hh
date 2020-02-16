#pragma once

#include "graph.hh"
#include <vector>

/// Bread First Search
/// Performs a BFS from source vertex, finding the closest path to all connected vertices
/// Visit all vertices separated by one edge from the source vertex
/// Then all vertices separated by 2 edges from the source
/// And so on
class BFSPath {
public:
  /// Find all BFS paths in `g` starting from source vertex `s`
  BFSPath(const Graph &g, std::size_t s);

  /// Returns if there is a path between `s` and `v`
  bool connected(std::size_t v) const;

  /// Returns shortest path from `s` to `v`, both included in the result
  std::vector<std::size_t> path_to(std::size_t v) const;

private:
  std::size_t _src;
  std::vector<std::size_t> _edges;

  // Recursive call to visit all connected vertices
  void bfs(const Graph &g, std::size_t v);
};
