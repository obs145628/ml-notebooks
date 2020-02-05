#include "leio.h"

int main() {
  int_t val = 0;
  while (val != -1) {
    val = std_getc();
    if (val != -1)
      std_putc(val);
  }
}
