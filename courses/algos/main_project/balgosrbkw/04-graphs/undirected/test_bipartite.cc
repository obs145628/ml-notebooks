#include "base.hh"
#include "bipartite.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph Bipartite", "") {
  Graph g(5);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(0, 4);

  Bipartite bp1(g);
  REQUIRE(bp1.is_bipartite() == true);

  g.add_edge(0, 3);
  Bipartite bp2(g);
  REQUIRE(bp2.is_bipartite() == false);
}
