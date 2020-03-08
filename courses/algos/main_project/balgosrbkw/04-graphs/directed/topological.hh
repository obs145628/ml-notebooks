#pragma once

#include "digraph.hh"
#include <vector>

/// Class to sort vertices by topological order
/// This ordering is for constrained-based scheduling
/// Each vertex a represent a task
/// An edge u -> v means that task u must be completed before doing v
/// Return an ordering of the vertices (tasks) that gives the order of the tasks
/// that satisfy the constraints
/// Only works for DAG (Directed Acyclic Graph)
/// Detects if not a DAG
class Topological {

public:
  Topological(const Digraph &g);
  Topological(const Topological &) = delete;

  /// returns true if the graph is a directed acyclic graph
  bool is_dag() const { return !_order.empty(); }

  // Returns the order, or empty if not a DAG
  const std::vector<std::size_t> &order() const { return _order; }

private:
  std::vector<std::size_t> _order;
};
