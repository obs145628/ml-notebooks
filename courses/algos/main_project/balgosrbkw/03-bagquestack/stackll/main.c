#include "leio.h"
#include "stack.h"

void test1() {
  int_t s = stack_new();
  printnl_int(stack_size(s));
  stack_push(s, 16);
  stack_push(s, 14);
  stack_push(s, 8);
  stack_push(s, 7);
  printnl_int(stack_size(s));

  while (stack_size(s)) {
    printnl_int(stack_pop(s));
  }
  stack_free(s);
}

void test2() {
  int_t s = stack_new();
  int_t i = 0;
  while (i < 1000) {
    stack_push(s, 2 * i * i - 12 * i + 6);
    i = i + 1;
  }

  while (stack_size(s)) {
    printnl_int(stack_pop(s));
  }
  stack_free(s);
}

int main() {
  test1();
  test2();
}
