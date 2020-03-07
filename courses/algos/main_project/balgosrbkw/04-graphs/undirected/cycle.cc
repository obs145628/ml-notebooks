#include "cycle.hh"

Cycle::Cycle(const Graph &g) : _marked(g.vcount(), false), _cycle(false) {
  for (std::size_t v = 0; v < g.vcount(); ++v)
    if (!_marked[v])
      dfs(g, v, v);
}

// v is the vertex we want to visit, and u it's predecessor
// Use a boolean array to keep track of visited vertices
// Before visiting a new vertex, mark it as visited.
// For all unvisited adjacent vertices, do a recursive call
// If already visted, and that's not it's predecessor, there is a cycle
// Undirected graph, we have a->b, b->a, so we need to check if that's not just
// the other link
void Cycle::dfs(const Graph &g, std::size_t v, std::size_t u) {
  _marked[v] = true;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (!_marked[*it])
      dfs(g, *it, v);
    else if (*it != u)
      _cycle = true;
}
