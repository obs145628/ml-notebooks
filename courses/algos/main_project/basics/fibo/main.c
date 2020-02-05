#include "leio.h"

int_t fibo_rec(int_t x) {
  return x < 2 ? x : fibo_rec(x - 1) + fibo_rec(x - 2);
}

int_t fibo_iter(int_t x) {
  int_t a = 0;
  int_t b = 1;
  int_t cpy_b = 0;
  while (x > 0) {
    cpy_b = b;
    b = a + b;
    a = cpy_b;
    x = x - 1;
  }
  return a;
}

int main() {
  int_t i = 0;
  while (i < 10) {
    print_int(i);
    std_putc(58);
    std_putc(32);
    print_int(fibo_rec(i));
    std_putc(44);
    std_putc(32);
    printnl_int(fibo_iter(i));
    i = i + 1;
  }
}
