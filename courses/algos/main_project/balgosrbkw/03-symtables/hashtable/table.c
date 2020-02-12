#include "table.h"
#include "lealloc.h"
#include "ledebug.h"

// Implementation based on hash table with separate hashing
// Separate hasing is implemented using a linked list
// Operations are amortized O(1)
// (If hash function produce a normal distribution of its inputs)

// Create a new node to store the they, and set it's next element as the head of
// the list
static int_t node_new(int_t key, int_t val, int_t head) {
  int_t res = fm_alloc(3);
  std_fmemset(res, key);
  std_fmemset(res + 1, val);
  std_fmemset(res + 2, head);
  return res;
}

static void ll_free(int_t l) {
  while (l) {
    int_t next = std_fmemget(l + 2);
    fm_free(l);
    l = next;
  }
}

// Returns the pointer to the node of `key` in the linked list
// Or 0 if `key` not found
static int_t ll_find(int_t l, int_t key) {
  int_t target = 0;

  while (l) {
    if (std_fmemget(l) == key) {
      target = l;
      l = 0;
    } else {
      l = std_fmemget(l + 2);
    }
  }

  return target;
}

// Like ll_find, but takes a pointer to node pointer, and returns a pointer to
// node pointer of node with right key (or 0)
static int_t ll_find_ptr(int_t l_ptr, int_t key) {
  int_t target = 0;
  int_t l = std_fmemget(l_ptr);

  while (l) {
    if (std_fmemget(l) == key) {
      target = l_ptr;
      l = 0;
    } else {
      l_ptr = l + 2;
      l = std_fmemget(l_ptr);
    }
  }

  return target;
}

// Performs hashing of integer to integer
static int_t hash_fn(int_t x) { return x * 2654435761; }

// Hash key and change the value to the right range to be an array index
static int_t hash_key(int_t x, int_t len) {
  x = hash_fn(x);
  x = x > 0 ? x : -x;
  return x % len;
}

int_t table_new() {
  int_t n = 37;
  int_t st = fm_alloc(2 + n);
  std_fmemset(st, n);
  std_fmemset(st + 1, 0);

  int_t i = 0;
  while (i < n) {
    std_fmemset(st + 2 + i, 0);
    i = i + 1;
  }

  return st;
}

void table_free(int_t st) {
  int_t n = std_fmemget(st);
  int_t i = 0;
  while (i < n) {
    ll_free(std_fmemget(st + 2 + i));
    i = i + 1;
  }

  fm_free(st);
}

int_t table_put(int_t st, int_t key, int_t val) {
  int_t n = std_fmemget(st);
  int_t idx = hash_key(key, n);
  int_t head = std_fmemget(st + 2 + idx);
  int_t node = ll_find(head, key);

  if (node) {
    std_fmemset(node + 1, val);
    return 0;
  } else {
    head = node_new(key, val, head);
    std_fmemset(st + 2 + idx, head);
    std_fmemset(st + 1, std_fmemget(st + 1) + 1);
    return 1;
  }
}

int_t table_delete(int_t st, int_t key) {
  int_t n = std_fmemget(st);
  int_t idx = hash_key(key, n);
  int_t head_ptr = st + 2 + idx;
  int_t node_ptr = ll_find_ptr(head_ptr, key);

  if (node_ptr) {
    int_t node = std_fmemget(node_ptr);
    panic_ifn(node);
    std_fmemset(node_ptr, std_fmemget(node + 2));
    fm_free(node);
    std_fmemset(st + 1, std_fmemget(st + 1) - 1);
    return 1;
  } else {
    return 0;
  }
}

int_t table_get(int_t st, int_t key) {
  int_t n = std_fmemget(st);
  int_t idx = hash_key(key, n);
  int_t head = std_fmemget(st + 2 + idx);
  int_t node = ll_find(head, key);
  panic_ifn(node);
  return std_fmemget(node + 1);
}

int_t table_contains(int_t st, int_t key) {
  int_t n = std_fmemget(st);
  int_t idx = hash_key(key, n);
  int_t head = std_fmemget(st + 2 + idx);
  return (ll_find(head, key) == 0) == 0;
}

int_t table_size(int_t st) { return std_fmemget(st + 1); }

int_t table_it_new(int_t st) {
  int_t it = fm_alloc(3);
  std_fmemset(it, 0);
  std_fmemset(it + 1, -1);
  std_fmemset(it + 2, st);
  table_it_next(it);
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
  int_t idx = std_fmemget(it + 1);
  int_t st = std_fmemget(it + 2);
  int_t n = std_fmemget(st);

  if (idx < n) {
    if (node)
      node = std_fmemget(node + 2);
    if (!node) {
      int_t found = 0;
      idx = idx + 1;
      while (found == 0) {
        if (idx == n) {
          found = 1;
        } else if (std_fmemget(st + 2 + idx)) {
          found = 1;
        } else {
          idx = idx + 1;
        }
      }

      node = idx < n ? std_fmemget(st + 2 + idx) : 0;
      std_fmemset(it + 1, idx);
    }

    std_fmemset(it, node);
  }
}
