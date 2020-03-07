#include "directed-cycle.hh"
#include <cassert>

namespace {
constexpr std::size_t NO_PRED = static_cast<std::size_t>(-1);
}

DirectedCycle::DirectedCycle(const Digraph &g)
    : _pred(g.vcount(), NO_PRED), _stack(g.vcount(), false) {
  for (std::size_t v = 0; v < g.vcount(); ++v)
    if (_pred[v] == NO_PRED)
      dfs(g, v);
}

bool DirectedCycle::has_cycle() const { return !cycle().empty(); }

const std::vector<std::size_t> &DirectedCycle::cycle() const { return _cycle; }

/// Recursive call to visit all reachable vertices
/// pred: vector of predecessors, pred[w] = v if there is a directed edge v -> w
/// -1 for source node, or if node not visited yet
// stack: boolean of visited nodes in the same pass, used to detect cycles
// cycle: detected cycle in correct order, only filled at once when a cycle is
// detected, using preds vector
//
// Simple DFS, that fill preds vectors and make sure every node is visited only
// once Extra attribute stack to detect cycles: currently visiting path as a
// stack: when visiting a new node, push to the stack (set to true), then pop
// (set to false) after visiting finished. If trying to visit a node already
// visited, its value is true
void DirectedCycle::dfs(const Digraph &g, std::size_t v) {
  _stack[v] = true;

  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it) {
    std::size_t w = *it;
    if (has_cycle()) // cycle already detected, stop
      return;
    else if (_stack[w]) // cycle detected
      build_cycle(v, w);
    else if (_pred[w] == NO_PRED) // visit new node
    {
      _pred[w] = v;
      dfs(g, w);
    }
  }

  _stack[v] = false;
}

// When going to v -> w, and cycle w to w found, use this function to build the
// cycle
void DirectedCycle::build_cycle(std::size_t v, std::size_t w) {
  // Create reversed path from w back to w (using preds list)
  // then return reversed vector
  std::vector<std::size_t> res;
  res.push_back(w);

  for (std::size_t x = v; x != w; x = _pred[x]) {
    assert(x != NO_PRED);
    res.push_back(x);
  }
  _cycle = std::vector<std::size_t>(res.rbegin(), res.rend());
}
