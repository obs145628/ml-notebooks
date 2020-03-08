#pragma once

#include "digraph.hh"
#include <vector>

/// Class to create 3 vertices ordering based on DFS:
/// pre-order: visit a vertex the first time you see it
/// post-order: visit a vertex the first time you see it, after visiting all
/// reachable vertices reverse post-order
class DFSOrder {

public:
  DFSOrder(const Digraph &g);
  DFSOrder(const DFSOrder &) = delete;

  const std::vector<std::size_t> &pre_order() const { return _pre; }
  const std::vector<std::size_t> &post_order() const { return _post; }
  const std::vector<std::size_t> &reverse_post_order() const {
    return _rev_post;
  }

private:
  std::vector<bool> _marked;
  std::vector<std::size_t> _pre;
  std::vector<std::size_t> _post;
  std::vector<std::size_t> _rev_post;

  /// Recursive call to visit all reachable vertices
  void dfs(const Digraph &g, std::size_t v);
};
