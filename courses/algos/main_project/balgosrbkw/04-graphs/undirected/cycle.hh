#pragma once

#include "graph.hh"
#include <vector>

/// Detect if a graph as at least one cycle
/// A cycle is a path that starts and ends with the same vertex
class Cycle {

public:
  /// Check if graph `g` has a cycle
  Cycle(const Graph &g);

  /// Returns true if there is a cycle in `g`
  bool has_cycle() const { return _cycle; }

private:
  std::vector<bool> _marked;
  bool _cycle;

  // Recursive DFS to detect cycles
  void dfs(const Graph &g, std::size_t v, std::size_t u);
};
