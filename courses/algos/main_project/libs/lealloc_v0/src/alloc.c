#include "lealloc.h"

// This implementation keeps allocating but never free

int_t fm_alloc(int_t len) {
  int_t top = std_fmemget(0);
  top = top ? top : 1;
  std_fmemset(0, top + len);
  return top;
}

void fm_free(int_t addr) { (void)addr; }
