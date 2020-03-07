#include "bipartite.hh"

Bipartite::Bipartite(const Graph &g) : _colors(g.vcount(), -1), _bp(true) {
  for (std::size_t v = 0; v < g.vcount(); ++v)
    if (_colors[v] == -1) {
      _colors[v] = 0;
      _dfs(g, v);
    }
}

void Bipartite::_dfs(const Graph &g, std::size_t v) {
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (_colors[*it] == -1) {
      _colors[*it] = !_colors[v];
      _dfs(g, *it);
    } else if (_colors[v] == _colors[*it])
      _bp = false;
}
