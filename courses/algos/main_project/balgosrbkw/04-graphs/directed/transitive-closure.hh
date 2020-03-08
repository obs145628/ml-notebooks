#pragma once

#include "digraph.hh"
#include <vector>

class TransitiveClosure {

public:
  TransitiveClosure(const Digraph &g);
  TransitiveClosure(const TransitiveClosure &) = delete;

  // Returns true if there is a path from `v` to `w` in G
  bool reachable(std::size_t v, std::size_t w) const {
    return _mat[v * _count + w];
  }

private:
  std::vector<bool> _mat;
  std::size_t _count;
};
