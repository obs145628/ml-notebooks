#include "directed-dfs.hh"

DirectedDFS::DirectedDFS(const Digraph &g, std::size_t s)
    : _marked(g.vcount(), false) {
  dfs(g, s);
}

bool DirectedDFS::reachable(std::size_t v) const { return _marked[v]; }

void DirectedDFS::dfs(const Digraph &g, std::size_t v) {
  _marked[v] = true;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (!_marked[*it])
      dfs(g, *it);
}
