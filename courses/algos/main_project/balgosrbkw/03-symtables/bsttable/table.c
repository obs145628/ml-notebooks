#include "table.h"
#include "lealloc.h"
#include "ledebug.h"

// Implementation based on BST (Binary Search Tree)
// All operations are linear in the height of the tree.
// In average height(T) = ln(size(T))
// But in wort cases all operations can be in O(n): depends on the insertion
// order
//
// BST: every node has a key, and 2 childs.
// the left child has a key < parent key
// the right child has a key > parent key
// every child node may be NULL (no child)

static int_t node_new(int_t key, int_t val, int_t left, int_t right,
                      int_t parent) {
  int_t node = fm_alloc(5);
  std_fmemset(node, key);
  std_fmemset(node + 1, val);
  std_fmemset(node + 2, left);
  std_fmemset(node + 3, right);
  std_fmemset(node + 4, parent);
  return node;
}

static void node_free(int_t node) {
  if (node) {
    node_free(std_fmemget(node + 2));
    node_free(std_fmemget(node + 3));
    fm_free(node);
  }
}

static void node_swap(int_t a, int_t b) {
  int_t a_key = std_fmemget(a);
  int_t a_val = std_fmemget(a + 1);
  std_fmemset(a, std_fmemget(b));
  std_fmemset(a + 1, std_fmemget(b + 1));
  std_fmemset(b, a_key);
  std_fmemset(b + 1, a_val);
}

static int_t node_min(int_t node) {
  int_t left = std_fmemget(node + 2);
  return left == 0 ? node : node_min(left);
}

// Search for an entry in the tree
// Returns the node pointer if found, otherwhise 0
//
// Algorithm:
// 1) if node is null => not found
// 2) Compare key with node.key:
//    - key < node.key => search in node.left
//    - key > node.key => search in node.right
//    - key == node.key => found
static int_t node_find(int_t node, int_t key) {
  if (node == 0) {
    return 0;
  } else {
    int_t cmp = key - std_fmemget(node);
    if (cmp < 0)
      return node_find(std_fmemget(node + 2), key);
    else if (cmp > 0)
      return node_find(std_fmemget(node + 3), key);
    else
      return node;
  }
}

// Insert or update [key/val] in the right place in the tree
// Returns 1 if insertion, 0 if update
//
// Algorithm:
// 1)
// - if key == node.key => update node.val = val
// - if key < node.key => insert in node.left
// - if key > node.key => insert in node.right
// 2)
// For recursive call, if left/right child is none, create new left/right child
// with key/val pair, and no children
static int_t node_put(int_t node_ptr, int_t parent, int_t key, int_t val) {
  int_t node = std_fmemget(node_ptr);
  if (node == 0) {
    std_fmemset(node_ptr, node_new(key, val, 0, 0, parent));
    return 1;
  } else {
    int_t cmp = key - std_fmemget(node);
    if (cmp == 0) {
      std_fmemset(node + 1, val);
      return 0;
    } else {
      int_t child_ptr = cmp < 0 ? node + 2 : node + 3;
      return node_put(child_ptr, node, key, val);
    }
  }
}

// Remove entry from the tree
// Returns 1 if entry found and deleted, 0 otherwhise
//
// Algorithm:
// 1) If node is null, delete failed
// 2) Compare key with node.key
// - if key < node.key => recursive delete key in node.left
// - if key > node.key => recursive delete key in node.right
// - if key == node.key => found node, go to (3)
//
// 3) If one of the children is null, move up the other child
// - if node.left is null, replace node with node.right
// - else if node.right is null, replace node with node.left
// - else => go to (4)
//
// 4) find a replacement node: either the rightmost node of left child, or
// leftmost node or right child 5) swap key/val of replacement node with current
// node 6) recursive delete of key in replacement node
static int_t node_del(int_t node_ptr, int_t key) {
  int_t node = std_fmemget(node_ptr);
  if (node == 0) {
    return 0;
  }

  else {
    int_t cmp = key - std_fmemget(node);
    if (cmp) {
      int_t child_ptr = cmp < 0 ? node + 2 : node + 3;
      return node_del(child_ptr, key);
    } else {
      int_t left = std_fmemget(node + 2);
      int_t right = std_fmemget(node + 3);

      if (left == 0) {
        std_fmemset(node_ptr, right);
        fm_free(node);
      } else if (right == 0) {
        std_fmemset(node_ptr, left);
        fm_free(node);
      } else {
        int_t rep_node = node_min(right);
        node_swap(node, rep_node);
        panic_ifn(node_del(node + 3, key));
      }

      return 1;
    }
  }
}

// Move to next node (in-order walk)
// Returns pointer to next node to stop
// Returns null if node is the last one
//
// Algorithm
// 1) Try to go right
// - if right child is not null, next is leftmost child of right
// - if right child is null, go to step 2
// 2) Move to the parent
// - if parent is null, this was the last node to visit
// - if moved up from left, next node is the parent
// - if moved up from right, recursive node_next of parent
static int_t node_next(int_t node) {
  int_t right = std_fmemget(node + 3);
  if (right) {
    return node_min(right);
  } else {
    int_t parent = std_fmemget(node + 4);
    if (parent == 0) {
      return 0;
    } else {
      int_t from_left = std_fmemget(parent + 2) == node;
      return from_left ? parent : node_next(parent);
    }
  }
}

int_t table_new() {
  int_t st = fm_alloc(2);
  std_fmemset(st, 0);
  std_fmemset(st + 1, 0);
  return st;
}

void table_free(int_t st) {
  node_free(std_fmemget(st));
  fm_free(st);
}

int_t table_put(int_t st, int_t key, int_t val) {
  int_t res = node_put(st, 0, key, val);
  std_fmemset(st + 1, std_fmemget(st + 1) + res);
  return res;
}

int_t table_delete(int_t st, int_t key) {
  int_t res = node_del(st, key);
  std_fmemset(st + 1, std_fmemget(st + 1) - res);
  return res;
}

int_t table_get(int_t st, int_t key) {
  int_t node = node_find(std_fmemget(st), key);
  panic_ifn(node);
  return std_fmemget(node + 1);
}

int_t table_contains(int_t st, int_t key) {
  return (node_find(std_fmemget(st), key) == 0) == 0;
}

int_t table_size(int_t st) { return std_fmemget(st + 1); }

// In-order walk
// Visit left, visit node, visit right
// Make sure all nodes are visited in key increase order

int_t table_it_new(int_t st) {
  int_t root = std_fmemget(st);
  int_t node = root ? node_min(root) : 0;

  int_t it = fm_alloc(1);
  std_fmemset(it, node);
  return it;
}

void table_it_free(int_t it) { fm_free(it); }

int_t table_it_is_end(int_t it) {
  int_t node = std_fmemget(it);
  return node == 0;
}

int_t table_it_get_key(int_t it) {
  int_t node = std_fmemget(it);
  panic_ifn(node);
  return std_fmemget(node);
}

int_t table_it_get_val(int_t it) {
  int_t node = std_fmemget(it);
  panic_ifn(node);
  return std_fmemget(node + 1);
}

void table_it_next(int_t it) {
  int_t node = std_fmemget(it);
  if (node) {
    node = node_next(node);
  }
  std_fmemset(it, node);
}
