#pragma once

// Basic algorithms to manipulate graphs

#include "graph.hh"

/// Returns the degree of vertex `v` in `g`
/// The degree of a vertex is a number of vertices connected to this vertex
std::size_t degree(const Graph &g, std::size_t v);

/// Returns the maximum degree among all vertices of `g`
std::size_t max_degree(const Graph &g);

/// Return the average dregree among all vertices of `g`
std::size_t avg_degree(const Graph &g);

/// Returns the number of self loops in `g`
/// A self loop if formed by an edge connecting a vertex to itself
std::size_t count_self_loops(const Graph &g);
