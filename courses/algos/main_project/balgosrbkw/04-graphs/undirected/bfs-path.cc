#include "bfs-path.hh"
#include <cassert>
#include <queue>

// The implementation use a vector of predecessors
// _edges[w] = v means the bfs search visited w from v
// _edges[v] = VERTEX_NONE when v is not connected to the graph, or is the src
// vertex
// The BFS order is done by using a queue (LIFO), instead of a stack for DFS.

#include <iostream>

namespace {
constexpr std::size_t VERTEX_NONE = static_cast<std::size_t>(-1);
}

BFSPath::BFSPath(const Graph &g, std::size_t s)
    : _src(s), _edges(g.vcount(), VERTEX_NONE) {
  bfs(g, _src);
}

bool BFSPath::connected(std::size_t v) const {
  return v == _src || _edges[v] != VERTEX_NONE;
}

// Start from the end and go through the predecessors list until reaching the
// start vertex The path is constructed in reverse order
std::vector<std::size_t> BFSPath::path_to(std::size_t v) const {
  assert(connected(v));
  std::vector<std::size_t> res;
  for (; v != _src; v = _edges[v])
    res.push_back(v);
  res.push_back(_src);
  return std::vector<std::size_t>(res.rbegin(), res.rend());
}

// Visit all connected vertices, starting at the source
// Start by pushing `src` vertex in the queue
// Iteratively remove vertex from queue
// For all unvisited adjacent vertices, store the reference to its predecessor
// on edge
// Then push it to the queue
void BFSPath::bfs(const Graph &g, std::size_t src) {

  std::queue<std::size_t> q;
  q.push(src);

  while (!q.empty()) {
    auto v = q.front();
    q.pop();
  
    for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
      if (!connected(*it)) {
	_edges[*it] = v;
	q.push(*it);
      }
  }
}
