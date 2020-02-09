#include "sort.h"

// 3-way quicksort
// Choose pivot value v
// Partition array into 3 parts: a[beg:lt] < v, a[lt:gt+1] == v, a[gt+1:end] > v
// Sort subarrays arr[beg:lt] and arr[gt+1:end] => whole array sorted
// subarrays are sorted by recursive call to quicksort
// base case: subarray of size <= 1: already sorted
// O(nlog(n))

static void swap(int_t arr, int_t i, int_t j) {
  int_t vi = std_fmemget(arr + i);
  std_fmemset(arr + i, std_fmemget(arr + j));
  std_fmemset(arr + j, vi);
}

static void sort_rec(int_t arr, int_t beg, int_t end) {
  if (end - beg > 1) {

    int_t lt = beg;
    int_t gt = end - 1;
    int_t i = beg + 1;
    int_t v = std_fmemget(arr + beg);

    while (i <= gt) {
      if (std_fmemget(arr + i) < v) {
        swap(arr, lt, i);
        lt = lt + 1;
        i = i + 1;
      } else if (std_fmemget(arr + i) > v) {
        swap(arr, i, gt);
        gt = gt - 1;
      } else {
        i = i + 1;
      }
    }

    sort_rec(arr, beg, lt);
    sort_rec(arr, gt + 1, end);
  }
}

void sort(int_t arr, int_t len) { sort_rec(arr, 0, len); }
