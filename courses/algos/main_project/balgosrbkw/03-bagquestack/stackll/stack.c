#include "stack.h"
#include "lealloc.h"
#include "ledebug.h"

int_t stack_new() {
  int_t stack = fm_alloc(2);
  std_fmemset(stack, 0);
  std_fmemset(stack, 1);
  return stack;
}

void stack_free(int_t stack) {
  int_t node = std_fmemget(stack);
  while (node) {
    int_t next_node = std_fmemget(node + 1);
    fm_free(node);
    node = next_node;
  }
}

void stack_push(int_t stack, int_t val) {
  int_t root = std_fmemget(stack);
  int_t node = fm_alloc(2);
  std_fmemset(node, val);
  std_fmemset(node + 1, root);
  std_fmemset(stack, node);
  std_fmemset(stack + 1, std_fmemget(stack + 1) + 1);
}

int_t stack_pop(int_t stack) {
  int_t root = std_fmemget(stack);
  panic_ifn(root);

  int_t val = std_fmemget(root);
  int_t new_root = std_fmemget(root + 1);
  fm_free(root);

  std_fmemset(stack, new_root);
  std_fmemset(stack + 1, std_fmemget(stack + 1) - 1);
  return val;
}

int_t stack_size(int_t stack) { return std_fmemget(stack + 1); }
