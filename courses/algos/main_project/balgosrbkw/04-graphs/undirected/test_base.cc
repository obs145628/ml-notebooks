#include "base.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph base degree", "") {
  Graph g(4);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  REQUIRE(degree(g, 0) == 2);
  REQUIRE(degree(g, 1) == 1);
  REQUIRE(degree(g, 2) == 3);
  REQUIRE(degree(g, 3) == 2);

  REQUIRE(max_degree(g) == 3);
  REQUIRE(avg_degree(g) == (2 + 1 + 3 + 2) / 4);
}

TEST_CASE("Graph count self loop none", "") {
  Graph g(4);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(2, 3);

  REQUIRE(count_self_loops(g) == 0);
}

TEST_CASE("Graph count self loop", "") {
  Graph g(4);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 1);
  g.add_edge(2, 2);

  REQUIRE(count_self_loops(g) == 2);
}
