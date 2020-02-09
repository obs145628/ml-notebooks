#include "table.h"
#include "lealloc.h"
#include "ledebug.h"

static int_t find_key(int_t st, int_t key) {
  int_t node = std_fmemget(st);
  int_t target = 0;

  while (node) {
    if (std_fmemget(node) == key) {
      target = node;
      node = 0;
    } else {
      node = std_fmemget(node + 2);
    }
  }

  return target;
}

int_t table_new() {
  int_t st = fm_alloc(2);
  std_fmemset(st, 0);
  std_fmemset(st + 1, 0);
  return st;
}

void table_free(int_t st) {
  int_t node = std_fmemget(st);
  while (node) {
    int_t next = std_fmemget(node + 2);
    fm_free(node);
    node = next;
  }

  fm_free(st);
}

int_t table_put(int_t st, int_t key, int_t val) {
  int_t insert = 1;
  int_t root = std_fmemget(st);
  int_t node = root;

  while (node) {
    int_t node_key = std_fmemget(node);
    int_t node_next = std_fmemget(node + 2);

    if (node_key == key) {
      std_fmemset(node + 1, val);
      insert = 0;
      node = 0;
    } else
      node = node_next;
  }

  if (insert) {
    int_t new_root = fm_alloc(3);
    std_fmemset(new_root, key);
    std_fmemset(new_root + 1, val);
    std_fmemset(new_root + 2, root);
    std_fmemset(st, new_root);
    std_fmemset(st + 1, std_fmemget(st + 1) + 1);
  }

  return insert;
}

// Remove the entry associated with the key
// Returns 1 if the key was found and deleted, 0 otherwhise
int_t table_delete(int_t st, int_t key) {
  int_t node = std_fmemget(st);
  int_t target = 0;
  int_t prev_ptr = st;

  while (node) {
    if (std_fmemget(node) == key) {
      target = node;
      node = 0;
    } else {
      prev_ptr = node + 2;
      node = std_fmemget(prev_ptr);
    }
  }

  if (target == 0) {
    return 0;
  } else {
    int_t target_next = std_fmemget(target + 2);
    std_fmemset(prev_ptr, target_next);
    fm_free(target);
    std_fmemset(st + 1, std_fmemget(st + 1) - 1);
    return 1;
  }
}

int_t table_get(int_t st, int_t key) {
  int_t node = find_key(st, key);
  panic_ifn(node);
  return std_fmemget(node + 1);
}

int_t table_contains(int_t st, int_t key) {
  return (find_key(st, key) == 0) == 0;
}

int_t table_size(int_t st) { return std_fmemget(st + 1); }

int_t table_it_new(int_t st) {
  int_t node = std_fmemget(st);
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
    int_t next = std_fmemget(node + 2);
    std_fmemset(it, next);
  }
}
