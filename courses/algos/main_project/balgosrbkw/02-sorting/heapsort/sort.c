#include "sort.h"

// Heapsort
// Perform sorting using a max binary heap
// Binary heap: implementation of priority queue
// Complete binary tree
// For each node of the tree, the value of it's children are bigger than the
// value of the node
// => The smallest value is at the root
// The whole tree represented by an array of base-1 index
// Root at index 1
// Parent of node k is k/2, And children of node k are 2*k and 2*k + 1

static int_t node_addr(int_t h, int_t k) { return h - 1 + k; }

static void node_swap(int_t h, int_t p, int_t q) {
  int_t pval = std_fmemget(node_addr(h, p));
  std_fmemset(node_addr(h, p), std_fmemget(node_addr(h, q)));
  std_fmemset(node_addr(h, q), pval);
}

static int_t node_cmp(int_t h, int_t p, int_t q) {
  return std_fmemget(node_addr(h, p)) - std_fmemget(node_addr(h, q));
}

// Top-down reheapify
// When inserting a node at position k, might violate the rule that its bigger
// than its children.
// Must swap node with the biggest of it's children
// Swap nodes with its children toward the leaves, until rule is ensured again.
static void sink(int_t h, int_t k, int_t len) {
  int_t valid = 0;

  while (valid == 0) {
    if (2 * k > len) {
      valid = 1;
    } else {
      int_t j = 2 * k;
      if (j < len ? node_cmp(h, j + 1, j) > 0 : 0) {
        j = j + 1;
      }

      if (node_cmp(h, k, j) > 0) {
        valid = 1;
      } else {
        node_swap(h, k, j);
        k = j;
      }
    }
  }
}

// First step: transform array into max heap
// All items at the leaves are already sunk
// All other items are sink by one one to construct the heap
//
// Second step: build sorted array from right to left
// Swap max value with last value of heap (new right inserted value)
// Reduce heap size by 1, and sink root value
// Iterate until heap size goes down to 1
void sort(int_t arr, int_t len) {

  int_t i = len / 2;
  while (i > 0) {
    sink(arr, i, len);
    i = i - 1;
  }

  i = len;
  while (i > 1) {
    node_swap(arr, 1, i);
    sink(arr, 1, i - 1);
    i = i - 1;
  }
}
