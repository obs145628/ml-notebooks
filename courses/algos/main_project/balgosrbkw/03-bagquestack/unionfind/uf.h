#ifndef UF_H_
#define UF_H_

#include "lestd.h"

// Unionfind is a datastructure to store objects into an emsemble of sets
// It has fast operations to find which set an object belongs to
// and to combine 2 sets into one

// Create unionfind structure with n sites (0 to n - 1)
int_t uf_new(int_t n);

// Free all memory of the unionfind
void uf_free(int_t uf);

// Connect p and q in one component
void uf_union(int_t uf, int_t p, int_t q);

// Returns the component identifier (between 0 and n - 1) for p
int_t uf_find(int_t uf, int_t p);

// Returns true if p and q are in the same component
int_t uf_connected(int_t uf, int_t p, int_t q);

// Returns the number of distinct components
int_t uf_count(int_t uf);

#endif //! UF_H_
