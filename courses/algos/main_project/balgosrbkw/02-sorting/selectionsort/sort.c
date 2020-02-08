#include "sort.h"

// selection sort
// at step i, arr[0:i] sorted
// find smallest elemen in arr[i+1:..], and swap it with arr[i]
// O(n^2)

static int_t cmp(int_t arr, int_t i, int_t j) {
  return std_fmemget(arr + i) - std_fmemget(arr + j);
}

static void swap(int_t arr, int_t i, int_t j) {
  int_t vi = std_fmemget(arr + i);
  std_fmemset(arr + i, std_fmemget(arr + j));
  std_fmemset(arr + j, vi);
}

void sort(int_t arr, int_t len) {
  int_t i = 0;
  while (i < len) {
    int_t min = i;

    int_t j = i + 1;
    while (j < len) {
      if (cmp(arr, j, min) < 0) {
        min = j;
      }
      j = j + 1;
    }

    swap(arr, min, i);
    i = i + 1;
  }
}
