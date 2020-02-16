#include "graph.hh"
#include <algorithm>
#include <catch2/catch.hpp>
#include <sstream>
#include <vector>

TEST_CASE("Graph::add_edge", "") {
  Graph g(6);
  g.add_edge(3, 5);
  g.add_edge(3, 4);
  g.add_edge(2, 3);
  REQUIRE(g.vcount() == 6);
  REQUIRE(g.ecount() == 3);
}

static std::vector<std::size_t> build_adj_set(const Graph &g, std::size_t v) {
  std::vector<std::size_t> res;
  for (auto it = g.adj_begin(v); it != g.adj_end(v); ++it)
    res.push_back(*it);
  std::sort(res.begin(), res.end());
  return res;
}

TEST_CASE("Graph::adj", "") {
  Graph g(5);
  g.add_edge(3, 1);
  g.add_edge(3, 2);
  g.add_edge(1, 4);

  REQUIRE(build_adj_set(g, 0) == std::vector<std::size_t>{});
  REQUIRE(build_adj_set(g, 1) == std::vector<std::size_t>{3, 4});
  REQUIRE(build_adj_set(g, 2) == std::vector<std::size_t>{3});
  REQUIRE(build_adj_set(g, 3) == std::vector<std::size_t>{1, 2});
  REQUIRE(build_adj_set(g, 4) == std::vector<std::size_t>{1});
}

TEST_CASE("Graph serialiazation", "") {
  Graph g1(6);
  g1.add_edge(3, 1);
  g1.add_edge(3, 2);
  g1.add_edge(1, 4);
  g1.add_edge(5, 1);
  g1.add_edge(4, 0);
  g1.add_edge(0, 5);

  std::ostringstream os;
  Graph::serialize(g1, os);
  std::istringstream is(os.str());
  auto g2 = Graph::unserialize(is);

  REQUIRE(g1.vcount() == g2.vcount());
  REQUIRE(g1.ecount() == g2.ecount());
  for (std::size_t i = 0; i < g1.vcount(); ++i)
    REQUIRE(build_adj_set(g1, i) == build_adj_set(g2, i));
}
