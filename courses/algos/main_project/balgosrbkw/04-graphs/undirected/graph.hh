#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

/// Represent an undirected graph G = <V, E>
/// With V set of vertices: v in V: v in [0, #V -1]
/// and E set of edges: e in E: e = (v_s, v_d). v_s in V, V_d in V
///
/// Implementation based on adjcenty-list:
/// Vector containing all adjacents vertices for every vertices.
class Graph {

public:
  using edge_iterator_t = std::vector<std::size_t>::const_iterator;

  /// Create a new graph with `vcount` vertices and 0 edges
  Graph(std::size_t vcount);

  /// Default implem right, but clostly operation
  Graph(const Graph &) = delete;
  Graph &operator=(const Graph &) = delete;

  Graph(Graph &&) = default;
  Graph &operator=(Graph &&) = default;

  /// Serialize a graph to text format
  static void serialize(const Graph &g, std::ostream &os);

  /// Load a graph from serialized text format
  static Graph unserialize(std::istream &is);

  /// Number of vertices
  std::size_t vcount() const { return _vcount; }

  /// NUmber of edges
  std::size_t ecount() const { return _ecount; }

  /// Add edge `v`-`w` to the graph
  /// Doesn't test for selp loop or parallel edges
  void add_edge(std::size_t v, std::size_t w);

  /// Returns an iterator to the beginning of the adjency list of vertex `v`
  edge_iterator_t adj_begin(std::size_t v) const;

  /// Returns an iterator to the end of the adjency list of vertex `v`
  edge_iterator_t adj_end(std::size_t v) const;

  /// Dump Graph to tree format
  void dump_tree(std::ostream &os);

private:
  std::size_t _vcount;
  std::size_t _ecount;
  std::vector<std::vector<std::size_t>> _adj;
};
