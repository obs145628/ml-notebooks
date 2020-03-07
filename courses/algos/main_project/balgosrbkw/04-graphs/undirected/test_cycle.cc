#include "base.hh"
#include "cycle.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph Cycle", "") {
  Graph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(4, 5);
  g.add_edge(1, 5);

  Cycle cy1(g);
  REQUIRE(cy1.has_cycle() == false);

  g.add_edge(3, 4);
  Cycle cy2(g);
  REQUIRE(cy2.has_cycle() == true);
}
