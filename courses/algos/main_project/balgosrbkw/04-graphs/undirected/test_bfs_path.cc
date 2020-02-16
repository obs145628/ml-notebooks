#include "base.hh"
#include "bfs-path.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph BFS path", "") {
  Graph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(4, 5);

  BFSPath bfs(g, 0);
  REQUIRE(bfs.connected(0) == true);
  REQUIRE(bfs.connected(1) == true);
  REQUIRE(bfs.connected(2) == true);
  REQUIRE(bfs.connected(3) == true);
  REQUIRE(bfs.connected(4) == false);
  REQUIRE(bfs.connected(5) == false);

  REQUIRE(bfs.path_to(0) == std::vector<std::size_t>{0});
  REQUIRE(bfs.path_to(1) == std::vector<std::size_t>{0, 3, 1});
  REQUIRE(bfs.path_to(2) == std::vector<std::size_t>{0, 2});
  REQUIRE(bfs.path_to(3) == std::vector<std::size_t>{0, 3});
}
