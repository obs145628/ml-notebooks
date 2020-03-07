#include "digraph.hh"
#include "directed-cycle.hh"
#include <catch2/catch.hpp>

TEST_CASE("Digraph cycle: none", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  DirectedCycle cy(g);
  REQUIRE(cy.has_cycle() == false);
  REQUIRE(cy.cycle() == std::vector<std::size_t>{});
}

TEST_CASE("Digraph cycle: one", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);
  g.add_edge(5, 0);

  DirectedCycle cy(g);
  REQUIRE(cy.has_cycle() == true);
  REQUIRE(cy.cycle() == std::vector<std::size_t>{2, 3, 5, 0});
}
