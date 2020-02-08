#include "uf.h"
#include "lealloc.h"
#include "ledebug.h"

// Implementation based on a forest of tree
// Each set is a tree, identified by it's root
// In the tree, every node has a single parent, and any number of children
// To find the set an object belongs to, we simply go through the parent's until
// reaching the root To perform the union of 2 sets, we simply set the root
// parent of one to the root of the other Operations costs are linear with the
// tree height. In order to reduce height, when doing union we attach the root
// of the smallest tree to the root of largest one To do this we keep track of
// the size of every trees

static int_t id_addr(int_t uf, int_t idx) { return uf + 1 + 2 * idx; }

static int_t size_addr(int_t uf, int_t idx) { return uf + 1 + 2 * idx + 1; }

int_t uf_new(int_t n) {
  int_t uf = fm_alloc(1 + 2 * n);
  std_fmemset(uf, n);
  int_t i = 0;
  while (i < n) {
    std_fmemset(id_addr(uf, i), i);
    std_fmemset(size_addr(uf, i), 1);
    i = i + 1;
  }

  return uf;
}

void uf_free(int_t uf) { fm_free(uf); }

void uf_union(int_t uf, int_t p, int_t q) {
  int_t pr = uf_find(uf, p);
  int_t qr = uf_find(uf, q);

  if (pr != qr) {
    int_t pr_size = std_fmemget(size_addr(uf, pr));
    int_t qr_size = std_fmemget(size_addr(uf, qr));
    std_fmemset(uf, std_fmemget(uf) - 1);

    if (pr_size < qr_size) {
      std_fmemset(id_addr(uf, pr), qr);
      std_fmemset(size_addr(uf, qr), qr_size + pr_size);
    } else {
      std_fmemset(id_addr(uf, qr), pr);
      std_fmemset(size_addr(uf, pr), pr_size + qr_size);
    }
  }
}

int_t uf_find(int_t uf, int_t p) {
  while (p != std_fmemget(id_addr(uf, p))) {
    p = std_fmemget(id_addr(uf, p));
  }
  return p;
}

int_t uf_connected(int_t uf, int_t p, int_t q) {
  return uf_find(uf, p) == uf_find(uf, q);
}

int_t uf_count(int_t uf) { return std_fmemget(uf); }
