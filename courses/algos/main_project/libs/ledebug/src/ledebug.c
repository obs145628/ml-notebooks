#include "ledebug.h"
#include "lestd.h"

#include <assert.h>

void panic() {
  std_putc(80);
  std_putc(65);
  std_putc(78);
  std_putc(73);
  std_putc(67);
  std_putc(33);
  std_putc(10);
  assert(0);
  std_exit(1);
}

// Panic if `cond` is false
void panic_ifn(int cond) {
  if (!cond)
    panic();
}
