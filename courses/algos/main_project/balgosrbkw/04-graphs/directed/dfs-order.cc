#include "dfs-order.hh"

DFSOrder::DFSOrder(const Digraph &g) : _marked(g.vcount(), false) {
  for (std::size_t v = 0; v < g.vcount(); ++v)
    if (!_marked[v])
      dfs(g, v);

  _rev_post = std::vector<std::size_t>(_post.rbegin(), _post.rend());
}

void DFSOrder::dfs(const Digraph &g, std::size_t v) {
  _pre.push_back(v);

  _marked[v] = true;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (!_marked[*it])
      dfs(g, *it);

  _post.push_back(v);
}
