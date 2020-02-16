#include "dfs.hh"

DFS::DFS(const Graph &g, std::size_t s)
    : _count(0), _marked(g.vcount(), false) {
  dfs(g, s);
}

// Use a boolean array to keep track of visited vertices
// Before visiting a new vertex, mark it as visited.
// For all unvisited adjacent vertices, do a recursive call
void DFS::dfs(const Graph &g, std::size_t v) {
  _marked[v] = true;
  ++_count;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    if (!_marked[*it])
      dfs(g, *it);
}
