#include "leio.h"

// Binary search algorithm
// Return index of key in v, or -1 if not found
// v must be sorted
int_t rank(int_t arr, int_t len, int_t key);

// Algorithm:
// Look at the miidle of the array:
// If key lower, than key must be in lower half
// If key bigger, than must be in upper half
// This way search zone is halfed every iteration
//
// Complexity:
// O(log(v.size())
int_t rank(int_t arr, int_t len, int_t key) {
  if (len == 0)
    return -1;
  int_t beg = 0;
  int_t end = len - 1;

  while (beg <= end) {
    int_t mid = beg + (end - beg) / 2;
    int_t mid_val = std_fmemget(arr + mid);
    if (key < mid_val)
      end = mid - 1;
    else if (key > mid_val)
      beg = mid + 1;
    else
      return mid;
  }

  return -1;
}

void test_empty() { printnl_int(rank(160, 0, 56)); }

void test1() {
  int_t i = 0;
  while (i < 10) {
    std_fmemset(160 + i, i);
    i += 1;
  }

  printnl_int(rank(160, 10, -50));
  i = 0;
  while (i < 10) {
    printnl_int(rank(160, 10, i));
    i += 1;
  }
  printnl_int(rank(160, 10, 63));
}

void test2() {
  int_t i = 0;
  while (i < 10) {
    std_fmemset(160 + i, i - 5);
    i += 1;
  }

  printnl_int(rank(160, 10, -50));
  i = -10;
  while (i < 10) {
    printnl_int(rank(160, 10, i));
    i += 1;
  }
  printnl_int(rank(160, 10, 63));
}

void test3() {
  int_t i = 0;
  while (i < 10) {
    std_fmemset(160 + i, 2 * i);
    i += 1;
  }

  printnl_int(rank(160, 10, -50));
  i = 0;
  while (i < 20) {
    printnl_int(rank(160, 10, i));
    i += 1;
  }
  printnl_int(rank(160, 10, 63));
}

int main() {
  test_empty();
  test1();
  test2();
  test3();
}
