#ifndef HEAP_H_
#define HEAP_H_

#include "lestd.h"

// Min Priority queue
// Push and pop items like a queue.
// But removed item is the smallest one

// Create a new empty heap
int_t heap_new();

// Free all memory of the heap
void heap_free(int_t h);

// Add val to the heap
void heap_push(int_t h, int_t val);

// Remove and return smallest item of the heap
int_t heap_pop(int_t h);

// Return smallest items of the heap
int_t heap_min(int_t h);

// Number of items in the heap
int_t heap_size(int_t h);

#endif //! HEAP_H_
