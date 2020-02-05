#include "leio.h"

// ascii codes:
// '\n': 10
// '-' : 45
// '0' : 48

static void print_rec(int_t x) {
  if (!(x == 0)) {
    print_rec(x / 10);
    std_putc(48 + (x % 10));
  }
}

void print_int(int_t x) {
  if (x < 0) {
    std_putc(45);
    print_rec(-x);
  }

  else if (x == 0) {
    std_putc(48);
  } else {
    print_rec(x);
  }
}

void printnl() { std_putc(10); }

void printnl_int(int_t x) {
  print_int(x);
  printnl();
}
