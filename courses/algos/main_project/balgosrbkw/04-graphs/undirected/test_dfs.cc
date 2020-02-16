#include "base.hh"
#include "dfs.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph DFS", "") {
  Graph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(4, 5);

  DFS dfs(g, 0);
  REQUIRE(dfs.count() == 4);
  REQUIRE(dfs.connected(0) == true);
  REQUIRE(dfs.connected(1) == true);
  REQUIRE(dfs.connected(2) == true);
  REQUIRE(dfs.connected(3) == true);
  REQUIRE(dfs.connected(4) == false);
  REQUIRE(dfs.connected(5) == false);
}
