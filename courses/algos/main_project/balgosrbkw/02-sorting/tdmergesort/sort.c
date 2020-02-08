#include "sort.h"
#include "lealloc.h"

// Top down merge sort
// Divide array in two, sort borth subarrays
// Merge both sorted subarrays in one
// Sort subarrays by recursive call
// End case: subarray is of size <= 1: already sorted
// O(nln(n))

static int_t cmp(int_t arr, int_t i, int_t j) {
  return std_fmemget(arr + i) - std_fmemget(arr + j);
}

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

static void sort_rec(int_t arr, int_t ws, int_t beg, int_t end) {
  if (end - beg > 1) {
    int_t mid = beg + (end - beg) / 2;
    sort_rec(arr, ws, beg, mid);
    sort_rec(arr, ws, mid, end);
    merge(arr, ws, beg, mid, end);
  }
}

void sort(int_t arr, int_t len) {
  int_t ws = fm_alloc(len);
  sort_rec(arr, ws, 0, len);
  fm_free(ws);
}
