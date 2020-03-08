#include "transitive-closure.hh"
#include "directed-dfs.hh"

TransitiveClosure::TransitiveClosure(const Digraph &g)
    : _mat(g.vcount() * g.vcount(), false), _count(g.vcount()) {
  for (std::size_t v = 0; v < _count; ++v) {
    DirectedDFS dfs(g, v);
    for (std::size_t w = 0; w < _count; ++w)
      _mat[v * _count + w] = dfs.reachable(w);
  }
}
