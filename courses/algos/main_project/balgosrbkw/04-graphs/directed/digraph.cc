#include "digraph.hh"
#include <cassert>

Digraph::Digraph(std::size_t vcount)
    : _vcount(vcount), _ecount(0), _adj(_vcount) {}

void Digraph::serialize(const Digraph &g, std::ostream &os) {
  os << g.vcount() << " " << g.ecount() << "\n";
  for (std::size_t v = 0; v < g.vcount(); ++v)
    for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
      os << v << " " << *it << "\n";
}

Digraph Digraph::unserialize(std::istream &is) {
  std::size_t vcount;
  std::size_t ecount;
  is >> vcount >> ecount;
  Digraph g(vcount);
  for (std::size_t i = 0; i < ecount; ++i) {
    std::size_t v;
    std::size_t w;
    is >> v >> w;
    g.add_edge(v, w);
  }
  return g;
}

void Digraph::add_edge(std::size_t v, std::size_t w) {
  assert(v < _vcount);
  assert(w < _vcount);
  _adj[v].push_back(w);
  ++_ecount;
}

Digraph::edge_iterator_t Digraph::adj_begin(std::size_t v) const {
  assert(v < _vcount);
  return _adj[v].cbegin();
}

Digraph::edge_iterator_t Digraph::adj_end(std::size_t v) const {
  assert(v < _vcount);
  return _adj[v].cend();
}

Digraph Digraph::reverse() {
  Digraph res(vcount());
  for (std::size_t v = 0; v < vcount(); ++v)
    for (auto it = adj_begin(v); it != adj_end(v); ++it)
      res.add_edge(*it, v);
  return res;
}

void Digraph::dump_tree(std::ostream &) {
  // @TODO
}
