#include "lealloc.h"
#include "leio.h"
#include "table.h"

int_t cmp(int_t arr, int_t i, int_t j) {
  return std_fmemget(arr + i) - std_fmemget(arr + j);
}
void swap(int_t arr, int_t i, int_t j) {
  int_t vi = std_fmemget(arr + i);
  std_fmemset(arr + i, std_fmemget(arr + j));
  std_fmemset(arr + j, vi);
}
void sort(int_t arr, int_t len) {
  int_t i = 0;
  while (i < len) {

    int_t j = i;
    while (j > 0 && cmp(arr, j, j - 1) < 0) {
      swap(arr, j, j - 1);
      j = j - 1;
    }

    i = i + 1;
  }
}
void print_arr(int_t arr, int_t len) {
  sort(arr, len);
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

void sort2(int_t arr1, int arr2, int_t len) {
  int_t i = 0;
  while (i < len) {

    int_t j = i;
    while (j > 0 && cmp(arr1, j, j - 1) < 0) {
      swap(arr1, j, j - 1);
      swap(arr2, j, j - 1);
      j = j - 1;
    }

    i = i + 1;
  }
}
void print_arr2(int_t arr1, int arr2, int_t len) {
  sort2(arr1, arr2, len);
  std_putc(91);

  int_t i = 0;
  while (i < len) {
    std_putc(40);
    print_int(std_fmemget(arr1 + i));
    std_putc(59);
    print_int(std_fmemget(arr2 + i));
    std_putc(41);
    if (i + 1 < len) {
      std_putc(44);
      std_putc(32);
    }
    i = i + 1;
  }

  std_putc(93);
  std_putc(10);
}

void print_keys(int_t st) {
  int_t len = table_size(st);
  int_t keys = fm_alloc(len);
  int_t it = table_it_new(st);
  int_t i = 0;

  while (i < len) {
    std_fmemset(keys + i, table_it_get_key(it));
    table_it_next(it);
    i = i + 1;
  }

  print_arr(keys, len);
  table_it_free(it);
  fm_free(keys);
}

void print_vals(int_t st) {
  int_t len = table_size(st);
  int_t vals = fm_alloc(len);
  int_t it = table_it_new(st);
  int_t i = 0;

  while (i < len) {
    std_fmemset(vals + i, table_it_get_val(it));
    table_it_next(it);
    i = i + 1;
  }

  print_arr(vals, len);
  table_it_free(it);
  fm_free(vals);
}

void print_table(int_t st) {
  int_t len = table_size(st);
  int_t keys = fm_alloc(len);
  int_t vals = fm_alloc(len);
  int_t it = table_it_new(st);
  int_t i = 0;

  while (i < len) {
    std_fmemset(keys + i, table_it_get_key(it));
    std_fmemset(vals + i, table_it_get_val(it));
    table_it_next(it);
    i = i + 1;
  }

  print_arr2(keys, vals, len);
  table_it_free(it);
  fm_free(keys);
  fm_free(vals);
}

void test1() {
  int_t st = table_new();
  print_keys(st);
  print_vals(st);
  print_table(st);
  table_free(st);
}

void test2() {
  int_t st = table_new();
  printnl_int(table_put(st, 3, 78));
  printnl_int(table_put(st, 6, 4));
  printnl_int(table_put(st, 2, 45));
  printnl_int(table_put(st, 1, 27));
  printnl_int(table_put(st, 2, 37));
  printnl_int(table_put(st, 8, 44));

  int_t i = 0;
  while (i < 10) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }

  print_keys(st);
  print_vals(st);
  print_table(st);
  table_free(st);
}

void test3() {
  int_t st = table_new();
  int_t i = 0;
  while (i < 20) {
    printnl_int(table_put(st, i, i * i));
    i = i + 1;
  }

  i = 0;
  while (i < 20) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }
  print_table(st);

  i = 0;
  while (i < 20) {
    printnl_int(table_delete(st, i));
    i = i + 1;
  }

  print_table(st);
  table_free(st);
}

void test4() {
  int_t st = table_new();
  int_t i = -40;
  while (i < 40) {
    printnl_int(table_put(st, i, i * i));
    i = i + 1;
  }

  i = -40;
  while (i < 40) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }
  print_table(st);

  i = -12;
  while (i < 4) {
    printnl_int(table_delete(st, i));
    i = i + 1;
  }

  i = -40;
  while (i < 40) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }
  print_table(st);

  i = 4;
  while (i < 28) {
    printnl_int(table_put(st, i, 4 * i * i - 5));
    i = i + 1;
  }

  i = -40;
  while (i < 40) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }
  print_table(st);

  i = -37;
  while (i < 8) {
    printnl_int(table_delete(st, i));
    i = i + 1;
  }

  i = -40;
  while (i < 40) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }
  print_table(st);

  i = 16;
  while (i < 39) {
    printnl_int(table_put(st, i, -2 * i + 50));
    i = i + 1;
  }

  i = -40;
  while (i < 40) {
    printnl_int(table_contains(st, i));
    i = i + 1;
  }
  print_table(st);

  table_free(st);
}

int main() {
  test1();
  test2();
  test3();
  test4();
}
