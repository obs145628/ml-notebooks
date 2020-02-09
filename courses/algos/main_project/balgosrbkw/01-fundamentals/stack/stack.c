#include "stack.h"
#include "lealloc.h"
#include "ledebug.h"

static void stack_resize(int_t stack, int_t new_cap) {
  int_t len = std_fmemget(stack + 1);
  int_t arr = std_fmemget(stack + 2);

  int_t new_arr = fm_alloc(new_cap);
  std_fmemcpy(new_arr, arr, len);
  fm_free(arr);
  std_fmemset(stack, new_cap);
  std_fmemset(stack + 2, new_arr);
}

int_t stack_new() {
  int_t stack = fm_alloc(3);
  int_t arr = fm_alloc(2);
  std_fmemset(stack, 2);
  std_fmemset(stack + 1, 0);
  std_fmemset(stack + 2, arr);
  return stack;
}

void stack_free(int_t stack) {
  int_t arr = std_fmemget(stack + 2);
  fm_free(arr);
  fm_free(stack);
}

void stack_push(int_t stack, int_t val) {
  int_t cap = std_fmemget(stack);
  int_t len = std_fmemget(stack + 1);
  if (len == cap)
    stack_resize(stack, 2 * cap);

  int_t arr = std_fmemget(stack + 2);
  std_fmemset(arr + len, val);
  std_fmemset(stack + 1, len + 1);
}

int_t stack_pop(int_t stack) {
  int_t cap = std_fmemget(stack);
  int_t len = std_fmemget(stack + 1);
  panic_ifn(len > 0);
  if (len > 1 && len - 1 == cap / 4)
    stack_resize(stack, cap / 2);

  int_t arr = std_fmemget(stack + 2);
  std_fmemset(stack + 1, len - 1);
  return std_fmemget(arr + len - 1);
}

int_t stack_size(int_t stack) { return std_fmemget(stack + 1); }
