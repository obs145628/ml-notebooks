#include "topological.hh"
#include "dfs-order.hh"
#include "directed-cycle.hh"

Topological::Topological(const Digraph &g) {
  DirectedCycle cy(g);
  if (!cy.has_cycle()) {
    DFSOrder order(g);
    _order = order.reverse_post_order();
  }
}
