#include "scc.hh"
#include "dfs-order.hh"
#include <cassert>

SCC::SCC(const Digraph &g) : _ids(g.vcount(), 0), _count(1) { kosaraju_run(g); }

bool SCC::connected(std::size_t v, std::size_t w) const {
  return id(v) == id(w);
}

std::size_t SCC::count() const { return _count; }

std::size_t SCC::id(std::size_t v) const {
  assert(v < _ids.size());
  return _ids[v];
}

std::set<std::size_t> SCC::component(std::size_t idx) const {
  assert(idx < _count);
  std::set<std::size_t> res;
  for (std::size_t v = 0; v < _ids.size(); ++v)
    if (_ids[v] == idx)
      res.insert(v);
  return res;
}

// Kosaraju algorithm:
// Based on a basic DFS algo
// Performs many DFS, from all yet unmarked devices, until all nodes are marked
// Every time a new DFS is done, all the reached nodes of the DFS form a new
// component The difference with a classic DFS is the order in which in the
// unmarked nodes are visited when starting a new DFS
//
// Without this special ordering, the DFS would put on the components all
// strongly connected vertices, but also some extra nodes that are not strongly
// connected. The ordering is the reverse post-order of the reversed graph (all
// edges inverted) This ordering enforces that when DFS from s reaches v, there
// is also a path in reversed graph from s to v (because v is after s in the
// reverse post-order)
//
// Complete proof p588 proposition H

void SCC::kosaraju_run(const Digraph &g) {
  std::vector<bool> marked(g.vcount(), false);
  _count = 0;
  DFSOrder order(g.reverse());
  for (auto v : order.reverse_post_order())
    if (!marked[v]) {
      kosaraju_dfs(g, marked, v);
      ++_count;
    }
}

void SCC::kosaraju_dfs(const Digraph &g, std::vector<bool> &marked,
                       std::size_t v) {
  marked[v] = true;
  _ids[v] = _count;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (!marked[*it])
      kosaraju_dfs(g, marked, *it);
}
