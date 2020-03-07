#include "base.hh"
#include "cc.hh"
#include "graph.hh"
#include <catch2/catch.hpp>

TEST_CASE("Graph CC", "") {
  Graph g(7);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(4, 5);

  CC cc(g);
  REQUIRE(cc.count() == 3);
  REQUIRE(cc.connected(0, 0) == true);
  REQUIRE(cc.connected(0, 1) == true);
  REQUIRE(cc.connected(0, 2) == true);
  REQUIRE(cc.connected(0, 3) == true);
  REQUIRE(cc.connected(0, 4) == false);
  REQUIRE(cc.connected(0, 5) == false);
  REQUIRE(cc.connected(0, 6) == false);
  REQUIRE(cc.connected(4, 0) == false);
  REQUIRE(cc.connected(4, 1) == false);
  REQUIRE(cc.connected(4, 2) == false);
  REQUIRE(cc.connected(4, 3) == false);
  REQUIRE(cc.connected(4, 4) == true);
  REQUIRE(cc.connected(4, 5) == true);
  REQUIRE(cc.connected(4, 6) == false);
  REQUIRE(cc.connected(6, 0) == false);
  REQUIRE(cc.connected(6, 1) == false);
  REQUIRE(cc.connected(6, 2) == false);
  REQUIRE(cc.connected(6, 3) == false);
  REQUIRE(cc.connected(6, 4) == false);
  REQUIRE(cc.connected(6, 5) == false);
  REQUIRE(cc.connected(6, 6) == true);
}
