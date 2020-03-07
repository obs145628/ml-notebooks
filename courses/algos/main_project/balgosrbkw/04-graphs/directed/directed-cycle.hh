#pragma once

#include "digraph.hh"
#include <vector>

/// Check if a graph has a cycle
class DirectedCycle {

public:
  DirectedCycle(const Digraph &g);
  DirectedCycle(const DirectedCycle &) = delete;

  /// Returns true if the graph has a cycle
  bool has_cycle() const;

  // vertices on the cycle, or empty if there is no cycles
  const std::vector<std::size_t> &cycle() const;

private:
  std::vector<std::size_t> _pred;
  std::vector<bool> _stack;
  std::vector<std::size_t> _cycle;

  /// Recursive call to visit all reachable vertices
  void dfs(const Digraph &g, std::size_t v);

  // When going to v -> w, and cycle w to w found, use this function to build
  // the cycle
  void build_cycle(std::size_t v, std::size_t w);
};
