#ifndef LERAND_H_
#define LERAND_H_

#include "lestd.h"

// basic random generator based on rand (2) man page

// Create a new random generator with specific seed
int_t rng_new(int_t seed);

// Free all memory allocated for the random generator
void rng_free(int_t rng);

// Generate next i32 number
int_t rng_next(int_t rng);

#endif //! LERAND_H_
