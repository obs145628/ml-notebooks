#include "digraph.hh"
#include "directed-dfs.hh"
#include <catch2/catch.hpp>

TEST_CASE("Digraph DFS", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  DirectedDFS dfs(g, 0);
  REQUIRE(dfs.reachable(0) == true);
  REQUIRE(dfs.reachable(1) == false);
  REQUIRE(dfs.reachable(2) == true);
  REQUIRE(dfs.reachable(3) == true);
  REQUIRE(dfs.reachable(4) == false);
  REQUIRE(dfs.reachable(5) == true);
}
