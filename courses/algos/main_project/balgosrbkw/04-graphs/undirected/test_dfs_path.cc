#include "base.hh"
#include "dfs-path.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph DFS path", "") {
  Graph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(4, 5);

  DFSPath dfs(g, 0);
  REQUIRE(dfs.connected(0) == true);
  REQUIRE(dfs.connected(1) == true);
  REQUIRE(dfs.connected(2) == true);
  REQUIRE(dfs.connected(3) == true);
  REQUIRE(dfs.connected(4) == false);
  REQUIRE(dfs.connected(5) == false);

  REQUIRE(dfs.path_to(0) == std::vector<std::size_t>{0});
  REQUIRE(dfs.path_to(1) == std::vector<std::size_t>{0, 2, 3, 1});
  REQUIRE(dfs.path_to(2) == std::vector<std::size_t>{0, 2});
  REQUIRE(dfs.path_to(3) == std::vector<std::size_t>{0, 2, 3});
}
