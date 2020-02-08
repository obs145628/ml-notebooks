#include "sort.h"

// shell sort
// try to speed insertion sort
// insertion sort slow because it need to swap with all elements after it's
// right position shellsort try to swap over bigger ranges to move values faster
// it use h-sorted array: every h-th item in the erray is sorted
// [eg: 0, h, 2h, ...] sorted, [1, h+1, 2h + 1, ...] sorted, etc
// By starting with big values of h and reduce it until h=1 we get a sorted
// array We use insertion sort to h-sort the array O(n^2)

static int_t cmp(int_t arr, int_t i, int_t j) {
  return std_fmemget(arr + i) - std_fmemget(arr + j);
}

static void swap(int_t arr, int_t i, int_t j) {
  int_t vi = std_fmemget(arr + i);
  std_fmemset(arr + i, std_fmemget(arr + j));
  std_fmemset(arr + j, vi);
}

void sort(int_t arr, int_t len) {

  int_t h = 1;
  while (h < len / 3) {
    h = 3 * h + 1;
  }

  while (h > 0) {
    int_t i = h;
    while (i < len) {

      int_t j = i;
      while (j > h - 1 && cmp(arr, j, j - h) < 0) {
        swap(arr, j, j - h);
        j = j - h;
      }

      i = i + 1;
    }

    h = h / 3;
  }
}
