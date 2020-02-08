#include "sort.h"

// quicksort
// select a pivot, and parition the array such that pivot is in right place i,
// and all items in arr[0:i] are < pivot, and all items in arr[i+1:len] are >
// pivot sort subarrays arr[0:i] and arr[i+1:len] => whole array sorted
// subarrays are sorted by recursive call to quicksort
// base case: subarray of size <= 1: already sorted
// O(nlog(n))

static void swap(int_t arr, int_t i, int_t j) {
  int_t vi = std_fmemget(arr + i);
  std_fmemset(arr + i, std_fmemget(arr + j));
  std_fmemset(arr + j, vi);
}

// chosse pivot value v
// put all other values <= v into arr[beg:mid]
// and all other values >= v into arr[mid+1:end]
// value v is in it's final place arr[mid]
// returns mid

static int_t partition(int_t arr, int_t beg, int_t end) {
  int_t pi = beg;
  int_t pval = std_fmemget(arr + pi);
  swap(arr, beg, pi);

  int_t i = beg + 1;
  int_t j = end - 1;
  int_t cross = 0;

  while (cross == 0) {

    while (i < end ? std_fmemget(arr + i) < pval : 0) {
      i = i + 1;
    }

    while (j > beg ? std_fmemget(arr + j) > pval : 0) {
      j = j - 1;
    }

    if (i >= j) {
      cross = 1;
    } else {
      swap(arr, i, j);
    }
  }

  i = i - 1;

  swap(arr, beg, i);
  return i;
}

static void sort_rec(int_t arr, int_t beg, int_t end) {
  if (end - beg > 1) {
    int_t mid = partition(arr, beg, end);
    sort_rec(arr, beg, mid);
    sort_rec(arr, mid + 1, end);
  }
}

void sort(int_t arr, int_t len) { sort_rec(arr, 0, len); }
