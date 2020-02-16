#include "base.hh"

std::size_t degree(const Graph &g, std::size_t v) {
  std::size_t res = 0;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    ++res;
  return res;
}

std::size_t max_degree(const Graph &g) {
  std::size_t res = 0;
  for (std::size_t v = 0; v < g.vcount(); ++v) {
    auto deg = degree(g, v);
    if (deg > res)
      res = deg;
  }
  return res;
}

std::size_t avg_degree(const Graph &g) { return 2 * g.ecount() / g.vcount(); }

std::size_t count_self_loops(const Graph &g) {
  std::size_t res2 = 0;
  for (std::size_t v = 0; v < g.vcount(); ++v)
    for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
      res2 += v == *it;
  return res2 / 2;
}
