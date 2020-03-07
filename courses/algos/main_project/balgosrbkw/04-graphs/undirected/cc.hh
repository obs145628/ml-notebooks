#pragma once

#include "graph.hh"
#include <vector>

/// Connected Components
/// Find all connected components of a grapg
class CC {
public:
  /// Build connected components of `g`
  CC(const Graph &g);

  /// Returns the total number of connected components
  std::size_t count() const { return _count; }

  /// Check if `v` and `w` belong to the same component
  bool connected(std::size_t v, std::size_t w) const { return id(v) == id(w); }

  /// Returns the component identifier for vertex `v`
  /// Between 0 and count() - 1
  std::size_t id(std::size_t v) const { return _ids[v]; }

private:
  std::size_t _count;
  std::vector<std::size_t> _ids;

  // Recursive dfs call
  void _dfs(const Graph &g, std::size_t v);
};
