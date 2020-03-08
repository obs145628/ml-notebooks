#pragma once

#include "digraph.hh"

Digraph load_graph(const char *data);

/// Basic example with 13 vertices and 22 edges
extern const char *tiny_dg_data;
