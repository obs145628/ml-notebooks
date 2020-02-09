#include "leio.h"
#include "queue.h"

void test1() {
  int_t q = queue_new();
  printnl_int(queue_size(q));

  queue_push(q, 10);
  queue_push(q, 18);
  queue_push(q, 23);
  queue_push(q, 45);

  while (queue_size(q)) {
    printnl_int(queue_pop(q));
  }

  queue_free(q);
}

void test2() {
  int_t q = queue_new();

  queue_push(q, 18);
  queue_push(q, 25);
  queue_push(q, 16);
  queue_push(q, 56);
  printnl_int(queue_pop(q));
  printnl_int(queue_pop(q));

  queue_push(q, 12);
  printnl_int(queue_pop(q));
  printnl_int(queue_pop(q));

  queue_push(q, 24);
  queue_push(q, 8);
  queue_push(q, -34);
  printnl_int(queue_pop(q));
  printnl_int(queue_pop(q));

  queue_free(q);
}

void test3() {
  int_t q = queue_new();
  int_t i = 0;
  while (i < 1000) {
    queue_push(q, 3 * i * i + 2 * i - 134);
    i += 1;
  }

  while (queue_size(q)) {
    printnl_int(queue_pop(q));
  }

  queue_free(q);
}

int main() {
  test1();
  test2();
  test3();
}
