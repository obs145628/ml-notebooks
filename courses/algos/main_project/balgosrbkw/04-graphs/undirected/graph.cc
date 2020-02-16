#include "graph.hh"
#include <cassert>

Graph::Graph(std::size_t vcount) : _vcount(vcount), _ecount(0), _adj(_vcount) {}

void Graph::serialize(const Graph &g, std::ostream &os) {
  os << g.vcount() << " " << g.ecount() << "\n";
  for (std::size_t v = 0; v < g.vcount(); ++v)
    for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
      os << v << " " << *it << "\n";
}

Graph Graph::unserialize(std::istream &is) {
  std::size_t vcount;
  std::size_t ecount;
  is >> vcount >> ecount;
  Graph g(vcount);
  for (std::size_t i = 0; i < ecount; ++i) {
    std::size_t v;
    std::size_t w;
    is >> v >> w;
    g.add_edge(v, w);
  }
  return g;
}

void Graph::add_edge(std::size_t v, std::size_t w) {
  assert(v < _vcount);
  assert(w < _vcount);
  _adj[v].push_back(w);
  _adj[w].push_back(v);
  ++_ecount;
}

Graph::edge_iterator_t Graph::adj_begin(std::size_t v) const {
  assert(v < _vcount);
  return _adj[v].cbegin();
}

Graph::edge_iterator_t Graph::adj_end(std::size_t v) const {
  assert(v < _vcount);
  return _adj[v].cend();
}

void Graph::dump_tree(std::ostream &) {
  // @TODO
}
