#include "leio.h"
#include "uf.h"

void test1() {
  int_t g = uf_new(3);
  printnl_int(uf_find(g, 0));
  printnl_int(uf_find(g, 1));
  printnl_int(uf_find(g, 2));
  printnl_int(uf_connected(g, 0, 1));
  printnl_int(uf_connected(g, 0, 2));
  printnl_int(uf_connected(g, 1, 2));
  printnl_int(uf_count(g));
  uf_free(g);
}

void test2() {
  int_t g = uf_new(3);
  uf_union(g, 0, 1);
  printnl_int(uf_find(g, 0));
  printnl_int(uf_find(g, 1));
  printnl_int(uf_find(g, 2));
  printnl_int(uf_connected(g, 0, 1));
  printnl_int(uf_connected(g, 0, 2));
  printnl_int(uf_connected(g, 1, 2));
  printnl_int(uf_count(g));
  uf_free(g);
}

void test3() {
  int_t g = uf_new(3);
  uf_union(g, 0, 1);
  uf_union(g, 1, 2);
  printnl_int(uf_find(g, 0));
  printnl_int(uf_find(g, 1));
  printnl_int(uf_find(g, 2));
  printnl_int(uf_connected(g, 0, 1));
  printnl_int(uf_connected(g, 0, 2));
  printnl_int(uf_connected(g, 1, 2));
  printnl_int(uf_count(g));
  uf_free(g);
}

void test4() {
  int_t g = uf_new(12);
  uf_union(g, 0, 2);
  uf_union(g, 10, 5);
  uf_union(g, 4, 2);
  uf_union(g, 8, 9);
  uf_union(g, 0, 7);
  uf_union(g, 1, 7);
  uf_union(g, 3, 11);
  uf_union(g, 5, 8);
  uf_union(g, 4, 7);

  int_t i = 0;
  while (i < 12) {
    printnl_int(uf_find(g, i));
    i += 1;
  }
  printnl_int(uf_count(g));
  uf_free(g);
}

int main() {
  test1();
  test2();
  test3();
  test4();
}
