#ifndef LEALLOC_H_
#define LEALLOC_H_

#include "lestd.h"

// Allocates memory on flat memory, and returns address
int_t fm_alloc(int_t len);

// Free memory allocated with fm_alloc
void fm_free(int_t addr);

#endif //! LEALLOC_V0_H_
