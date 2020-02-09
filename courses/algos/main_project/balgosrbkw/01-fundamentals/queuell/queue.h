#ifndef QUEUE_H_
#define QUEUE_H_

#include "lestd.h"

// Create a new empty queue
int_t queue_new();

// Free all memory allocated to a queue
void queue_free(int_t q);

// Returns the number of items in the queue
int_t queue_size(int_t q);

// Push one item to the back of the queue
void queue_push(int_t q, int_t val);

// Pop and returns one item from the front of the queue
int_t queue_pop(int_t q);

#endif //! QUEUE_H_
