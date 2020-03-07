#pragma once

#include "graph.hh"
#include <vector>

/// Check if a graph is bipartite
/// A graph is bipartite if can be divided into 2 groups of vertices, where each
/// edge connected a vertex from one group to another
/// Can also be describer as 2-coloring problem:
/// The graph can be colored using 2 colors, such that there is no edge between
/// 2 vertices of the same color.
class Bipartite {

public:
  /// Check if graph `g` is bipartite
  Bipartite(const Graph &g);

  /// Returns true if the graph is bipartite
  bool is_bipartite() const { return _bp; }

private:
  std::vector<int> _colors;
  bool _bp;

  // Recursive DFS to assign colors
  void _dfs(const Graph &g, std::size_t v);
};
