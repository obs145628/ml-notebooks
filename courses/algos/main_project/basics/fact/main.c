#include "leio.h"

int_t fact_rec(int_t x) { return x < 2 ? 1 : x * fact_rec(x - 1); }

int_t fact_iter(int_t x) {
  int_t res = 1;
  while (x > 1) {
    res = res * x;
    x = x - 1;
  }
  return res;
}

int main() {
  int_t i = 0;
  while (i < 10) {
    print_int(i);
    std_putc(58);
    std_putc(32);
    print_int(fact_rec(i));
    std_putc(44);
    std_putc(32);
    printnl_int(fact_iter(i));
    i = i + 1;
  }
}
