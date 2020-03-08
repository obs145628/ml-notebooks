#include "digraph.hh"
#include "transitive-closure.hh"
#include <catch2/catch.hpp>

TEST_CASE("Transitive Closure", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);
  TransitiveClosure tc(g);

  REQUIRE(tc.reachable(0, 0) == true);
  REQUIRE(tc.reachable(0, 1) == false);
  REQUIRE(tc.reachable(0, 2) == true);
  REQUIRE(tc.reachable(0, 3) == true);
  REQUIRE(tc.reachable(0, 4) == false);
  REQUIRE(tc.reachable(0, 5) == true);

  REQUIRE(tc.reachable(1, 0) == false);
  REQUIRE(tc.reachable(1, 1) == true);
  REQUIRE(tc.reachable(1, 2) == false);
  REQUIRE(tc.reachable(1, 3) == true);
  REQUIRE(tc.reachable(1, 4) == false);
  REQUIRE(tc.reachable(1, 5) == true);

  REQUIRE(tc.reachable(2, 0) == false);
  REQUIRE(tc.reachable(2, 1) == false);
  REQUIRE(tc.reachable(2, 2) == true);
  REQUIRE(tc.reachable(2, 3) == true);
  REQUIRE(tc.reachable(2, 4) == false);
  REQUIRE(tc.reachable(2, 5) == true);

  REQUIRE(tc.reachable(3, 0) == false);
  REQUIRE(tc.reachable(3, 1) == false);
  REQUIRE(tc.reachable(3, 2) == false);
  REQUIRE(tc.reachable(3, 3) == true);
  REQUIRE(tc.reachable(3, 4) == false);
  REQUIRE(tc.reachable(3, 5) == true);

  REQUIRE(tc.reachable(4, 0) == false);
  REQUIRE(tc.reachable(4, 1) == false);
  REQUIRE(tc.reachable(4, 2) == false);
  REQUIRE(tc.reachable(4, 3) == false);
  REQUIRE(tc.reachable(4, 4) == true);
  REQUIRE(tc.reachable(4, 5) == true);

  REQUIRE(tc.reachable(5, 0) == false);
  REQUIRE(tc.reachable(5, 1) == false);
  REQUIRE(tc.reachable(5, 2) == false);
  REQUIRE(tc.reachable(5, 3) == false);
  REQUIRE(tc.reachable(5, 4) == false);
  REQUIRE(tc.reachable(5, 5) == true);
}
