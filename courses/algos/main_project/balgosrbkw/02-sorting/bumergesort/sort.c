#include "sort.h"
#include "lealloc.h"

// Bottum Up merge sort
// Contrary to top-down version, We do one pass to merge all subarrays of size
// 1, then 2, then 4, then 8, until we reach full array size O(nln(n))

static int_t cmp(int_t arr, int_t i, int_t j) {
  return std_fmemget(arr + i) - std_fmemget(arr + j);
}

static int_t min(int_t a, int_t b) { return a < b ? a : b; }

// arr[beg:mid] and and arr[mid:end] are sorted
// copy arr[beg:end] into ws[beg:end]
// merge both array into one sorted array arr[beg:end]
// ws temporary memory space
static void merge(int_t arr, int_t ws, int_t beg, int_t mid, int_t end) {
  int_t i = beg;
  int_t j = mid;
  std_fmemcpy(ws + beg, arr + beg, end - beg);

  int_t k = beg;
  while (k < end) {
    int_t read_i = 1;
    if (i == mid) {
      read_i = 0;
    } else if (j == end) {
      read_i = 1;
    } else {
      read_i = cmp(ws, i, j) < 0;
    }

    int_t val = 0;
    if (read_i) {
      val = std_fmemget(ws + i);
      i = i + 1;
    } else {
      val = std_fmemget(ws + j);
      j = j + 1;
    }

    std_fmemset(arr + k, val);
    k = k + 1;
  }
}

void sort(int_t arr, int_t len) {
  int_t ws = fm_alloc(len);
  int_t sz = 1;
  while (sz < len) {

    int_t beg = 0;
    while (beg < len - sz) {
      int_t mid = beg + sz;
      int_t end = min(beg + 2 * sz, len);
      merge(arr, ws, beg, mid, end);
      beg = end;
    }

    sz = sz * 2;
  }

  fm_free(ws);
}
