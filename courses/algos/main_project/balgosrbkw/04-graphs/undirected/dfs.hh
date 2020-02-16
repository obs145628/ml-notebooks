#pragma once

#include "graph.hh"
#include <vector>

/// Depth first Search
/// Visit all vertices connected to a source vertex.
/// Visit order is defined by going recursively through the unvisited vertices
/// adjacent to the current vertex
class DFS {

public:
  /// Performs A DFS on Graph `g` starting at source vertex `s`
  DFS(const Graph &g, std::size_t s);
  DFS(const DFS &) = delete;

  /// Returns true if there is path between vertices `s` and `v`
  bool connected(std::size_t v) const { return _marked[v]; }

  /// Returns the number of vertices connected to `s`
  std::size_t count() const { return _count; }

private:
  std::size_t _count;
  std::vector<bool> _marked;

  // Recursive call to visit all connected vertices
  void dfs(const Graph &g, std::size_t v);
};
