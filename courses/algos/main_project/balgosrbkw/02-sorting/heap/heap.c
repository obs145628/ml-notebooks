#include "heap.h"
#include "lealloc.h"
#include "ledebug.h"

// Binary heap: implementation of priority queue
// Complete binary tree
// For each node of the tree, the value of it's children are bigger than the
// value of the node
// => The smallest value is at the root
// The whole tree represented by an array of base-1 index
// Root at index 1
// Parent of node k is k/2, And children of node k are 2*k and 2*k + 1

static void heap_resize(int_t h, int_t new_cap) {
  int_t len = std_fmemget(h);
  int_t arr = std_fmemget(h + 2);
  int_t new_arr = fm_alloc(new_cap);

  std_fmemcpy(new_arr, arr, len);
  std_fmemset(h + 1, new_cap);
  std_fmemset(h + 2, new_arr);
  fm_free(arr);
}

static int_t node_addr(int_t h, int_t k) {
  int_t arr = std_fmemget(h + 2);
  return arr - 1 + k;
}

static void node_swap(int_t h, int_t p, int_t q) {
  int_t pval = std_fmemget(node_addr(h, p));
  std_fmemset(node_addr(h, p), std_fmemget(node_addr(h, q)));
  std_fmemset(node_addr(h, q), pval);
}

static int_t node_cmp(int_t h, int_t p, int_t q) {
  return std_fmemget(node_addr(h, p)) - std_fmemget(node_addr(h, q));
}

// Bottum-up reheapify
// When inserting a node at position k, might violate the rule that it is bigger
// than its parent.
// Swap node with its parents towards the root, until rule is ensured again
static void swim(int_t h, int_t k) {
  while (k > 1 ? node_cmp(h, k, k / 2) < 0 : 0) {
    node_swap(h, k, k / 2);
    k = k / 2;
  }
}

// Top-down reheapify
// When inserting a node at position k, might violate the rule that its smaller
// than its children.
// Must swap node with the smallest of it's children
// Swap nodes with its children toward the leaves, until rule is ensured again.
static void sink(int_t h, int_t k) {
  int_t valid = 0;
  int_t len = std_fmemget(h);

  while (valid == 0) {
    if (2 * k > len) {
      valid = 1;
    } else {
      int_t j = 2 * k;
      if (j < len ? node_cmp(h, j + 1, j) < 0 : 0) {
        j = j + 1;
      }

      if (node_cmp(h, k, j) < 0) {
        valid = 1;
      } else {
        node_swap(h, k, j);
        k = j;
      }
    }
  }
}

int_t heap_new() {
  int_t h = fm_alloc(3);
  int_t arr = fm_alloc(4);
  std_fmemset(h, 0);
  std_fmemset(h + 1, 4);
  std_fmemset(h + 2, arr);
  return h;
}

void heap_free(int_t h) {
  fm_free(h + 2);
  fm_free(h);
}

// add item at the end of the array and swim the value towards the root
void heap_push(int_t h, int_t val) {
  int_t len = std_fmemget(h);
  int_t cap = std_fmemget(h + 1);
  if (len == cap) {
    heap_resize(h, cap * 2);
  }

  std_fmemset(node_addr(h, len + 1), val);
  std_fmemset(h, len + 1);
  swim(h, len + 1);
}

// Put last item on top of the list, and sink value towards the leaves
int_t heap_pop(int_t h) {
  int_t len = std_fmemget(h);
  panic_ifn(len > 0);
  int_t res = std_fmemget(node_addr(h, 1));

  node_swap(h, 1, len);
  std_fmemset(h, len - 1);
  sink(h, 1);

  return res;
}

// Return smallest items of the heap
int_t heap_min(int_t h) {
  panic_ifn(heap_size(h) > 0);
  return std_fmemget(node_addr(h, 1));
}

int_t heap_size(int_t h) { return std_fmemget(h); }
