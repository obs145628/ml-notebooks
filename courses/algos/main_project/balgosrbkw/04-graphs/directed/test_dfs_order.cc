#include "dfs-order.hh"
#include "digraph.hh"
#include <catch2/catch.hpp>

TEST_CASE("DFS Order", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  DFSOrder order(g);
  REQUIRE(order.pre_order() == std::vector<std::size_t>{0, 2, 3, 5, 1, 4});
  REQUIRE(order.post_order() == std::vector<std::size_t>{5, 3, 2, 0, 1, 4});
  REQUIRE(order.reverse_post_order() ==
          std::vector<std::size_t>{4, 1, 0, 2, 3, 5});
}
