#include "digraph.hh"
#include "topological.hh"
#include <catch2/catch.hpp>

static void check_topo(const Digraph &g,
                       const std::vector<std::size_t> &order) {

  auto grev = g.reverse();
  std::vector<bool> done(g.vcount(), false);

  for (auto v : order) {
    for (auto it = grev.adj_begin(v); it != grev.adj_end(v); ++it)
      REQUIRE(done[*it] == true);
    done[v] = true;
  }

  for (auto st : done)
    REQUIRE(st == true);
}

TEST_CASE("Topological DAG", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);

  Topological ts(g);
  REQUIRE(ts.is_dag() == true);
  check_topo(g, ts.order());
}

TEST_CASE("Topological no DAG", "") {
  Digraph g(6);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 3);
  g.add_edge(2, 3);
  g.add_edge(3, 5);
  g.add_edge(4, 5);
  g.add_edge(5, 0);

  Topological ts(g);
  REQUIRE(ts.is_dag() == false);
}
