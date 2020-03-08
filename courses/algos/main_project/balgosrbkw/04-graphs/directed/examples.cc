#include "examples.hh"
#include <sstream>

Digraph load_graph(const char *data) {
  std::istringstream is(data);
  return Digraph::unserialize(is);
}

const char *tiny_dg_data = "13 "
                           "22 "
                           "4  2 "
                           "2  3 "
                           "3  2 "
                           "6  0 "
                           "0  1 "
                           "2  0 "
                           "11 12 "
                           "12  9 "
                           "9 10 "
                           "9 11 "
                           "8  9 "
                           "10 12 "
                           "11  4 "
                           "4  3 "
                           "3  5 "
                           "7  8 "
                           "8  7 "
                           "5  4 "
                           "0  5 "
                           "6  4 "
                           "6  9 "
                           "7  6 ";
