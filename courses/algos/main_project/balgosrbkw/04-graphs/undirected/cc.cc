#include "cc.hh"

namespace {
constexpr std::size_t ID_NONE = static_cast<std::size_t>(-1);
}

CC::CC(const Graph &g) : _count(0), _ids(g.vcount(), ID_NONE) {
  for (std::size_t v = 0; v < g.vcount(); ++v)
    if (_ids[v] == ID_NONE) {
      _dfs(g, v);
      ++_count;
    }
}

void CC::_dfs(const Graph &g, std::size_t v) {
  _ids[v] = _count;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (_ids[*it] == ID_NONE)
      _dfs(g, *it);
}
