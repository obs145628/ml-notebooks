#include "digraph.hh"
#include "examples.hh"
#include "scc.hh"
#include <catch2/catch.hpp>

TEST_CASE("SCC", "") {
  auto g = load_graph(tiny_dg_data);

  SCC scc(g);
  REQUIRE(scc.count() == 5);
  REQUIRE(scc.component(scc.id(0)) == std::set<std::size_t>{0, 2, 3, 4, 5});
  REQUIRE(scc.component(scc.id(1)) == std::set<std::size_t>{1});
  REQUIRE(scc.component(scc.id(6)) == std::set<std::size_t>{6});
  REQUIRE(scc.component(scc.id(7)) == std::set<std::size_t>{7, 8});
  REQUIRE(scc.component(scc.id(9)) == std::set<std::size_t>{9, 10, 11, 12});
}
