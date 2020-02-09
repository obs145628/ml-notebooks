#include "lealloc.h"
#include "leio.h"
#include "lerand.h"
#include "sort.h"

void print_arr(int_t arr, int_t len) {
  std_putc(91);

  int_t i = 0;
  while (i < len) {
    print_int(std_fmemget(arr + i));
    if (i + 1 < len) {
      std_putc(44);
      std_putc(32);
    }
    i = i + 1;
  }

  std_putc(93);
  std_putc(10);
}

void test1() {
  int_t arr = fm_alloc(7);
  std_fmemset(arr + 0, 12);
  std_fmemset(arr + 1, 8);
  std_fmemset(arr + 2, -6);
  std_fmemset(arr + 3, 25);
  std_fmemset(arr + 4, 18);
  std_fmemset(arr + 5, 12);
  std_fmemset(arr + 6, -2);
  sort(arr, 7);
  print_arr(arr, 7);
  fm_free(arr);
}

void test2() {
  int_t len = 100;
  int_t arr = fm_alloc(len);

  int_t i = 0;
  while (i < len) {
    std_fmemset(arr + i, -2 * i * i + 5 * i - 8);
    i += 1;
  }

  sort(arr, len);
  print_arr(arr, len);
  fm_free(arr);
}

void test3() {
  int_t len = 207;
  int_t arr = fm_alloc(len);

  int_t i = 0;
  while (i < len) {
    std_fmemset(arr + i, 1000 + 12 * i);
    i += 1;
  }

  sort(arr, len);
  print_arr(arr, len);
  fm_free(arr);
}

void test4() {
  int_t len = 178;
  int_t arr = fm_alloc(len);

  int_t i = 0;
  while (i < len) {
    std_fmemset(arr + i, 1000 - 12 * i);
    i += 1;
  }

  sort(arr, len);
  print_arr(arr, len);
  fm_free(arr);
}

void test5() {
  int_t len = 675;
  int_t arr = fm_alloc(len);
  int_t rng = rng_new(78);

  int_t i = 0;
  while (i < len) {
    std_fmemset(arr + i, rng_next(rng));
    i = i + 1;
  }

  sort(arr, len);
  print_arr(arr, len);
  rng_free(rng);
  fm_free(arr);
}

void test6() {
  int_t arr = fm_alloc(9);
  std_fmemset(arr + 0, 2);
  std_fmemset(arr + 1, 4);
  std_fmemset(arr + 2, 1);
  std_fmemset(arr + 3, 1);
  std_fmemset(arr + 4, 4);
  std_fmemset(arr + 5, 2);
  std_fmemset(arr + 6, 3);
  std_fmemset(arr + 7, 4);
  std_fmemset(arr + 8, 1);
  sort(arr, 9);
  print_arr(arr, 9);
  fm_free(arr);
}

void test7() {
  int_t arr = fm_alloc(250);

  int_t i = 0;
  while (i < 25) {
    int_t j = 0;
    while (j < 10) {
      std_fmemset(arr + 10 * i + j, 200 + 12 * i);
      j = j + 1;
    }
    i = i + 1;
  }

  sort(arr, 250);
  print_arr(arr, 250);
  fm_free(arr);
}

void test8() {
  int_t arr = fm_alloc(250);

  int_t i = 0;
  while (i < 25) {
    int_t j = 0;
    while (j < 10) {
      std_fmemset(arr + 10 * i + j, 200 - 12 * i);
      j = j + 1;
    }
    i = i + 1;
  }

  sort(arr, 250);
  print_arr(arr, 250);
  fm_free(arr);
}

void test9() {
  int_t len = 113;
  int_t arr = fm_alloc(len);

  int_t i = 0;
  while (i < len) {
    std_fmemset(arr + i, (-2 * i * i + 5 * i + 43) % 10);
    i = i + 1;
  }

  sort(arr, len);
  print_arr(arr, len);
  fm_free(arr);
}

void test10() {
  int_t len = 327;
  int_t arr = fm_alloc(len);
  int_t rng = rng_new(54);

  int_t i = 0;
  while (i < len) {
    std_fmemset(arr + i, rng_next(rng) % 13);
    i = i + 1;
  }

  sort(arr, len);
  print_arr(arr, len);
  rng_free(rng);
  fm_free(arr);
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
}
